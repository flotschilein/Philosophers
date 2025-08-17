/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 22:48:36 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 17:33:52 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;
	int		i;

	i = -1;
	memset(&table, 0, sizeof(t_table));
	if (!((ac == 5 || ac == 6) && !input_fail(ac, av)))
		return (call_error(1), 1);
	if (init_table(av, ac, &table))
		return (call_error(2), 1);
	if (init_mutex(&table))
		return (free(table.philo), call_error(2), 1);
	while (++i < table.philo_count)
		pthread_create(&table.philo[i].thread, NULL, philo_code,
			&table.philo[i]);
	pthread_create(&table.monitor, NULL, monitor_code, &table);
	while (i > 0)
		pthread_join(table.philo[--i].thread, NULL);
	pthread_join(table.monitor, NULL);
	while (i < table.philo_count)
		pthread_mutex_destroy(&table.forks[i++]);
	pthread_mutex_destroy(&table.write_lock);
	pthread_mutex_destroy(&table.death_lock);
	free(table.philo);
	free(table.forks);
}
