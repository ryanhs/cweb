#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#include "dataRefresher.h"

#include "../models/config.h"
#include "../models/route.h"

int stmp = 3;

void *refresher_child_common(void *param){
	while(1) {
        pthread_mysleep(60 * 3);
        
		//refresh regularly somethings
	}
}

void refresher_init(){	
	pthread_t refresher_common
			  ;
			  
    pthread_create(&refresher_common, NULL, refresher_child_common, NULL);
}

void getTime(char *timing){
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(timing, 80, "%Y-%m-%d %H:%I:%S %p", timeinfo);
}

pthread_mutex_t fakeMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fakeCond = PTHREAD_COND_INITIALIZER;

void pthread_mysleep(int s){
	struct timespec timeToWait;
    struct timeval now;
    
    gettimeofday(&now,NULL);

    timeToWait.tv_sec = now.tv_sec + s;
    timeToWait.tv_nsec = now.tv_usec*1000;

    pthread_mutex_lock(&fakeMutex);
    pthread_cond_timedwait(&fakeCond, &fakeMutex, &timeToWait);
    pthread_mutex_unlock(&fakeMutex);
}
