#include <models/log.h>

void	ft_log(int level, const char *str, ...)
{
	va_list	args;
	char	*prefix;
	int		fd = 1;

	if (!str)
		return ;
	if (level >= LOG_WARNING)
		fd = 2;
	switch (level)
	{
		case LOG_DEBUG:
			prefix = "["C_MAG"DEBUG"C_END"] ";
			break ;
		case LOG_INFO:
			prefix = "["C_CYN"INFO"C_END"] ";
			break ;
		case LOG_WARNING:
			prefix = "["C_YEL"WARNING"C_END"] ";
			break ;
		case LOG_ERROR:
			prefix = "["C_RED"ERROR"C_END"] ";
			break ;
		default:
			prefix = "";
			break ;
	}
	va_start(args, str);
	ft_printf_fd(fd, "%s", prefix);
	ft_print_valist_fd(fd, str, args);
	ft_printf_fd(fd, "\n", prefix);
	va_end(args);
}