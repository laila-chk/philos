#include "philos.h"
#include "pthread.h"
#include <sys/time.h> 

void	print_error(int i)
{
	if (i == 1)
	{
		write(2, "Error!\nUsage:./philo number_of_philosophers ", 44);
		write(2, "time_to_die time_to_eat time_to_sleep ", 38);
		write(2, "[number_of_times_each_philosopher_must_eat]\n", 44);
	}
}

int	check_args(char **av, int count,int *vals)
{
	int	i;

	i = 0;
	while (i <= count - 2)
	{
		vals[i] = positive_atoi(av[i + 1]);
		i++;
	}
	i = 0;
	while (i <= count - 2)
	{
		if (vals[i] == -1)
			return (0);
		i++;
	}
	if (count == 5)
		vals[4] = -1;
	vals[5] = 1;
	return (1);
}


int	print_stamp(t_ph *ph, int flag)
{
	int		i;
	struct timeval tm;
	int		time;

	i = 0;
	time = 0;
	gettimeofday(&tm, NULL);
	if (flag == 0) 
	{
		while (i < ph->vals[0])
		{
			(ph+i)->start = tm.tv_sec * 1000 + tm.tv_usec / 1000;
			i++;
		}
	}
	else if (flag == 1)
	{
		time = (tm.tv_sec * 1000 + tm.tv_usec / 1000) - ph->start;
	}
	return (time);
}

void	time_between_meals(t_ph *ph)
{
	struct	timeval tv;

	gettimeofday(&tv, NULL);
	pthread_mutex_lock(&ph->last_mtx);
	ph->last = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	pthread_mutex_unlock(&ph->last_mtx);
	return ((void)1);
}

int	alive(t_ph *ph)
{
	int		j;
	int		i;
	struct	timeval	tvl;
	long long	curr;	

	j = 0;
	i = 0;
	gettimeofday(&tvl, NULL);
	while (j < ph->vals[0])
	{
		curr = tvl.tv_sec * 1000 + tvl.tv_usec / 1000;
		if (ph[j].meals != -1 && ph[j].meals >= ph[j].vals[4])
			i++;
		if (curr - ph[j].last >= ph->vals[1])
		{
			printf("death of %d cus of starvation: ",j );
			printf("%lld milliseconds of hunger\n", curr - ph[j].last);
			return (0);
		}
		j++;
	}
	if (i == ph->vals[0])
		return (0);
	return(1);
}

void	lock_print(char *str, t_ph *ph, int i)
{
	pthread_mutex_lock(ph->prnt);
	printf("%d   %d %s\n", print_stamp(ph, 1), i, str);
	pthread_mutex_unlock(ph->prnt);
}

void eat(int i, t_ph *ph)
{
	pthread_mutex_lock(&ph->fork);
	lock_print("has taken a fork", ph, i);
	if (i <= ph->vals[0] - 2)
		pthread_mutex_lock(&(ph+1)->fork);
	else
		pthread_mutex_lock(&(ph- (ph->vals[0] - 1))->fork);
	lock_print("has taken a fork", ph, i);
	lock_print("is eating", ph, i);

	time_between_meals(ph);
	usleep(ph->vals[2] * 1000);
	pthread_mutex_lock(&ph->mls_mtx);
	if (ph->meals >= 0)
		ph->meals += 1;
	pthread_mutex_unlock(&ph->mls_mtx);
	pthread_mutex_unlock(&ph->fork);
	if (i <= ph->vals[0] - 2)
		pthread_mutex_unlock(&(ph+1)->fork);
	else
		pthread_mutex_unlock(&(ph - (ph->vals[0] - 1))->fork);
}

void	think_and_sleep(t_ph *ph, int i)
{
	struct	timeval tv;
	long	strt;

	gettimeofday(&tv, NULL);
	strt = tv.tv_sec * 1000000 + tv.tv_usec; //kolchi b microsec;
	lock_print("is sleeping-----------", ph, i);
	usleep(ph->vals[3] * 900);
	while (ph->vals[3] * 1000 + strt < tv.tv_sec * 1000000 + tv.tv_usec)
		usleep(10);
	lock_print("is thinking", ph, i);
}

void	*routine(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	while (1)
	{
		eat(ph->i, ph);
		think_and_sleep(ph, ph->i);
	}
	return ((void *)1);
}

void	philos_init(t_ph *ph)
{
	int	i;

	i = 0;
	while (i < ph->vals[0])
	{
		ph[i].i = i;
		ph[i].meals = 0;
		pthread_mutex_init(&ph[i].last_mtx, NULL);
		i++;
	}
}

int odd_threads_creation(t_ph *ph)
{
	int		i;

	i = 1;
	while (i < ph->vals[0]) /*creating odd threads:*/
	{
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
		time_between_meals(&ph[i]);
		i += 2;
	}
	i = 0;
	while (i < ph->vals[0])
	{
		if (pthread_detach(ph[i++].th))
		{
			printf("Error while waiting for thread %d to terminate\n", i);
			return (0);
		}
	}
	return (1);
}
int	even_threads_creation(t_ph *ph)
{
	int		i;

	i = 0;
	while (i < ph->vals[0])
		pthread_mutex_init(&ph[i++].fork, NULL);
	philos_init(ph);
	i = 0;
	while (i < ph->vals[0]) /*creating even threads*/
	{
		if (pthread_create(&ph[i].th, NULL, &routine, &ph[i]))
		{
			printf("problem occured while creating the %dth thread\n", i);
			return (0);
		}
		time_between_meals(&ph[i]);
		i += 2;
	}
	usleep(500);
	if (!odd_threads_creation(ph))
		return (0);
	return (1);
}


int	main(int ac, char **av)
{								//makefile wa9ila kay relinki!!
	// o khdem b calloc!
	t_ph		*ph;
	pthread_mutex_t		*prnt;
	int			*vals;
	int			i;

	i = 0;
	prnt = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(prnt, NULL);
	if (ac >= 5 && ac <= 6)
	{
		vals = malloc(6 * sizeof(int));
		if (!check_args(av, ac, vals) || vals[0] == 0)
			return (1);
		ph = malloc(vals[0] * sizeof(t_ph));
		while (i < vals[0])
			ph[i++].vals = vals;
		i = 0;
		while (i < vals[0])
			ph[i++].prnt = prnt;
		print_stamp(ph, 0);
		if (!even_threads_creation(ph))
			return (1);
		while (1)
		{
			if (!alive(ph))
			{
				//destroy_mutexes();
				pthread_mutex_lock(ph->prnt);
				return (0);
			}
		}
	}
	print_error(1);
	return (1);
}

