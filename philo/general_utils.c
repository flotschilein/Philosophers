/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:30:13 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 17:31:14 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_logs(t_philo *philo, char *msg)
{
	long long	local_time;

	local_time = get_cur_time() - philo->table->start_time;
	pthread_mutex_lock(&philo->table->death_lock);
	pthread_mutex_lock(&philo->table->write_lock);
	if (philo->table->shall_die)
	{
		if (philo->table->first_death)
		{
			printf("%lld %d died\n", local_time, philo->id);
			philo->table->first_death = 0;
		}
		pthread_mutex_unlock(&philo->table->death_lock);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (!philo->table->shall_die)
		printf("%lld %d %s\n", local_time, philo->id, msg);
	pthread_mutex_unlock(&philo->table->death_lock);
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	sleep_think(t_philo *philo)
{
	if (philo->finished)
		return ;
	print_logs(philo, "is sleeping");
	sleep_n_ms(philo->table->sleep_time);
	print_logs(philo, "is thinking");
}
