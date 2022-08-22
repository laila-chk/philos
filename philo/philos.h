/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 17:53:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/22 17:54:58 by lchokri          ###   ########.fr       */
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

typedef struct s_multi
{
	pthread_mutex_t	mtx;
}t_multi;
#endif
