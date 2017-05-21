#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static int alarm_armed;
static void (*alarm_saved_handler)(int);
extern int last;
extern int ok;

static void alarm_handler(int sig)
{
	ok = 0;
	printf(" Alarm-last :");
	alarm_armed=1;
/*	#ifdef DEBUG
		fprintf(stderr, "in alarm\n");
	#endif
*/	
}

void cancel_alarm()
{
	alarm(0);
}

const int * set_alarm(unsigned int timeout)
{
	alarm_armed=0;
	alarm(timeout);
	return &alarm_armed;
}

int init_alarm()
{
	alarm_saved_handler=signal(SIGALRM, alarm_handler);	
	siginterrupt(SIGALRM, 1);	
	return 0;
}

void cleanup_alarm()
{
	signal(SIGALRM, alarm_saved_handler);
}
