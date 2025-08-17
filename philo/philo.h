/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 22:48:45 by fbraune           #+#    #+#             */
/*   Updated: 2025/08/17 17:31:24 by fbraune          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	bool			finished;
	int				max_eat;
	int				id;
	int				meals_eaten;
	long long		time_since_eat;
	pthread_t		thread;
	struct s_table	*table;
	pthread_mutex_t	eat_lock;
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
	bool			first_death;
}					t_table;

void				sleep_n_ms(long long ms);
long long			get_cur_time(void);

bool				input_fail(int ac, char **av);
bool				is_only_digit(char *arg);
bool				atoi_fail(t_table *table);
void				call_error(int err);
int					ft_atoi(char *s);

bool				init_mutex(t_table *table);
bool				init_table(char **av, int ac, t_table *table);
void				add_table_pointer(t_table *table);
void				fill_philo(t_philo *philo, int id, char **av, int ac);

void				*monitor_code(void *arg);
bool				check_eat_amount(t_table *table);
void				set_done(t_table *table);
bool				did_not_eat(t_table *table, int i);

void				print_logs(t_philo *philo, char *msg);
void				sleep_think(t_philo *philo);

#endif