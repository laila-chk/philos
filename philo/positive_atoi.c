/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   positive_atoi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 16:16:13 by lchokri           #+#    #+#             */
/*   Updated: 2022/07/05 16:16:47 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	positive_atoi(const char *str)
{
	int				i;
	long			sign;
	unsigned long	res;

	i = 0;
	sign = 1;
	res = 0;
	while ((str[i] <= 13 && str[i] >= 9) || str[i] == 32)
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
		{
			write(2, "Error! this value can't be negative\n", 36);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	while (str[i] <= '9' && str[i] >= '0')
	{
		if (!(str[i] <= '9' && str[i] >= '0'))
		{
			write(2, "Error! this value should contain positibnumbers only\n", 53);
			exit(EXIT_FAILURE);
		}
		res = res * 10 + str[i] - '0';
		i++;
	}
	return (sign * res);
}
