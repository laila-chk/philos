/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/05 20:16:16 by lchokri          ###   ########.fr       */
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
	int		j;

	j = 0;
	(void)i;
	while (j < ph->vals[0])
	{
		if ((ph+j)->meals != 0)
			break;
		j++;
	}
	if (j == ph->vals[0])
		return (0);
	if (ph->first && (ph->last - ph->first) >= (long long)ph->vals[1])
	{
		printf("starving.. :last:%lld    start:%lld  vals:%d\n", ph->last, ph->first, ph->vals[1]);
		printf("diff: %lld => %d\n", ph->last - ph->first, ph->vals[1]);
		return (0);
	}
	return (1);
}

int	print_stamp(t_ph *ph, int flg)
{
	int	i;
	struct	timeval	time;

	i = 0;
	gettimeofday(&time, NULL);
	if (flg == 0)
	{
		while (i < ph->vals[0])
			(ph + i++)->start = time.tv_sec * 1000 + time.tv_usec / 1000;
	}
	if (flg == 1)
		ph->end = (time.tv_sec * 1000 + time.tv_usec / 1000);
	return (ph->end - ph->start);
}

int	get_starting_time(t_ph *ph, int flg)
{
	struct	timeval tm;

	if (gettimeofday(&tm, NULL) == -1)
	{
		printf("Error occured while trying to get time of day!");
		return (0);
		//we still need to exit here!!
	}
	if (flg == 0)
		ph->first = tm.tv_sec * 1000 + tm.tv_usec / 1000;
	else if (flg == 1)
	{
		ph->first = ph->last;
		ph->last = tm.tv_sec * 1000 + tm.tv_usec / 1000;
	}
	printf("flag:%d   first:%lld last:%lld start:%lld\n", flg, ph->first, ph->last, ph->start);
	return (1);
}

void eat(int i, t_ph *ph)
{
	if (alive(ph, i))
	{
		get_starting_time(ph, 1);
		pthread_mutex_lock(&ph->fork);
		printf("%d   %d has taken a fork\n", print_stamp(ph, 1), i);
		if (i <= ph->vals[0] - 2)
		{
			pthread_mutex_lock(&(ph+1)->fork);
			printf("%d   %d has taken a fork\n", print_stamp(ph, 1), i);
		}
		else
		{
			pthread_mutex_lock(&(ph- (ph->vals[0] - 1))->fork);
			printf("%d   %d has taken a fork\n", print_stamp(ph, 1), i);
		}
		printf("%d   philosopher %d is eating.._____________\n", print_stamp(ph, 1), i);
		usleep(ph->vals[2] * 1000);
		if (ph[i].meals > 0)
			ph[i].meals -= 1;
		pthread_mutex_unlock(&ph->fork);
		if (i <= ph->vals[0] - 2)
			pthread_mutex_unlock(&(ph+1)->fork);
		else
			pthread_mutex_unlock(&(ph - (ph->vals[0] - 1))->fork);
	}
}

void	think_and_sleep(t_ph *ph)
{
	if (alive(ph, ph->i))
		{
			printf("%d    %d is thinking\n", print_stamp(ph, 1), ph->i);
			printf("%d    %d is sleeping\n\n", print_stamp(ph, 1), ph->i);
			//get enough sleep 
		}
}

void	*routine(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	while (1)
	{
		eat(ph->i, ph);
		think_and_sleep(ph);
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
	usleep(500);
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
		print_stamp(&ph[0], 0);
		if (!even_threads_creation(ph))
			return (1);
		i = 0;
		while (1)
		{
				if (alive(ph, 0) == 0)
				{
					//destroy_mutexes();
					printf("philosopher %d has died :'( \n", i);
				//	return (0);
				break;
				}
		}
		return (0);
	}
	print_error(1);
	return (1); 
}
