/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/17 20:17:57 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int		check_args(char **av, int count, int *philo)
{
	int	i;

	i = -1;
	while (i++ < count - 2)
	{
		philo[i] = positive_atoi(av[i + 1]);
	}
	while (i > -1)
	{
		if (philo[i--] == -1)
			return (0);
	}
	return (1);
}

int main(int ac, char **av)
{
	int		philo[5];

	if (ac >= 5 && ac <= 6)
	{
		if(!check_args(av, ac, philo))
			return (0);
		while (1);
	}
	else
		write(2, "Error!\nUsage:./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]", 126);
}
