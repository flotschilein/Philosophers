/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 22:48:36 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/06 16:31:45 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct s_table
{
	bool	is_limited;
	bool	shall_die;
	int		philo_count;
	int		die_time;
	int		eat_time;
	int		sleep_time;
	t_philo *philo;
} t_table;

typedef struct s_philo
{
	bool	dead;
	int		max_eat;
	int 	id;
	int 	meals_eaten;
} t_philo;

int ft_atoi(char *s)
{
	int			i;
	long int 	ret;

	i = 0;
	ret = 0;
	while(s[i])
	{
		ret *= 10;
		ret += s[i] - '0';
		i++;
	}
	if (ret > 2147483647)
		return (-1);
	return ((int)ret);
}

void call_error(int err)
{
	if (err == 1)
		printf("use with proper input");
	else if (err == 2)
		printf("init fail");
}

bool atoi_fail(t_table *table)
{
	if (table->philo_count == -1)
		return (1);
	if (table->die_time == -1)
		return (1);
	if (table->eat_time	== -1)
		return (1);
	if (table->sleep_time == -1)
		return (1);
	return (0);
}

bool init_table(char **av, int ac, t_table *table)
{
	int i;

	i = 0;
	if (ac == 5)
		table->is_limited 	= 0;
	else
		table->is_limited	= 1;
	table->shall_die 		= 0;
	table->philo_count 		= ft_atoi(av[2]);
	table->die_time 		= ft_atoi(av[3]);
	table->eat_time			= ft_atoi(av[4]);
	table->sleep_time		= ft_atoi(av[5]);
	if(atoi_fail(table))
		return (1);
	table->philo = malloc(table->philo_count * sizeof(t_philo));
	if (!table->philo)
		return (1);
	while(i < table->philo_count)
	{
		fill_philo()
		i++;
	}
	return (0);
}

bool is_only_num(char *arg)
{
	int i = 0;

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

bool input_fail(int ac, char **av)
{
	int i = 1;

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
	t_table table;
	int fail;

	fail = 0;
	memset(&table, 0, sizeof(t_table));
	if ((ac == 5 || ac == 6) && !input_fail(ac, av))
	{
		if (init_table(av, ac, &table))
			return (call_error(2), 1);
	}
	else
		return (call_error(1), 1);
	return (0);
}
