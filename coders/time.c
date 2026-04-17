#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

long	now_ms(t_sim *sim)
{
	if (sim == NULL)
		return (0);
	return (get_time_ms() - sim->start_time_ms);
}

void	smart_sleep(t_sim *sim, long duration_ms)
{
	long	start;

	start = get_time_ms();
	while (!is_stopped(sim))
	{
		if ((get_time_ms() - start) >= duration_ms)
			break ;
		usleep(500);
	}
}