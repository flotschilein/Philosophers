/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 22:48:36 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/06 21:48:34 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	t_table			*table;
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

long long	get_cur_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
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
		max_eat = ft_atoi(av[5]);
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
		fill_philo(&table->philo[i], i + 1, av, ac);
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
		i = -1;
		while (++i < table.philo_count)
			pthread_create(&table.philo[i].thread, NULL, philo_code,
				&table.philo[i]);
		pthread_create(&table.monitor, NULL, monitor_code, &table);
		i = -1;
		while (++i < table.philo_count)
			pthread_join(table.philo[i].thread, NULL);
		pthread_join(table.monitor, NULL);
		free(table.philo);
	}
	else
		return (call_error(1), 1);
	return (0);
}
