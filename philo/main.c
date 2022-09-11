/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/11 14:41:23 by lchokri          ###   ########.fr       */
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

int	alive(t_ph *ph)
{
	int			j;
	int			i;
	long long	curr;	

	j = 0;
	i = 0;
	while (j < ph->vals[0])
	{
		curr = update_time();
		if (ph[j].meals != -1 && ph[j].meals >= ph[j].vals[4])
			i++;
		if (curr - ph[j].last >= ph->vals[1])
			return (0);
		j++;
	}
	if (i == ph->vals[0])
		return (0);
	return (1);
}

void	lock_print(char *str, t_ph *ph, int i)
{
	pthread_mutex_lock(ph->prnt);
	printf("%d   %d %s\n", print_stamp(ph, 1), i, str);
	pthread_mutex_unlock(ph->prnt);
}

void	eat(int i, t_ph *ph)
{
	pthread_mutex_lock(&ph->fork);
	lock_print("has taken a fork", ph, i);
	if (i <= ph->vals[0] - 2)
		pthread_mutex_lock(&(ph + 1)->fork);
	else
		pthread_mutex_lock(&(ph - (ph->vals[0] - 1))->fork);
	lock_print("has taken a fork", ph, i);
	lock_print("is eating", ph, i);
	time_between_meals(ph);
	my_sleep(ph->vals[2]);
	pthread_mutex_lock(&ph->mls_mtx);
	if (ph->meals >= 0)
		ph->meals += 1;
	pthread_mutex_unlock(&ph->mls_mtx);
	pthread_mutex_unlock(&ph->fork);
	if (i <= ph->vals[0] - 2)
		pthread_mutex_unlock(&(ph + 1)->fork);
	else
		pthread_mutex_unlock(&(ph - (ph->vals[0] - 1))->fork);
}

void	*routine(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	while (1)
	{
		eat(ph->i, ph);
		lock_print("is sleeping", ph, ph->i);
		my_sleep(ph->vals[3]);
		lock_print("is thinking", ph, ph->i);
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
		ph[i].meals = 0;
		pthread_mutex_init(&ph[i].last_mtx, NULL);
		i++;
	}
}

int	odd_threads_creation(t_ph *ph)
{
	int		i;

	i = 1;
	while (i < ph->vals[0])
	{
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
		time_between_meals(&ph[i]);
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
	while (i < ph->vals[0])
	{
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
		time_between_meals(&ph[i]);
		i += 2;
	}
	usleep(100);
	if (!odd_threads_creation(ph))
		return (0);
	return (1);
}
//ba9i mochkil flowel
//makefile wa9ila kay relinki!! o khdem b calloc!
//check namings of functions and folders
int	main(int ac, char **av)
{	
	t_ph			*ph;
	pthread_mutex_t	*prnt;
	int				*vals;
	int				i;

	i = 0;
	prnt = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(prnt, NULL);
	if (ac >= 5 && ac <= 6)
	{
		vals = malloc(5 * sizeof(int));
		if (!check_args(av, ac, vals) || vals[0] == 0)
			return (1);
		ph = malloc(vals[0] * sizeof(t_ph));
		while (i < vals[0])
			ph[i++].vals = vals;
		i = 0;
		while (i < vals[0])
			ph[i++].prnt = prnt;
		print_stamp(ph, 0);
		if (!even_threads_creation(ph))
			return (1);
		while (1)
		{
			if (!alive(ph))
			{
				//destroy_mutexes();
				pthread_mutex_lock(ph->prnt);
				return (0);
			}
		}
	}
	print_error(1);
	return (1);
}
