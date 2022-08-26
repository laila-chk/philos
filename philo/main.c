/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchokri <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:26:32 by lchokri           #+#    #+#             */
/*   Updated: 2022/08/26 20:29:48 by lchokri          ###   ########.fr       */
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

int	check_args(char **av, int count,int *vals)
{
	int	i;

	i = 0;
	while (i < count - 2)
	{
		vals[i] = positive_atoi(av[i + 1]);
		i++;
	}
	i = 0;
	while (i < count - 2)
	{
		if (vals[i] == -1)
			return (0);
		i++;
	}
	if (count == 5)
		vals[4] = -1;
	return (1);
}
/*
   void	*routine(void *arg)
   {
   t_gen	*gen;

   gen = ((t_gen*)arg);
   printf("thread  number %d has entered\n", gen->j);
//	while (1)
//		eat(gen);
return ((void *)1);
}
*/

void	struct_init(t_ph *ph)
{
	int	i;

	i = 0;
	while (i < ph->vals[0])
	{
		ph[i].i = i;// that's a fucking dumb ass idea
		i++;
	}
}
/*
   int	threads_creation(t_gen *gen)
   {
   int		i;

   i = 0;
   pthread_mutex_init(gen->fork, NULL);
   struct_init(gen);
   while (i < gen->vals[0])
   {
   if (pthread_create(&(gen->th[i++]), NULL, &routine, gen))
   {
   printf("problem occured while creating the %dth thread\n", i);
   return (0);
   }
   }
   i = 0;
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
   */
int	main(int ac, char **av)
{								//makefile wa9ila kay relinki!!
	// o khdem b calloc!
	t_ph	*ph;
	int		vals[5];
	int		i;

	i = 0;
	if (ac >= 5 && ac <= 6)
	{
		ph = malloc(vals[0] * sizeof(t_ph));
		if (!check_args(av, ac, vals) || vals[0] == 0)
			return (1);
		while (i < vals[0])
			ph[i++].vals = vals;
//		if (!threads_creation(&gen))
//			return (1);
//		pthread_mutex_destroy(gen.fork);
		return (0);
	}
	print_error(1);
	return (1); 
}
