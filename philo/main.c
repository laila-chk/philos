/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/22 23:05:39 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	print_error(int i)
{
	if (i == 1)
	{
		write(2, "Error!\nUsage:./philo number_of_philosophers", 43);
		write(2, "time_to_die time_to_eat time_to_sleep", 37);
		write(2, "[number_of_times_each_philosopher_must_eat]\n", 44);
	}
}

int	check_args(char **av, int count, int *philo)
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

void	*routine(void *arg)
{
	(void)arg;
	printf("i will sleep now\n");
	sleep(1);
	printf("i will pretend to eat \n");
	return ((void *)1);
	//---------time to eat:
	//mutex lock 
	//usleep()
	//mutex unlock
	//usleep()
	//~~~so we are basically locking a data (taking a fork) and using sleep to hold on there
	//~then we unlock that data (let go of the fork) and go to sleep.
	/**---- this one not gonna work----**/
	/*
	 *the idea is to use mutex_lock on mutexes of threads N+two so only those will access this
	 *data and others wont
	 *
	 * */
}

int	threads_creation(int num, pthread_t *th)
{
	int		i;

	i = 0;
	while (num > 0)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL))
		{
			printf("problem occured while creating the %dth thread\n", i + 1);
			return (0);
		}
		if (pthread_join(th[i], NULL))
		{
			printf("Error while waiting for thread %d to terminate\n", i + 1);
			return (0);
		}
		(num)--;
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{
	int			ph[5];
	pthread_t	*th;

	if (ac >= 5 && ac <= 6)
	{
		if (!check_args(av, ac, ph) || ph[0] == 0)
			return (1);
		th = malloc(ph[0] * sizeof(pthread_t));
		if (!threads_creation(ph[0], th))
			return (1);
		//watcher thread here!
		return (0);//kolchi smooth --> ret 1
	}
	print_error(1);//khrejna mn if or madkholnalachii asln ==> kayn chi mochkil
	return (1); 
}
