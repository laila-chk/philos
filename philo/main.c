/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/24 04:51:56 by lchokri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	print_error(int i)
{
	if (i == 1)
	{
		write(2, "Error!\nUsage:./philo number_of_philosophers", 43);
		write(2, "time_to_die time_to_eat time_to_sleep", 37);
		write(2, "[number_of_times_each_philosopher_must_eat]\n", 44);
	}
}

int	check_args(char **av, int count, t_gen *gen)
{
	int	i;

	i = -1;
	while (i++ < count - 2)
	{
		gen->vals[i] = positive_atoi(av[i + 1]);
	}
	while (i > -1)
	{
		if (gen->vals[i--] == -1)
			return (0);
	}
	if (count == 5)
		gen->vals[4] = -1;
	return (1);
}

void	eat(t_gen *gen)
{
	int		i;

	i = 0;
	while (i < gen->vals[0])
	{
		//pthread_mutex_lock();
			i+=2;
	}
}

void	*routine(void *arg)
{
	t_gen	*gen;

	gen = ((t_gen*)arg);
	printf("pretending to eat\n");
	eat(gen);
	return ((void *)1);
}

int	threads_creation(t_gen *gen)
{
	int		i;

	i = 0;
	while (gen->vals[0]> i)
	{
		if (pthread_create(&(gen->th[i]), NULL, &routine, gen))
		{
			printf("problem occured while creating the %dth thread\n", i + 1);
			return (0);
		}
		i++;
	}
	i = 0;
	while (gen->vals[0]> i)
	{
		if (pthread_detach((gen->th[i])))//pthread_detach is better
		{
			printf("Error while waiting for thread %d to terminate\n", i + 1);
			return (0);
		}
		i++;
	}
	return (1);
}

int	main(int ac, char **av)
{								//makefile wa9ila kay relinki!!
	t_gen		gen;

	if (ac >= 5 && ac <= 6)
	{
		if (!check_args(av, ac, &gen) || gen.vals[0] == 0)
			return (1);
		gen.th = malloc(gen.vals[0] * sizeof(pthread_t));
		gen.mtx= malloc(gen.vals[0] * sizeof(pthread_mutex_t));
		if (!threads_creation(&gen))
			return (1);
		//watcher thread here!
		return (0);//kolchi smooth --> ret 1
	}
	print_error(1);//khrejna mn if or madkholnalachii asln ==> kayn chi mochkil
	return (1); 
}
