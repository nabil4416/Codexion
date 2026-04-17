#include "codexion.h"

static int	is_digit_string(const char *str)
{
	int	i;

	if (str == NULL || str[0] == '\0')
		return (FALSE);
	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (FALSE);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	parse_positive_int(const char *str, int *out)
{
	long	value;
	int		i;

	if (out == NULL || !is_digit_string(str))
		return (ERROR);
	value = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		value = (value * 10) + (str[i] - '0');
		if (value > 2147483647)
			return (ERROR);
		i++;
	}
	if (value <= 0)
		return (ERROR);
	*out = (int)value;
	return (SUCCESS);
}

int	parse_positive_long(const char *str, long *out)
{
	long	value;
	int		i;

	if (out == NULL || !is_digit_string(str))
		return (ERROR);
	value = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i] != '\0')
	{
		value = (value * 10) + (str[i] - '0');
		if (value < 0)
			return (ERROR);
		i++;
	}
	if (value <= 0)
		return (ERROR);
	*out = value;
	return (SUCCESS);
}

int	parse_scheduler(const char *str, t_scheduler_type *scheduler)
{
	if (str == NULL || scheduler == NULL)
		return (ERROR);
	if (strcmp(str, "fifo") == 0)
	{
		*scheduler = SCHED_TYPE_FIFO;
		return (SUCCESS);
	}
	if (strcmp(str, "edf") == 0)
	{
		*scheduler = SCHED_TYPE_EDF;
		return (SUCCESS);
	}
	return (ERROR);
}

int	parse_args(t_sim *sim, int ac, char **av)
{
	if (sim == NULL || av == NULL || ac != 9)
		return (print_usage(), ERROR);
	if (parse_positive_int(av[1], &sim->number_of_coders) == ERROR)
		return (print_error("Invalid number_of_coders"), ERROR);
	if (parse_positive_long(av[2], &sim->time_to_burnout) == ERROR)
		return (print_error("Invalid time_to_burnout"), ERROR);
	if (parse_positive_long(av[3], &sim->time_to_compile) == ERROR)
		return (print_error("Invalid time_to_compile"), ERROR);
	if (parse_positive_long(av[4], &sim->time_to_debug) == ERROR)
		return (print_error("Invalid time_to_debug"), ERROR);
	if (parse_positive_long(av[5], &sim->time_to_refactor) == ERROR)
		return (print_error("Invalid time_to_refactor"), ERROR);
	if (parse_positive_int(av[6], &sim->number_of_compiles_required) == ERROR)
		return (print_error("Invalid number_of_compiles_required"), ERROR);
	if (parse_positive_long(av[7], &sim->dongle_cooldown) == ERROR)
		return (print_error("Invalid dongle_cooldown"), ERROR);
	if (parse_scheduler(av[8], &sim->scheduler) == ERROR)
		return (print_error("Scheduler must be fifo or edf"), ERROR);
	return (SUCCESS);
}