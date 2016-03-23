#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <resolv.h>	
#include <signal.h>	

#include "../models/config.h"

void signal_handler(int signal){
	if(DEBUG);
		printf("signal received: \n  - ");
	
	switch(signal){
		case SIGINT:
			printf("SIGINT <- external interrupt, usually initiated by the user\n");
			break;
		case SIGTERM:
			printf("SIGTERM <- termination request, sent to the program\n");
			break;
		case SIGSEGV:
			printf("SIGSEGV <- invalid memory access (segmentation fault)\n");
			break;
		case SIGILL:
			printf("SIGILL <- invalid program image, such as invalid instruction\n");
			break;
		case SIGABRT:
			printf("SIGABRT <- abnormal termination condition, as is e.g. initiated by abort()\n");
			break;
		case SIGFPE:
			printf("SIGFPE <- erroneous arithmetic operation such as divide by zero\n");
			break;
		default:
			printf("unknown signal -> %d\n", signal);
			break;
	}
	
	abort();
}

void handle_signal(){
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGFPE, signal_handler);
}
