/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:25:02 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 17:27:10 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	did_not_eat(t_table *table, int i)
{
	long long	start_time_local;
	bool		should_die;

	should_die = 0;
	start_time_local = get_cur_time();
	pthread_mutex_lock(&table->death_lock);
	pthread_mutex_lock(&table->philo[i].eat_lock);
	if (start_time_local - table->philo[i].time_since_eat > table->die_time)
	{
		table->shall_die = 1;
		should_die = 1;
	}
	pthread_mutex_unlock(&table->philo[i].eat_lock);
	pthread_mutex_unlock(&table->death_lock);
	if (should_die)
	{
		print_logs(&table->philo[i], "died");
		return (1);
	}
	return (0);
}

void	set_done(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		table->philo[i].finished = 1;
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
		pthread_mutex_lock(&table->philo[i].eat_lock);
		if (table->philo[i].meals_eaten >= table->philo[i].max_eat)
			full_count++;
		pthread_mutex_unlock(&table->philo[i].eat_lock);
		i++;
	}
	if (full_count == table->philo_count)
	{
		pthread_mutex_lock(&table->death_lock);
		set_done(table);
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
