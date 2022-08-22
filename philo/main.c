/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/22 17:18:14 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int		check_args(char **av, int count, int *philo)
{
	int	i;

	i = -1;
	while (i++ < count - 2)
	{
		philo[i] = positive_atoi(av[i + 1]);
	}
	while (i > -1)
	{
		if (philo[i--] == -1)
			return (0);
	}
	if (count == 5)
		philo[4] = -1;
	return (1);
}

void*	routine()
{
	printf("i will sleep now\n");
	sleep(1);
	printf("i will pretend to eat \n");
	return ((void *)1);	
}

// creating each thread:
int		threads_creation(int num, pthread_t *th)
{
	int		i;

	i = 0;
	while (num > 0)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL))
		{
			printf("some problem had occured while creating the %dth thread\n", i + 1);
			return (0);
		}
		if (pthread_join(th[i], NULL))
		{
			printf("an Error occured while waiting for thread number %d to terminate\n", i + 1);
			return (0);
		}
		(num)--;
		i++;
	}
	return (1);
}

int main(int ac, char **av)
{
	int			ph[5];
	pthread_t	*th;

	if (ac >= 5 && ac <= 6)
	{
		if(!check_args(av, ac, ph) || ph[0] == 0)
			return (0);
		th = malloc(ph[0] * sizeof(pthread_t));
		if (!threads_creation(ph[0], th))
			return;
	}
	else
		write(2, "Error!\nUsage:./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]", 126);
}
