# ifndef PHILOS_H
# define PHILOS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <pthread.h>

typedef struct s_philos_data
{
	int		n_of_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		eating_times;
}	t_philos;

int	positive_atoi(const char *str);
size_t	ft_strlen(const char *s);

#endif
