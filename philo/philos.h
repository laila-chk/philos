/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*         :q                                       +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 17:53:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/03 18:08:39 by lchokri          ###   ########.fr       */
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

typedef	struct	s_ph
{
	int					i;
	long long			start;
	long long			last;
	int					*vals;
	int					meals;
	pthread_mutex_t		fork;
	pthread_mutex_t		vals_mtx;
	pthread_t			th;
}	t_ph;

#endif
