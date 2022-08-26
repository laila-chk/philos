/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/26 13:15:56 by lchokri          ###   ########.fr       */
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

//philos who will eat: N+2 then N+1
//forks to lock:N+1
/*void	eat(t_gen *gen)
{
	int		i;

	i = 0;
//	while (i < gen->vals[0])
//	{
//		pthread_mutex_lock(&(gen->fork[i]));
//		printf (".. taking fork number %d\n", i);
//		i++;
//	/
	while (i < gen->vals[0] && gen->j % 2 == 0 && gen->vals[0] > 1)
	{
		pthread_mutex_lock(&(gen->fork[i]));
		pthread_mutex_lock(&(gen->fork[i + 1]));
		printf("%d < %d\n", gen->j, gen->vals[0]);
		printf (".. taking fork number %d and %d\n", gen->j, gen->j+1);
		printf("philosopher number %d is eating.......\n", gen->j);
		usleep(gen->vals[2]);
		gen->j+=2;
	}
	i = 0;
	while (i < gen->vals[0])
	{
		pthread_mutex_unlock(&gen->fork[i++]);
		printf("putting down fork number %d\n", i - 1);
	}
}

void	eat(t_gen gen)
{
	pthread_mutex_lock(&(gen->fork[]));
}
*/
void	*routine(void *arg)
{
	t_gen	*gen;

	gen = ((t_gen*)arg);
	printf("thread  number %d has entered\n", gen->j);
//	while (1)
//		eat(gen);
	return ((void *)1);
}

void	struct_init(t_gen *gen)
{
	int	i;

	i = 0;
	gen->dt = malloc(gen->vals[0] * sizeof(gen->dt));
	while (i < gen->vals[0])
	{
		gen->dt[i].i = i;// that's a fucking dumb ass idea
		gen->dt[i].t_die = gen->vals[1];
		gen->dt[i].t_sleep = gen->vals[3];
		gen->dt[i].meals = gen->vals[4];
		i++;
	}
}

int	threads_creation(t_gen *gen)
{
	int		i;

	i = 0;
	pthread_mutex_init(gen->fork, NULL);
	struct_init(gen);
	while (gen->vals[0] > gen->j)
	{
		if (pthread_create(&(gen->th[(gen->j)]), NULL, &routine, gen))
		{
			printf("problem occured while creating the %dth thread\n", gen->j);
			return (0);
		}
	printf("___%d\n", gen->j);
		(gen->j)++;
	}
	while (gen->vals[0] > i)
	{
		if (pthread_join((gen->th[i++]), NULL))
		{
			printf("Error while waiting for thread %d to terminate\n", i);
			return (0);
		}
	}
	return (1);
}

int	main(int ac, char **av)
{								//makefile wa9ila kay relinki!!
	t_gen		gen;

	gen.j = 0;
	if (ac >= 5 && ac <= 6)
	{
		if (!check_args(av, ac, &gen) || gen.vals[0] == 0)
			return (1);
		gen.th = malloc(gen.vals[0] * sizeof(pthread_t));
		gen.fork= malloc(gen.vals[0] * sizeof(pthread_mutex_t));
		if (!threads_creation(&gen))
			return (1);
		pthread_mutex_destroy(gen.fork);
		return (0);
	}
	print_error(1);
	return (1); 
}
