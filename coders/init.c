#include "codexion.h"

static void	init_sim_defaults(t_sim *sim)
{
	sim->start_time_ms = 0;
	sim->request_seq = 0;
	sim->stop = FALSE;
	sim->someone_burned_out = FALSE;
	sim->coders = NULL;
	sim->dongles = NULL;
}

int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = (t_dongle *)malloc(sizeof(t_dongle)
			* sim->number_of_coders);
	if (sim->dongles == NULL)
		return (print_error("Failed to allocate dongles"), ERROR);
	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].is_taken = FALSE;
		sim->dongles[i].holder_id = 0;
		sim->dongles[i].cooldown_until_ms = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (print_error("Failed to init dongle mutex"), ERROR);
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (print_error("Failed to init dongle cond"), ERROR);
		if (heap_init(&sim->dongles[i].waiting_queue,
				sim->number_of_coders) == ERROR)
			return (print_error("Failed to init dongle queue"), ERROR);
		i++;
	}
	return (SUCCESS);
}

void	assign_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].left_dongle = &sim->dongles[i];
		if (sim->number_of_coders == 1)
			sim->coders[i].right_dongle = &sim->dongles[i];
		else
			sim->coders[i].right_dongle = &sim->dongles[(i + 1)
				% sim->number_of_coders];
		i++;
	}
}

int	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = (t_coder *)malloc(sizeof(t_coder)
			* sim->number_of_coders);
	if (sim->coders == NULL)
		return (print_error("Failed to allocate coders"), ERROR);
	i = 0;
	while (i < sim->number_of_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].left_dongle = NULL;
		sim->coders[i].right_dongle = NULL;
		sim->coders[i].last_compile_start_ms = 0;
		sim->coders[i].compile_count = 0;
		sim->coders[i].state = STATE_WAITING;
		if (pthread_mutex_init(&sim->coders[i].mutex, NULL) != 0)
			return (print_error("Failed to init coder mutex"), ERROR);
		i++;
	}
	assign_dongles(sim);
	return (SUCCESS);
}

int	init_sim(t_sim *sim)
{
	if (sim == NULL)
		return (ERROR);
	init_sim_defaults(sim);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (print_error("Failed to init stop mutex"), ERROR);
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (print_error("Failed to init log mutex"), ERROR);
	if (pthread_mutex_init(&sim->seq_mutex, NULL) != 0)
		return (print_error("Failed to init seq mutex"), ERROR);
	if (init_dongles(sim) == ERROR)
		return (ERROR);
	if (init_coders(sim) == ERROR)
		return (ERROR);
	sim->start_time_ms = get_time_ms();
	return (SUCCESS);
}