/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep_time_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:09:08 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 17:09:53 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	while ((get_cur_time() - start_time_local) < ms - 1)
		usleep(1000);
	while ((get_cur_time() - start_time_local) < ms)
		usleep(100);
}