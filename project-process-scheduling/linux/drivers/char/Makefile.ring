makering: ring.c
	gcc -D__KERNEL__ -DMODULE -O2 -c ring.c
