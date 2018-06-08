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
	int i;
	int status;
	printf("from OSS\n");
	sleep(3);
	
	//get shared memory segment for clock
	int shm_clock = shmget(shmkey1, sizeof(int), IPC_CREAT | 0666);
	if(shm_clock < 0){
		perror("shmget failed");
		exit(1);	
	}
	
	//initialize message queue
	int msgqid;
	int msgflag = 666;
	
	key_t msgqkey = shmkey2;

	msgqid = msgget(shmkey2, IPC_CREAT | 0666);
	if(msgqid == -1){
		perror("mssget");
		exit(1);
	} else
		printf("msg queue created\n");
	sleep(2);

	int msgerror = msgctl(msgqid, IPC_RMID, NULL);
	if(msgerror == -1){
		perror("msg queue removal error\n");
		exit(1);
	} else 
		printf("msg queue deallocated\n");
	
	//attach clock to shared memory
	int * clock = (int *)shmat(shm_clock,NULL,0);
	printf("clock initialized\n");
	int * test = clock;
	*test = *test + 1;

	//fork user processes
	printf("maxproc before fork loop: %d\n", maxproc);
	pid_t pids[maxproc];
	for(i=0; i< maxproc; i++){
		if((pids[i] = fork()) < 0){
			perror("fork");
			return 1;
		}
		else if(pids[i] == 0){ //if in forked process
			printf("user process created %ld\n", getpid());
			execl("./user", "./user", NULL);
			exit(0);
		}
		else{
			(void)waitpid(pids[i], &status, 0);
		}
	}

	//detach clock from shared memory
	int clockRelease = shmdt(clock);
	if(clockRelease < 0){
		perror("shmdt fail on releasing clock var");
	}
	//delete/release shared memory 
	int clockDeleteError = shmctl(shm_clock, IPC_RMID, NULL);
	if(clockDeleteError < 0){
		perror("shmctl error on clock var\n");
	}
	sleep(1);
	printf("oss done\n");

return 0;
}
