#include "codexion.h"

static int	check_burnout(t_sim *sim, t_coder *coder)
{
	long	now;
	long	last_start;
	long	elapsed;

	now = now_ms(sim);
	last_start = get_last_compile_start(coder);
	elapsed = now - last_start;
	if (elapsed > sim->time_to_burnout)
	{
		set_coder_state(coder, STATE_BURNED_OUT);
		set_burnout_stop(sim);
		log_burnout(coder);
		return (TRUE);
	}
	return (FALSE);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	if (sim == NULL)
		return (NULL);
	while (!is_stopped(sim))
	{
		if (all_coders_done(sim))
		{
			set_stop(sim);
			break ;
		}
		i = 0;
		while (i < sim->number_of_coders && !is_stopped(sim))
		{
			if (check_burnout(sim, &sim->coders[i]))
				return (NULL);
			i++;
		}
		usleep(5);  // ← 5ms instead of no sleep (better precision)
	}
	return (NULL);
}