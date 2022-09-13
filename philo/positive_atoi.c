/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   positive_atoi.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 16:16:13 by lchokri           #+#    #+#             */
/*   Updated: 2022/09/02 13:55:03 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	positive_atoi(const char *str)
{
	int				i;
	unsigned long	res;

	i = 0;
	res = -1;
	while ((str[i] <= 13 && str[i] >= 9) || str[i] == 32)
		i++;
	if (str[i] == '-')
	{
//		write(2, "Error! all values should be positive\n", 38);
		return (-1);
	}
	if (str[i] == '+')
		i++;
	if (str[i] <= '9' && str[i] >= '0')
		res = 0;
	while (str[i] <= '9' && str[i] >= '0')
		res = res * 10 + str[i++] - '0';
	if (str[i] && !(str[i] <= '9' && str[i] >= '0'))
	{
//		write(2, "Error! ", 7);
//		write(2, str, ft_strlen(str));
//		write(2, " should be a positive number\n", 30);
		return (-1);
	}
	return (res);
}

int	check_args(char **av, int count, int *vals)
{
	int	i;

	i = 0;
	while (i <= count - 2)
	{
		// if (av[i + 1][0] == '\0')
		// {
		// 	write(2, "Error: empty argument\n", 22)
		// 	return (0);
		// }
		vals[i] = positive_atoi(av[i + 1]);
		i++;
	}
	i = 0;
	while (i <= count - 2)
	{
		if (vals[i] == -1)
		{
			write(2, "Error: all values should be positive numbers\n", 45);
			return (0);
		}
		i++;
	}
	if (count == 5)
		vals[4] = -1;
	vals[5] = 0;
	return (1);
}
