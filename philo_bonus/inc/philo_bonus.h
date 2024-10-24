/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 23:48:09 by vzurera-          #+#    #+#             */
/*   Updated: 2024/02/25 15:13:48 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>

# ifndef MAX_PHILOS
#  define MAX_PHILOS 250 
# endif

# define NC   	"\x1b[0m"		// Default
# define RD		"\033[1;31m"	// Bold Red
# define G     	"\033[1;32m"	// Bold Gree
# define Y     	"\033[1;33m"	// Bold Yellow
# define B     	"\033[1;34m"	// Bold Blue
# define M     	"\033[1;35m"	// Bold Magenta
# define C     	"\033[1;36m"	// Bold Cyan
# define W     	"\033[1;37m"	// Bold White

# define UN 	"\033[4m"		// Underline
# define IT 	"\033[3m"		// Italic

enum e_errors
{
	ARG_MAX_PHILOS = 8,
	ARG_BAD = 9,
	ARG_NEG = 10,
	ARG_NDIG = 11,
	ARG_MINT = 12,
	ARG_TIMES = 13,
	SM_FAILED = 14,
	NO_FORK = 15,
	THRD_CREATE = 20,
	THRD_JOIN = 21,
	NO_MEMORY = 30,
	END = 99
};

enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	FORK_A,
	FORK_B,
	DIED,
	SEC,
	MLSEC,
	MCRSEC
};

typedef struct s_table
{
	long		t_philos;
	long		t_die;
	long		t_eat;
	long		t_sleep;
	long		t_meals;
	long		dinner_start;
	int			dinner_end;
	int			*pid;
	sem_t		*sm_waiter;
	sem_t		*sm_forks;
	sem_t		*sm_table;
	pthread_t	maitre;
	int			id;
	int			curr_meal;
	long 		last_meal;
	int			died;
}	t_table;

//	MAIN
long	get_time(int mode);
void	wait_time(t_table *table, long time);
void	exit_error(t_table *table, int error);
//	TABLE
void	set_table(t_table *table, int argc, char **argv);
void	clean_table(t_table *table, int error);
//	DINNER
void	dinner(t_table *table);

#endif
