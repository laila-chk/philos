/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 03:46:38 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/13 03:47:03 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

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
	pthread_mutex_lock(ph->mls_mtx);
	if (ph->meals >= 0)
		ph->meals += 1;
	pthread_mutex_unlock(ph->mls_mtx);
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
		i++;
	}
}

int	odd_threads_creation(t_ph *ph)
{
	int		i;

	i = 1;
	while (i < ph->vals[0])
	{
		time_between_meals(&ph[i]);
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
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
		time_between_meals(&ph[i]);
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
		i += 2;
	}
	usleep(100);
	if (!odd_threads_creation(ph))
		return (0);
	return (1);
}
