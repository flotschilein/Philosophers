/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:15:35 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 17:16:33 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (ret > INT_MAX)
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

bool	is_only_digit(char *arg)
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
		if (!is_only_digit(av[i]))
			return (1);
		i++;
	}
	return (0);
}
