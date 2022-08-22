# ifndef PHILOS_H
# define PHILOS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <pthread.h>

int	positive_atoi(const char *str);
size_t	ft_strlen(const char *s);

typedef struct s_multi
{
	pthread_mutex_t mtx;
//	void	forks;
} t_multi;
#endif
