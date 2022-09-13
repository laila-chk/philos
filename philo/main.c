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

int	all_meals(t_ph *ph, int *j, int i)
{
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
		if (curr - ph[*j].last >= ph->vals[1])
		{
			pthread_mutex_unlock(ph->last_mtx);
			return (0);
		}
		pthread_mutex_unlock(ph->last_mtx);
		(*j)++;
	}
	if (!all_meals(ph, j, i))
		return (0);
	return (1);
}

int	first_init(t_ph *ph, pthread_mutex_t *prnt, \
	pthread_mutex_t *mls, pthread_mutex_t *lst)
{
	int	i;

	i = 0;
	if (!prnt || !mls || !lst || ph->vals[4] == 0)
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
	if (ac < 5 || ac > 6)
		return (print_error(1));
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
