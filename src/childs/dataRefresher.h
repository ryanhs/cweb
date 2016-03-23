#ifndef _DATA_REFRESHER_H
#define _DATA_REFRESHER_H

#include "../models/config.h"

void refresher_init();

void getTime(char *timing);
void pthread_mysleep(int s);

#endif
