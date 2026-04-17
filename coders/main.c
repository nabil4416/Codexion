#include "codexion.h"

static void	zero_sim(t_sim *sim)
{
	memset(sim, 0, sizeof(t_sim));
}

int	main(int ac, char **av)
{
	t_sim	sim;

	zero_sim(&sim);
	if (parse_args(&sim, ac, av) == ERROR)
		return (ERROR);
	if (init_sim(&sim) == ERROR)
	{
		cleanup_sim(&sim);
		return (ERROR);
	}
	if (start_simulation(&sim) == ERROR)
	{
		set_stop(&sim);
		cleanup_sim(&sim);
		return (ERROR);
	}
	if (join_simulation(&sim) == ERROR)
	{
		cleanup_sim(&sim);
		return (ERROR);
	}
	cleanup_sim(&sim);
	return (SUCCESS);
}