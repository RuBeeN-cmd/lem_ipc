#ifndef LOG_H
#define LOG_H

#include <libft.h>

#define C_END	"\033[0m"
#define C_RED	"\033[0;31m"
#define C_GRN	"\033[0;32m"
#define C_YEL	"\033[0;33m"
#define C_BLU	"\033[0;34m"
#define C_MAG	"\033[0;35m"
#define C_CYN	"\033[0;36m"
#define C_WHT	"\033[0;37m"

#define LOG_DEBUG	0
#define LOG_INFO	1
#define LOG_WARNING	2
#define LOG_ERROR	3

// log.c
void	ft_log(int level, char *msg);

#endif