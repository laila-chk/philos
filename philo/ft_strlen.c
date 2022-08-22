/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 18:33:48 by lchokri           #+#    #+#             */
/*   Updated: 2021/11/27 03:36:07 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0' )
		i++;
	return (i);
}