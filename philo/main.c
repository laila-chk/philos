/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/12 16:12:45 by lchokri          ###   ########.fr       */
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

int	alive(t_ph *ph, int *j)
{
	int			i;
	long long	curr;	

	i = 0;
	while (*j < ph->vals[0])
	{
		curr = update_time();	
		pthread_mutex_lock(ph->mls_mtx);
		if (ph[*j].vals[4] != -1 && ph[*j].meals >= ph[*j].vals[4])
			i++;
		pthread_mutex_unlock(ph->mls_mtx);
		pthread_mutex_lock(ph->last_mtx);
	//	printf("%p{{{{{{}}}}}}}%d\n", &ph[*j].last, *j);
		if (curr - ph[*j].last >= ph->vals[1])
		{
			pthread_mutex_unlock(ph->last_mtx);
			return (0);
		}
		pthread_mutex_unlock(ph->last_mtx);
		(*j)++;
	}
	(*j)--;
	pthread_mutex_lock(ph->mls_mtx);
	if (ph[*j].meals != -1 && i == ph->vals[0])
	{
		*j = -1;
		pthread_mutex_unlock(ph->mls_mtx);
		return (0);
	}
	pthread_mutex_unlock(ph->mls_mtx);
	return (1);
}

void	lock_print(char *str, t_ph *ph, int i)
{
	pthread_mutex_lock(ph->prnt);
	printf("%d   %d %s\n", print_stamp(ph, 1), i + 1, str);
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
		////pthread_mutex_init(&ph[i].last_mtx, NULL);
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

int	first_init(t_ph *ph, pthread_mutex_t *prnt, pthread_mutex_t *mls, pthread_mutex_t *lst)
{
	int	i;

	i = 0;
	if (!prnt || !mls || ph->vals[4] == 0)
		return (0);
	pthread_mutex_init(prnt, NULL);
	pthread_mutex_init(mls, NULL);
	pthread_mutex_init(lst, NULL);
	while (i < ph->vals[0])
	{
        ph[i].last_mtx = lst;
		ph[i].prnt = prnt;
		ph[i].mls_mtx = mls;
		i++;
	}
	print_stamp(ph, 0);
	return (1);
}

int	stimulation_ended(t_ph *ph)
{
	int	j;

	while (1)
	{
		j = 0;
		if (!alive(ph, &j))
		{
			pthread_mutex_lock(ph->prnt);
			if (j != -1)
				printf("%d   %d died\n", print_stamp(ph, 1), j + 1);
			return (1);
		}
	}
}

int	main(int ac, char **av)
{	
	t_ph			*ph;
	pthread_mutex_t	*prnt;
	pthread_mutex_t	*lst;
	pthread_mutex_t	*mls_mtx;
	int				*vals;

	prnt = malloc(sizeof(pthread_mutex_t));
	lst = malloc(sizeof(pthread_mutex_t));
	mls_mtx = malloc(sizeof(pthread_mutex_t));
	if (ac >= 5 && ac <= 6)
	{
		vals = malloc(6 * sizeof(int));
		if (!vals || !check_args(av, ac, vals) || vals[0] == 0)
			return (1);
		ph = malloc(vals[0] * sizeof(t_ph));
		if (!ph)
			return (1);
		while (vals[5] < vals[0])
			ph[vals[5]++].vals = vals;
		if (!first_init(ph, prnt, mls_mtx, lst) || !even_threads_creation(ph))
			return (1);
		if (stimulation_ended(ph))
			return (0);
	}
	print_error(1);
	return (1);
}
