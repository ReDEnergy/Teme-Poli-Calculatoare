#ifndef _ALARM_H
#define _ALARM_H

extern void cancel_alarm();
extern const int * set_alarm(unsigned int timeout);

extern int init_alarm();
extern void cleanup_alarm();



#endif //_ALARM_H
