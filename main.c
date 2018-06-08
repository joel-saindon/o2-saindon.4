#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main (int argc, char * argv[]){

	int opt;
	while((opt = getopt(argc,argv, "hs:l:t:")) != -1){
		switch (opt){
			case 'h':
				printf("Help Text");
				return -1;
			case 's':
				printf("s option, args %d \n", argc);
				//printf("optarg: %s\n", optarg);
				int test = atoi(argv[2]);
				maxproc = test;
				int shm_maxproc = shmget(shmkey3, sizeof(int), IPC_CREAT | 0666)
				if(shm_maxproc < 0){
					perror("maxproc shmget fail");
					exit(1);
				}
				break;
			case 'l':
				printf("l option\n");
				logfile = argv[6];
				printf("%s\n", logfile);
				break;
			case 't':
				printf("t option\n");
				timeout = atoi(argv[4]);
				printf("timeout: %d\n", timeout);
				break;
			case '?':
				printf("Usage: \n");
				return -1;
			default:
				return -1;	
		}
	}

	printf("Hello World\n");
	printf("maxproc: %d\n", maxproc);
	int status;
	pid_t childpid = 0;
	pid_t waitreturn;
	int i = 0;

	for(i=0; i< 1; i++){
		if((childpid = fork()) < 0){
			perror("fork");
			return -1;
		}
		else if (childpid == 0){
			printf("child created %d\n", getpid());
			execl("./oss", "./oss", NULL);
			exit(0);
		}
		else{
			(void)waitpid(childpid, &status, 0);
		}
	}

return 0;
}

