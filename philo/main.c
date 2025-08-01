/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 22:48:36 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/01 15:18:56 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

void call_error(int err)
{
	if (err == 1)
		printf("use with proper arguments");
}
void init_philo_perm(char **av)
{

}

void init_philo_amount(char **av)
{

}
bool input_fail(char **av)
{

}
int	main(int ac, char **av)
{
	if (ac == 5 && !input_fail(av))
		init_philo_perm(av);
	else if (ac == 6 && !input_fail(av))
		init_philo_amount(av);
	else
	{
		call_error(1);
		return (1);
	}
	return (0);
}