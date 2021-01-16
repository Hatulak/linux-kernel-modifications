#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/unistd.h>
#include <linux/linkage.h>

_syscall2(int,change_time,int,od_czasu,int,do_czasu)

int main() {
    change_time(4000,81000);
	return 0;
}
