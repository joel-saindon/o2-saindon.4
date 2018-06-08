#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/msg.h>

#define TIMER_MSG "Alarm received, killing processes, freeing memory\n"

key_t shmkey1 = 640640;
key_t shmkey2 = 464064;
key_t shmkey3 = 444767;

int maxproc;
char * t_opt_arg;
char * l_opt_arg;
char * s_opt_arg;

int timeout = 0;
char * logfile;
static volatile sig_atomic_t doneflag = 0;


/*ARGSUSED*/
static void setdoneflag(int signo){
	doneflag = 1;
}

struct sigaction act;
