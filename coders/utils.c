#include "codexion.h"

void	print_error(const char *msg)
{
	if (msg == NULL)
		return ;
	fprintf(stderr, "Error: %s\n", msg);
}

void	print_usage(void)
{
	fprintf(stderr,
		"Usage: ./codexion "
		"number_of_coders "
		"time_to_burnout "
		"time_to_compile "
		"time_to_debug "
		"time_to_refactor "
		"number_of_compiles_required "
		"dongle_cooldown "
		"scheduler\n");
	fprintf(stderr, "scheduler must be: fifo or edf\n");
}