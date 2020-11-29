#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>

#define CHANGE_PAUSE_TIME _IOW(61,4,int)

int main() {

	int fd = open("/dev/morse");
	ioctl(fd, CHANGE_PAUSE_TIME, 2000);
	close(fd);
	return 0;
}
