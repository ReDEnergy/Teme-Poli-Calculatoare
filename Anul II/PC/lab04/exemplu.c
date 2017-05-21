#include <stdio.h>
#include <unistd.h>

#include "alarm.h"

int ok;
int main()
{
	const int *was_timeout;
	init_alarm();
	
	was_timeout=set_alarm(2);
	sleep(3);
	cancel_alarm();
	printf("was_timeout=%d\n", *was_timeout);

	was_timeout=set_alarm(1);
	sleep(2);
	cancel_alarm();
	printf("was_timeout=%d\n", *was_timeout);

	cleanup_alarm();
	return 0;
}

