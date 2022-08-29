/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/29 14:44:44 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	print_error(int i)
{
	if (i == 1)
	{
		write(2, "Error!\nUsage:./philo number_of_philosophers ", 44);
		write(2, "time_to_die time_to_eat time_to_sleep ", 38);
		write(2, "[number_of_times_each_philosopher_must_eat]\n", 44);
	}
}

int	check_args(char **av, int count,int *vals)
{
	int	i;

	i = 0;
	while (i < count - 2)
	{
		vals[i] = positive_atoi(av[i + 1]);
		i++;
	}
	i = 0;
	while (i < count - 2)
	{
		if (vals[i] == -1)
			return (0);
		i++;
	}
	if (count == 5)
		vals[4] = -1;
	return (1);
}

void	impair(int i, t_ph *ph)
{
	if (i % 2 != 0) //feeding pair philos first
	{
		pthread_mutex_lock(&ph->fork);
		printf("fork %d was picked up---------- i = %d\n", i, i);
		if (i < ph->vals[0] - 2)
		{
			pthread_mutex_lock(&(ph+1)->fork);
			printf("fork %d was picked up----------i = %d\n", i+1, i);
		}
			printf("______________philosopher %d is eating..________________i = %d\n", i, i);
			sleep(1);
		pthread_mutex_unlock(&ph->fork);
		printf("**************fork %d back to table i = %d\n", i, i);
		if (i < ph->vals[0] - 2)
		{
			pthread_mutex_unlock(&(ph+1)->fork);
			printf("***************fork %d is back to table i = %d\n", i+1, i);
		}
	}

}

void eat(int i, t_ph *ph)
{
	if (i % 2 == 0) //feeding pair philos first
	{
		pthread_mutex_lock(&ph->fork);
		printf("fork %d was picked up---------- i = %d\n", i, i);
		if (i < ph->vals[0] - 2)
		{
			pthread_mutex_lock(&(ph+1)->fork);
			printf("fork %d was picked up----------i = %d\n", i+1, i);
		}
		if (i != ph->vals[0] - 1) 
		{
			printf("______________philosopher %d is eating..________________i = %d\n", i, i);
			sleep(1);
		}
		pthread_mutex_unlock(&ph->fork);
		printf("**************fork %d back to table i = %d\n", i, i);
		if (i < ph->vals[0] - 2)
		{
			pthread_mutex_unlock(&(ph+1)->fork);
			printf("***************fork %d is back to table i = %d\n", i+1, i);
		}
	}
	else
		impair(i, ph);

}

void	*routine(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	while (1)
	{
		eat(ph->i, ph);
//		think();
		sleep(2);
	}
	return ((void *)1);
}


void	philos_init(t_ph *ph)
{
	int	i;

	i = 0;
	while (i < ph->vals[0])
	{
		ph[i].i = i;
		i++;
	}
}

int	threads_creation(t_ph *ph)
{
	int		i;

	i = 0;
	while (i < ph->vals[0])
		pthread_mutex_init(&ph[i++].fork, NULL);
	philos_init(ph);
	i = -1;
	while (i++ < ph->vals[0] - 1) /*creating threads:*/
	{
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
	}
	i = 0;
	while (i < ph->vals[0])
	{
		if (pthread_join(ph[i++].th, NULL))
		{
			printf("Error while waiting for thread %d to terminate\n", i);
			return (0);
		}
	}
	return (1);
}

int	main(int ac, char **av)
{								//makefile wa9ila kay relinki!!
	// o khdem b calloc!
	t_ph	*ph;
	int		vals[5];
	int		i;

	i = 0;
	if (ac >= 5 && ac <= 6)
	{
		if (!check_args(av, ac, vals) || vals[0] == 0)
			return (1);
		ph = malloc(vals[0] * sizeof(t_ph));
		while (i < vals[0])
			ph[i++].vals = vals;
		if (!threads_creation(ph))
			return (1);
		//		pthread_mutex_destroy(gen.fork);
		return (0);
	}
	print_error(1);
	return (1); 
}
