/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 17:53:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/26 13:16:16 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

int		positive_atoi(const char *str);
size_t	ft_strlen(const char *s);

typedef	struct s_dt
{
	int		i;
	int		t_die;
	int		t_sleep;
	int		meals;
}t_dt;

typedef struct s_gen
{
	int		j;
	int		tot;
	int		vals[5];
	pthread_mutex_t	*fork;
	pthread_t		*th;
	t_dt			*dt;

}t_gen;


typedef struct s_shared
{
	int			i;
	int			tot;
	int			t_die;
	int			t_eat;
	int			t_sleep;
	int			meals;
	int			start;

}	t_gen;

typedef	struct	s_data;
{
	t_gen		gen;
	pthread_t	*th;
	int			n;
}	t_dt;

typedef	struct	*ph
{
	int				i;
	int				start;
	int				vals[5];
	pthread_mutex_t	*fork;
	pthread_t		*th;

}	t_ph;

#endif
