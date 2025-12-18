#include <utils/log.h>
#include <libft.h>

t_log_level *get_log_level() {
	static t_log_level current_log_level = LEVEL_INFO;
	return &current_log_level;
}

void set_log_level(t_log_level level) {
	t_log_level *current_log_level = get_log_level();
	*current_log_level = level;
}

static int *get_empty_prefix() {
	static int empty_prefix = 0;
	return &empty_prefix;
}

static pid_t get_saved_pid() {
	static pid_t pid = 0;
	if (pid == 0) {
		pid = getpid();
	}
	return pid;
}

char *get_log_prefix(t_log_level level) {
	static char buff[128] = {};
	int 		empty_prefix = *get_empty_prefix();
	ft_bzero(buff, sizeof(buff));

	if (empty_prefix)
		return buff;
	int idx = 0;
	ft_strlcpy(buff + idx, "[", sizeof(buff) - idx);
	idx += 1;
	ft_strlcpy(buff + idx, get_color(COLOR_LIGHTGRAY), sizeof(buff) - idx);
	idx += ft_strlen(get_color(COLOR_DARKGRAY));
	char *pid_str = ft_itoa(get_saved_pid());
	ft_strlcpy(buff + idx, pid_str, sizeof(buff) - idx);
	idx += ft_strlen(pid_str);
	free(pid_str);
	ft_strlcpy(buff + idx, "]", sizeof(buff) - idx);
	idx += 1;
	ft_strlcpy(buff + idx, "[", sizeof(buff) - idx);
	idx += 1;
	if (level == LEVEL_DEBUG) {
		char *color = get_color(COLOR_PURPLE);
		ft_strlcpy(buff + idx, color, sizeof(buff) - idx);
		idx += ft_strlen(color);
		ft_strlcpy(buff + idx, DBG_PREFIX, sizeof(buff) - idx);
		idx += ft_strlen(DBG_PREFIX);
	} else if (level == LEVEL_INFO) {
		char *color = get_color(COLOR_CYAN);
		ft_strlcpy(buff + idx, color, sizeof(buff) - idx);
		idx += ft_strlen(color);
		ft_strlcpy(buff + idx, INFO_PREFIX, sizeof(buff) - idx);
		idx += ft_strlen(INFO_PREFIX);
	} else if (level == LEVEL_WARNING) {
		char *color = get_color(COLOR_YELLOW);
		ft_strlcpy(buff + idx, color, sizeof(buff) - idx);
		idx += ft_strlen(color);
		ft_strlcpy(buff + idx, WARN_PREFIX, sizeof(buff) - idx);
		idx += ft_strlen(WARN_PREFIX);
	} else if (level == LEVEL_ERROR) {
		char *color = get_color(COLOR_RED);
		ft_strlcpy(buff + idx, color, sizeof(buff) - idx);
		idx += ft_strlen(color);
		ft_strlcpy(buff + idx, ERR_PREFIX, sizeof(buff) - idx);
		idx += ft_strlen(ERR_PREFIX);
	}
	char *reset = get_color(COLOR_RESET);
	ft_strlcpy(buff + idx, reset, sizeof(buff) - idx);
	idx += ft_strlen(reset);
	ft_strlcpy(buff + idx, "] ", sizeof(buff) - idx);
	idx += 2;
	return buff;
}

void	disable_prefix() {
	int *empty_prefix = get_empty_prefix();
	*empty_prefix = 1;
}