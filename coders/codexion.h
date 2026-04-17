#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

/*
** ===========================
** Constants
** ===========================
*/

# define TRUE 1
# define FALSE 0

# define SUCCESS 0
# define ERROR 1

/*
** ===========================
** Enums
** ===========================
*/

typedef enum e_scheduler_type
{
	SCHED_TYPE_FIFO,
	SCHED_TYPE_EDF
}	t_scheduler_type;

typedef enum e_coder_state
{
	STATE_WAITING,
	STATE_COMPILING,
	STATE_DEBUGGING,
	STATE_REFACTORING,
	STATE_BURNED_OUT
}	t_coder_state;

/*
** ===========================
** Forward declarations
** ===========================
*/

typedef struct s_sim		t_sim;
typedef struct s_coder		t_coder;
typedef struct s_dongle		t_dongle;
typedef struct s_request	t_request;
typedef struct s_heap		t_heap;

/*
** ===========================
** Request / Heap
** ===========================
*/

struct s_request
{
	int		coder_id;
	long	request_time_ms;
	long	deadline_ms;
	long	seq_num;
};

struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
};

/*
** ===========================
** Dongle
** ===========================
*/

struct s_dongle
{
	int				id;
	int				is_taken;
	int				holder_id;
	long			cooldown_until_ms;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	t_heap			waiting_queue;
};

/*
** ===========================
** Coder
** ===========================
*/

struct s_coder
{
	int				id;
	pthread_t		thread;
	t_sim			*sim;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	long			last_compile_start_ms;
	int				compile_count;
	t_coder_state	state;
	pthread_mutex_t	mutex;
};

/*
** ===========================
** Simulation
** ===========================
*/

struct s_sim
{
	int					number_of_coders;
	long				time_to_burnout;
	long				time_to_compile;
	long				time_to_debug;
	long				time_to_refactor;
	int					number_of_compiles_required;
	long				dongle_cooldown;
	t_scheduler_type	scheduler;

	long				start_time_ms;
	long				request_seq;

	int					stop;
	int					someone_burned_out;
	pthread_mutex_t		stop_mutex;
	pthread_mutex_t		log_mutex;
	pthread_mutex_t		seq_mutex;

	pthread_t			monitor_thread;
	t_coder				*coders;
	t_dongle			*dongles;
};

/*
** ===========================
** Parse
** ===========================
*/

int		parse_args(t_sim *sim, int ac, char **av);
int		parse_positive_int(const char *str, int *out);
int		parse_positive_long(const char *str, long *out);
int		parse_scheduler(const char *str, t_scheduler_type *scheduler);

/*
** ===========================
** Init / Cleanup
** ===========================
*/

int		init_sim(t_sim *sim);
int		init_coders(t_sim *sim);
int		init_dongles(t_sim *sim);
void	assign_dongles(t_sim *sim);
void	cleanup_sim(t_sim *sim);
void	destroy_coders(t_sim *sim);
void	destroy_dongles(t_sim *sim);

/*
** ===========================
** Simulation
** ===========================
*/

int		start_simulation(t_sim *sim);
int		join_simulation(t_sim *sim);

/*
** ===========================
** Routine / Monitor
** ===========================
*/

void	*coder_routine(void *arg);
void	*monitor_routine(void *arg);

/*
** ===========================
** Actions
** ===========================
*/

int		coder_compile(t_coder *coder);
int		coder_debug(t_coder *coder);
int		coder_refactor(t_coder *coder);

/*
** ===========================
** Dongles
** ===========================
*/

int		take_dongles(t_coder *coder);
void	release_dongles(t_coder *coder);
int		try_take_dongle(t_coder *coder, t_dongle *dongle);
int		can_take_dongle(t_coder *coder, t_dongle *dongle);

/*
** ===========================
** Scheduler
** ===========================
*/

int		request_cmp(t_sim *sim, t_request a, t_request b);
long	get_request_deadline(t_coder *coder);
long	next_request_seq(t_sim *sim);

/*
** ===========================
** Heap
** ===========================
*/

int		heap_init(t_heap *heap, int capacity);
void	heap_destroy(t_heap *heap);
int		heap_push(t_heap *heap, t_request req, t_sim *sim);
int		heap_pop(t_heap *heap, t_request *out, t_sim *sim);
t_request	heap_peek(t_heap *heap);
int		heap_is_empty(t_heap *heap);

/*
** ===========================
** Time
** ===========================
*/

long	get_time_ms(void);
long	now_ms(t_sim *sim);
void	smart_sleep(t_sim *sim, long duration_ms);

/*
** ===========================
** Logs
** ===========================
*/

void	log_taken_dongle(t_coder *coder);
void	log_compiling(t_coder *coder);
void	log_debugging(t_coder *coder);
void	log_refactoring(t_coder *coder);
void	log_burnout(t_coder *coder);

/*
** ===========================
** Stop / State
** ===========================
*/

int		is_stopped(t_sim *sim);
void	set_stop(t_sim *sim);
void	set_burnout_stop(t_sim *sim);
int		all_coders_done(t_sim *sim);

void	set_coder_state(t_coder *coder, t_coder_state state);
t_coder_state	get_coder_state(t_coder *coder);
long	get_last_compile_start(t_coder *coder);
void	set_last_compile_start(t_coder *coder, long value);

/*
** ===========================
** Utils
** ===========================
*/

void	print_error(const char *msg);
void	print_usage(void);

#endif