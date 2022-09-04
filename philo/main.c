/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/04 18:44:19 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
#include "pthread.h"
#include <sys/time.h> 

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
	while (i <= count - 2)
	{
		vals[i] = positive_atoi(av[i + 1]);
		i++;
	}
	i = 0;
	while (i <= count - 2)
	{
		if (vals[i] == -1)
			return (0);
		i++;
	}
	if (count == 5)
		vals[4] = -1;
	return (1);
}

int	alive(t_ph *ph, int i)
{
	if (ph->meals == 0)
	{
		printf("no more meals left for %d\n", i);
		return (0);
	}
	if (ph->start && (ph->last - ph->start) >= (long long)ph->vals[1])
	{
		printf("starving.. :last:%lld    start:%lld  vals:%d\n", ph->last, ph->start, ph->vals[1]);
		printf("diff: %lld => %d\n", ph->last - ph->start, ph->vals[1]);
		return (0);
	}
	return (1);
}

int	get_starting_time(t_ph *ph, int flg)
{
	struct	timeval time;

	if (gettimeofday(&time, NULL) == -1)
	{
		printf("Error occured while trying to get time of day!");
		return (0);
		//we still need to exit here!!
	}
	if (flg == 0)
		ph->start = time.tv_sec * 1000 + time.tv_usec / 1000;
	else if (flg == 1)
	{
		ph->start = ph->last;
		ph->last = time.tv_sec * 1000 + time.tv_usec / 1000;
	}
	return (1);
}

void eat(int i, t_ph *ph)
{
	if (alive(ph, i))
	{
		get_starting_time(ph, 1);
		pthread_mutex_lock(&ph->fork);
		printf("fork %d was picked up---------- i = %d\n", i, i);
		if (i <= ph->vals[0] - 2)
		{
			pthread_mutex_lock(&(ph+1)->fork);
			printf("fork %d was picked up----------i = %d\n", i+1, i);
		}
		else
		{
			pthread_mutex_lock(&(ph- (ph->vals[0] - 1))->fork);
			printf("~~~~~~fork 0 was picked up by %d----------\n", i);
		}
		printf("______________philosopher %d is eating..________________\n\n", i);
		usleep(ph->vals[2] * 1000);
		if (ph[i].meals > 0)
			ph[i].meals -= 1;
		pthread_mutex_unlock(&ph->fork);
		printf("**************fork %d back to table i = %d\n", i, i);
		if (i <= ph->vals[0] - 2)
		{
			pthread_mutex_unlock(&(ph+1)->fork);
			printf("***************fork %d is back to table i = %d\n", i+1, i);
		}
		else
		{
			pthread_mutex_unlock(&(ph - (ph->vals[0] - 1))->fork);
			printf("fork 0 is back to tablee----------\n");
		}
		printf("philo %d finished eating\n", i);
	}
}

void	think(t_ph *ph)
{
	if (alive(ph, ph->i))
	{
		printf("philosopher %d is thinking\n", ph->i);
		sleep(1);
	}
}

void	*routine(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	while (1)
	{
		eat(ph->i, ph);
		think(ph);
		usleep(ph->vals[3] * 1000);
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
		ph[i].meals = ph->vals[4];
		i++;
	}
}

int odd_threads_creation(t_ph *ph)
{
	int		i;

	i = 1;
	while (i < ph->vals[0]) /*creating odd threads:*/
	{
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
		if (!get_starting_time(&ph[i], 0))
			return (0);
		i += 2;
	}
	i = 0;
	while (i < ph->vals[0])
	{
		if (pthread_detach(ph[i++].th))
		{
			printf("Error while waiting for thread %d to terminate\n", i);
			return (0);
		}
	}
	return (1);
}
int	even_threads_creation(t_ph *ph)
{
	int		i;

	i = 0;
	while (i < ph->vals[0])
		pthread_mutex_init(&ph[i++].fork, NULL);
	philos_init(ph);
	i = 0;
	while (i < ph->vals[0]) /*creating even threads*/
	{
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
		if (!get_starting_time(&ph[i], 0))
			return (0);
		i += 2;
	}
	usleep(200);
	if (!odd_threads_creation(ph))
		return (0);
	return (1);
}

int	main(int ac, char **av)
{								//makefile wa9ila kay relinki!!
	// o khdem b calloc!
	t_ph		*ph;
	int			vals[5];
	int			i;

	i = 0;
	if (ac >= 5 && ac <= 6)
	{
		if (!check_args(av, ac, vals) || vals[0] == 0)
			return (1);
		ph = malloc(vals[0] * sizeof(t_ph));
		while (i < vals[0])
			ph[i++].vals = vals;
		if (!even_threads_creation(ph))
			return (1);
		while (1)
		{
			while (i < ph->vals[0])
			{
				if (alive(&ph[i], i) == 0)
				{
					//destroy_mutexes();
					   /*i decided to destroy bfr printing so annoncing the death will be as close to the end as
						*
						* much as possible*/
					printf("philosopher %d has died :'( \n", i);
					return (0);
				}
				i++;
			}
			i = 0;
		}
		sleep(20);
		return (0);
	}
	print_error(1);
	return (1); 
}
