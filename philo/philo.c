/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:32:57 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 18:52:08 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat_even(t_philo *philo)
{
	t_table	*table;
	int		first;
	int		second;

	table = philo->table;
	get_fork_order(philo, &first, &second);
	pthread_mutex_lock(&table->forks[first]);
	print_logs(philo, "has taken a fork");
	pthread_mutex_lock(&table->forks[second]);
	print_logs(philo, "has taken a fork");
	pthread_mutex_lock(&philo->eat_lock);
	philo->time_since_eat = get_cur_time();
	pthread_mutex_unlock(&philo->eat_lock);
	print_logs(philo, "is eating");
	sleep_n_ms(table->eat_time);
	pthread_mutex_lock(&philo->eat_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->eat_lock);
	pthread_mutex_unlock(&table->forks[second]);
	pthread_mutex_unlock(&table->forks[first]);
}

void	eat_odd(t_philo *philo)
{
	t_table	*table;
	int		first;
	int		second;

	table = philo->table;
	get_fork_order(philo, &first, &second);
	pthread_mutex_lock(&table->forks[first]);
	print_logs(philo, "has taken a fork");
	sleep_n_ms(5);
	pthread_mutex_lock(&table->forks[second]);
	print_logs(philo, "has taken a fork");
	pthread_mutex_lock(&philo->eat_lock);
	philo->time_since_eat = get_cur_time();
	pthread_mutex_unlock(&philo->eat_lock);
	print_logs(philo, "is eating");
	sleep_n_ms(table->eat_time);
	pthread_mutex_lock(&philo->eat_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->eat_lock);
	pthread_mutex_unlock(&table->forks[second]);
	pthread_mutex_unlock(&table->forks[first]);
}

void	eat_stuff(t_philo *philo)
{
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
	if (philo->table->philo_count % 2 == 0)
		eat_even(philo);
	else
		eat_odd(philo);
}

void	philo_eat_sleep(t_table *table, t_philo *philo)
{
	pthread_mutex_lock(&table->death_lock);
	if (!table->shall_die && !philo->finished)
	{
		pthread_mutex_unlock(&table->death_lock);
		eat_stuff(philo);
	}
	else
		pthread_mutex_unlock(&table->death_lock);
	pthread_mutex_lock(&table->death_lock);
	if (!table->shall_die && !philo->finished)
	{
		pthread_mutex_unlock(&table->death_lock);
		sleep_think(philo);
	}
	else
		pthread_mutex_unlock(&table->death_lock);
}

void	*philo_code(void *arg)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)arg;
	table = philo->table;
	pthread_mutex_lock(&philo->eat_lock);
	philo->time_since_eat = get_cur_time();
	pthread_mutex_unlock(&philo->eat_lock);
	sleep_n_ms(1);
	if (philo->id % 2 == 1)
		sleep_n_ms(table->eat_time / 2 + philo->id / 100);
	pthread_mutex_lock(&philo->eat_lock);
	philo->time_since_eat = get_cur_time();
	pthread_mutex_unlock(&philo->eat_lock);
	while (1)
	{
		pthread_mutex_lock(&table->death_lock);
		if (table->shall_die || philo->finished)
		{
			pthread_mutex_unlock(&table->death_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&table->death_lock);
		philo_eat_sleep(table, philo);
	}
}
