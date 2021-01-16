#include <stdio.h>
#include <linux/unistd.h>
#include <sched.h>
#include <errno.h>

_syscall2(int,change_time,int,od_czasu,int,do_czasu)

int main() {
    struct sched_param param;
    int my_pid = 0;
    int policy, priority;
    int sts;
    int i=0;

    policy = sched_getscheduler(my_pid);
    printf("policy %d\n", policy);

    sts = sched_getparam(my_pid, &param);

    printf("pobrane %d\n", sts);
    printf("priority %d\n", param.sched_priority);
    printf("err code %d\n", errno);

    param.sched_priority = 1;

    sts = sched_setscheduler(my_pid,3,&param);
    printf("zmiana %d\n", sts);
    printf("err code %d\n", errno);

    policy = sched_getscheduler(my_pid);
    
    while(i < 100){
        i++;
        printf("nowe policy %d\n", policy);

    }

    change_time(80000,81000);

	return 0;
}