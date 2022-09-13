/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/11 14:42:35 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/11 14:42:37 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	print_stamp(t_ph *ph, int flag)
{
	int				i;
	struct timeval	tm;
	int				time;

	i = 0;
	time = 0;
	gettimeofday(&tm, NULL);
	if (flag == 0)
	{
		while (i < ph->vals[0])
		{
			(ph + i)->start = tm.tv_sec * 1000 + tm.tv_usec / 1000;
			i++;
		}
	}
	else if (flag == 1)
	{
		time = (tm.tv_sec * 1000 + tm.tv_usec / 1000) - ph->start;
	}
	return (time);
}

long long	update_time(void)
{
	struct timeval	tv;
	long long		time;

	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (time);
}

void	time_between_meals(t_ph *ph)
{
	pthread_mutex_lock(ph->last_mtx);
	ph->last = update_time(); //here :last
	pthread_mutex_unlock(ph->last_mtx);
	return ((void)1);
}

void	my_sleep(int duration)
{
	long long	strt;
	long long	curr;

	strt = update_time();
	curr = update_time();
	usleep(duration * 900);
	while (duration + strt > curr)
	{
		usleep(50);
		curr = update_time();
	}
}
