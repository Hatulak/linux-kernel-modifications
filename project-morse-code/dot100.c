#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

#define CHANGE_DOT_TIME _IOW(60,2,int)

int main() {
	int fd = open("/dev/morse");
	ioctl(fd, CHANGE_DOT_TIME, 100);
	close(fd);
	return 0;
}
