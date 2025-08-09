/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 22:48:36 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/09 15:37:00 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_pthread/_pthread_t.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_philo
{
	bool			dead;
	int				max_eat;
	int				id;
	int				meals_eaten;
	long long		time_since_eat;
	pthread_t		thread;
	struct s_table	*table;
}					t_philo;

typedef struct s_table
{
	bool			is_limited;
	bool			shall_die;
	int				philo_count;
	int				die_time;
	int				eat_time;
	int				sleep_time;
	long long		start_time;
	t_philo			*philo;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
}					t_table;

long long	get_cur_time(void)
{
	struct timeval	tv;
	long long		time;

	time = 0;
	gettimeofday(&tv, NULL);
	time = tv.tv_sec * 1000;
	time += tv.tv_usec / 1000;
	return (time);
}
void	sleep_n_ms(long long ms)
{
	long long	start_time_local;

	start_time_local = get_cur_time();
	while ((get_cur_time() - start_time_local) < ms)
		usleep(100);
}

void	print_logs(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->write_lock);
	printf("%lld %d %s\n", get_cur_time() - philo->table->start_time, philo->id,
		msg);
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	eat_odd(t_philo *philo, int fork_right)
{
	pthread_mutex_lock(&philo->table->forks[fork_right]);
	print_logs(philo, "has taken a fork");
	pthread_mutex_lock(&philo->table->forks[fork_right - 1]);
	print_logs(philo, "has taken a fork");
	print_logs(philo, "is eating");
	philo->time_since_eat = get_cur_time();
	sleep_n_ms(philo->table->eat_time);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->forks[fork_right - 1]);
	pthread_mutex_unlock(&philo->table->forks[fork_right]);
}

void	eat_even(t_philo *philo, int fork_right)
{
	pthread_mutex_lock(&philo->table->forks[fork_right - 1]);
	print_logs(philo, "has taken a fork");
	pthread_mutex_lock(&philo->table->forks[fork_right]);
	print_logs(philo, "has taken a fork");
	print_logs(philo, "is eating");
	philo->time_since_eat = get_cur_time();
	sleep_n_ms(philo->table->eat_time);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->forks[fork_right]);
	pthread_mutex_unlock(&philo->table->forks[fork_right - 1]);
}

void	eat_stuff(t_philo *philo)
{
	int	fork_right;

	fork_right = philo->id % philo->table->philo_count;
	if (philo->table->philo_count == 1)
	{
		pthread_mutex_lock(&philo->table->forks[0]);
		print_logs(philo, "has taken a fork");
		while (1)
		{
			pthread_mutex_lock(&philo->table->death_lock);
			if (philo->table->shall_die)
			{
				pthread_mutex_unlock(&philo->table->death_lock);
				break ;
			}
			pthread_mutex_unlock(&philo->table->death_lock);
			sleep_n_ms(10);
		}
		pthread_mutex_unlock(&philo->table->forks[0]);
		return ;
	}
	if (philo->id % 2)
		eat_odd(philo, fork_right);
	else
		eat_even(philo, fork_right);
}

void	sleep_think(t_philo *philo)
{
	print_logs(philo, "is sleeping");
	sleep_n_ms(philo->table->sleep_time);
	print_logs(philo, "is thinking");
}

void	*philo_code(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)arg;
	table = philo->table;
	if (philo->id % 2)
		sleep_n_ms(table->eat_time / 2);
	while (1)
	{
		pthread_mutex_lock(&table->death_lock);
		if (table->shall_die)
		{
			pthread_mutex_unlock(&table->death_lock);
			break ;
		}
		pthread_mutex_unlock(&table->death_lock);
		eat_stuff(philo);
		sleep_think(philo);
	}
	return (NULL);
}

bool	did_not_eat(t_table *table, int i)
{
	long long	start_time_local;
	bool		should_die;

	should_die = 0;
	start_time_local = get_cur_time();
	pthread_mutex_lock(&table->death_lock);
	if (start_time_local - table->philo[i].time_since_eat > table->die_time)
	{
		table->shall_die = 1;
		should_die = 1;
	}
	pthread_mutex_unlock(&table->death_lock);
	if (should_die)
	{
		print_logs(&table->philo[i], "died");
		return (1);
	}
	return (0);
}

void	silent_kill_all(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		table->philo[i].dead = 1;
		i++;
	}
}

