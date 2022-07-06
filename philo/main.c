/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/07/05 18:14:38 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	check_args(char **av, int count, t_philos *ph_data)
{
	ph_data->n_of_philos = positive_atoi(av[1]);
	ph_data->time_to_die = positive_atoi(av[2]);
	ph_data->time_to_eat = positive_atoi(av[3]);
	ph_data->time_to_sleep= positive_atoi(av[4]);
	if (count == 6)
		ph_data->eating_times = positive_atoi(av[5]);
}

int main(int ac, char **av)
{
	t_philos ph_data;	

	if (ac >= 5 && ac <= 6)
	{
		check_args(av, ac, &ph_data);
	}
	else
		write(2, "Error!\nUsage:./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]", 126);
}
