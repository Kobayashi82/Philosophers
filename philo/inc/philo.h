/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vzurera- <vzurera-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 19:51:40 by vzurera-          #+#    #+#             */
/*   Updated: 2024/02/18 18:30:52 by vzurera-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

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
	MTX_INIT = 15,
	MTX_LOCK = 16,
	MTX_UNLOCK = 17,
	MTX_DESTROY = 18,
	THRD_CREATE = 20,
	THRD_JOIN = 21,
	THRD_DETACH = 22,
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

typedef struct s_table	t_table;
typedef pthread_mutex_t	t_mtx;

typedef struct s_philo
{
	int			id;
	int			meal_done;
	long		curr_meal;
	long		last_meal;
	pthread_t	thread;
	t_mtx		*fork_a;
	t_mtx		*fork_b;
	t_mtx		mtx_philo;
	t_table		*table;
}	t_philo;

struct	s_table
{
	long		t_philos;
	long		t_die;
	long		t_eat;
	long		t_sleep;
	long		t_meals;
	long		dinner_start;
	int			dinner_end;
	int			ready;
	long		th;
	pthread_t	maitre;
	t_philo		*philos;
	t_mtx		*forks;
	t_mtx		mtx_table;
	t_mtx		mtx_waiter;
};

//	MAIN
long	get_time(int mode);
void	wait_time(t_table *table, long usec);
void	exit_error(t_table *table, int error);
//	TABLE
void	set_table(t_table *table, int argc, char **argv);
void	clean_table(t_table *table, int error);
//	DINNER
void	dinner(t_table *table);
//	MUTEX
void	mtx_set(t_table *table, t_mtx *mutex, int action);
void	set_int(t_table *table, t_mtx *mutex, int *value1, int value2);
int		get_int(t_table *table, t_mtx *mutex, int *value1);
void	set_long(t_table *table, t_mtx *mutex, long *value1, long value2);
long	get_long(t_table *table, t_mtx *mutex, long *value);

#endif
