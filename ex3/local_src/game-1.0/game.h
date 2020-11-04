#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

void sigio_handler(int signo);