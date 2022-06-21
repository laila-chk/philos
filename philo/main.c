
/* header*/
/* header*/
/* header*/
/* header*/

#include "philos.h"

int main(int ac, char **av)
{
	int		philos;

	if (ac > 4)
	{
		philos = simple_atoi(av[1]); 	
		printf("---->%d<----\n", philos);
	}
	else
		write(2, "Error!\nUsage:./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]", 126);
}
