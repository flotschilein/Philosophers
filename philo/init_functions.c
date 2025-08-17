/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:20:49 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 20:17:18 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fill_philo(t_philo *philo, int id, char **av, int ac)
{
	int	max_eat;

	max_eat = -1;
	philo->id = id;
	philo->meals_eaten = 0;
	philo->finished = 0;
	if (ac == 6)
		max_eat = ft_atoi(av[5]);
	philo->max_eat = max_eat;
}

void	add_table_pointer(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		table->philo[i].table = table;
		pthread_mutex_lock(&table->philo[i].eat_lock);
		table->philo[i].time_since_eat = table->start_time;
		pthread_mutex_unlock(&table->philo[i].eat_lock);
		i++;
	}
}

bool	init_table(char **av, int ac, t_table *table)
{
	int	i;

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
	table->first_death = 1;
	if (atoi_fail(table))
		return (1);
	table->philo = malloc(table->philo_count * sizeof(t_philo));
	if (!table->philo)
		return (1);
	i = -1;
	while (++i < table->philo_count)
		fill_philo(&table->philo[i], i + 1, av, ac);
	return (0);
}

bool	init_mutex(t_table *table)
{
	int	i;

	table->forks = malloc(table->philo_count * sizeof(pthread_mutex_t));
	if (!table->forks)
		return (1);
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (1);
		if (pthread_mutex_init(&table->philo[i].eat_lock, NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->death_lock, NULL) != 0)
		return (1);
	add_table_pointer(table);
	return (0);
}
