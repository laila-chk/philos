/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 17:53:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/11 14:26:43 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_ph
{
	int					i;
	long long			start;
	long long			end;
	long long			last;
	long long			first;
	int					*vals;
	int					meals;
	pthread_mutex_t		fork;
	pthread_mutex_t		*prnt;
	pthread_mutex_t		mls_mtx;
	pthread_mutex_t		last_mtx;
	pthread_mutex_t		vals_mtx;
	pthread_t			th;
}t_ph;

int			positive_atoi(const char *str);
size_t		ft_strlen(const char *s);
int			print_stamp(t_ph *ph, int flag);
void		time_between_meals(t_ph *ph);
long long	update_time(void);
void		my_sleep(int duration);
int	check_args(char **av, int count, int *vals);
#endif
