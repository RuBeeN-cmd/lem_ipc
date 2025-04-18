#include "lem_ipc.h"

void	ft_log(int level, char *msg)
{
	char	*prefix;
	int		fd = 1;
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
	ft_printf_fd(fd, "%s%s", prefix, msg);
}