bool	check_eat_amount(t_table *table)
{
	int	i;
	int	full_count;

	i = 0;
	full_count = 0;
	if (!table->is_limited)
		return (false);
	while (i < table->philo_count)
	{
		if (table->philo[i].meals_eaten >= table->philo[i].max_eat)
			full_count++;
		i++;
	}
	if (full_count == table->philo_count)
	{
		pthread_mutex_lock(&table->death_lock);
		silent_kill_all(table);
		pthread_mutex_unlock(&table->death_lock);
		return (1);
	}
	return (0);
}

void	*monitor_code(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (1)
	{
		i = 0;
		while (i < table->philo_count)
		{
			if (did_not_eat(table, i) || check_eat_amount(table))
				return (NULL);
			i++;
		}
		sleep_n_ms(1);
	}
}
int	ft_atoi(char *s)
{
	int			i;
	long int	ret;

	i = 0;
	ret = 0;
	while (s[i])
	{
		ret *= 10;
		ret += s[i] - '0';
		i++;
	}
	if (ret > 2147483647)
		return (-1);
	return ((int)ret);
}

void	call_error(int err)
{
	if (err == 1)
		printf("use with proper input\n");
	else if (err == 2)
		printf("init fail\n");
}

bool	atoi_fail(t_table *table)
{
	if (table->philo_count == -1 || table->philo_count > 200)
		return (1);
	if (table->die_time == -1)
		return (1);
	if (table->eat_time == -1)
		return (1);
	if (table->sleep_time == -1)
		return (1);
	return (0);
}

void	fill_philo(t_philo *philo, int id, char *av, int ac)
{
	int	max_eat;

	max_eat = -1;
	philo->id = id;
	philo->dead = false;
	philo->meals_eaten = 0;
	if (ac == 6)
		max_eat = ft_atoi(&av[5]);
	if (max_eat != -1)
		philo->max_eat = max_eat;
	else
		philo->max_eat = -1;
}

void	add_table_pointer(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		table->philo[i].table = table;
		table->philo[i].time_since_eat = table->start_time;
		i++;
	}
}

bool	init_table(char **av, int ac, t_table *table)
{
	int	i;

	i = 0;
	if (ac == 5)
		table->is_limited = 0;
	else
		table->is_limited = 1;
	table->shall_die = 0;
	table->philo_count = ft_atoi(av[1]);
	table->die_time = ft_atoi(av[2]);
	table->eat_time = ft_atoi(av[3]);
	table->sleep_time = ft_atoi(av[4]);
	table->start_time = get_cur_time();
	if (atoi_fail(table))
		return (1);
	table->philo = malloc(table->philo_count * sizeof(t_philo));
	if (!table->philo)
		return (1);
	while (i < table->philo_count)
	{
		fill_philo(&table->philo[i], i + 1, *av, ac);
		i++;
	}
	add_table_pointer(table);
	return (0);
}

bool	is_only_num(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '\0')
		return (0);
	while (arg[i])
	{
		if (arg[i] < '0' || arg[i] > '9' || i >= 10)
			return (0);
		i++;
	}
	return (1);
}

bool	input_fail(int ac, char **av)
{
	int	i;

	i = 1;
	while (i < ac)
	{
		if (!is_only_num(av[i]))
			return (1);
		i++;
	}
	return (0);
}
bool	init_mutex(t_table *table)
{
	int	i;

	table->forks = malloc(table->philo_count * sizeof(pthread_mutex_t));
	if (!table->forks)
		return (true);
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (true);
		i++;
	}
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
		return (true);
	if (pthread_mutex_init(&table->death_lock, NULL) != 0)
		return (true);
	return (false);
}
int	main(int ac, char **av)
{
	t_table	table;
	int		i;

	i = 0;
	memset(&table, 0, sizeof(t_table));
	if ((ac == 5 || ac == 6) && !input_fail(ac, av))
	{
		if (init_table(av, ac, &table))
			return (call_error(2), 1);
		if (init_mutex(&table))
		{
			free(table.philo);
			return (call_error(2), 1);
		}
		i = -1;
		while (++i < table.philo_count)
			pthread_create(&table.philo[i].thread, NULL, philo_code,
				&table.philo[i]);
		pthread_create(&table.monitor, NULL, monitor_code, &table);
		i = -1;
		while (++i < table.philo_count)
			pthread_join(table.philo[i].thread, NULL);
		pthread_join(table.monitor, NULL);
		i = -1;
		while (++i < table.philo_count)
			pthread_mutex_destroy(&table.forks[i]);
		pthread_mutex_destroy(&table.write_lock);
		pthread_mutex_destroy(&table.death_lock);
		free(table.philo);
		free(table.forks);
	}
	else
		return (call_error(1), 1);
	return (0);
}
