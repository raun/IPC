#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 

int main(){
	int shmid;
	key_t key=5678;
	int *shm_state,score=0,diceScore,sec;

	if((shmid=shmget(key,sizeof(int),IPC_CREAT|0666))<0){
		printf("Error in shmget()\n");
		exit(1);
	}

	if((shm_state=(int *)shmat(shmid,NULL,0))==(int *)-1){
		printf("Error in shmat()\n");
		exit(2);
	}
	srand(time(0));
	*shm_state=0;
	while(1){
		while((*shm_state>=0)&&(*shm_state<3)){
			printf("\nWaiting for player 1 ");
			printf("\nScore : %d",score);
			sleep(2);
			if(*shm_state==7){
				shmdt(shm_state);
				shmctl(shmid, IPC_RMID, 0);
				exit(0);
			}
		}
		if(*shm_state==7){
			shmdt(shm_state);
			shmctl(shmid, IPC_RMID, 0);
			exit(0);
		}
		diceScore=(rand()%6)+1;
		printf("\nPlayer 2 dice : %d",diceScore);
		if(diceScore==6){
			printf("\nSecondary calling...");
			*shm_state=4;
			sec = (rand()%100)+1;
			printf("\nNumber generated : %d",sec);
			score= score+sec;
			printf("\nScore after secondary:%d",score);
			printf("\nChecking if player 2 won or not??");
			*shm_state=5;
			if(score>500){
				printf("Player 2 won...");
				shmdt(shm_state);
				exit(0);
			}
		}
		*shm_state=0;
	}
}