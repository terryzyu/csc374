#include "assign2Headers.h"

pid_t answererPID;
pid_t guesserPID;

int shouldRun = 1;

//Should handle alarm. Sends signal to respective children
void sigAlarmHandler(int sig){
	kill(answererPID, TIME_OVER_SIGNAL);
	kill(guesserPID, TIME_OVER_SIGNAL);
	shouldRun = 0;
}

//Gathers dead children
void sigChildHandler(int sig){
	int s;
	while((waitpid(1,&s,WNOHANG)) > 0){}
	shouldRun = 0;
}

int main(){

	//Signal handlers
	struct sigaction act;

	memset(&act, '\0', sizeof(act));

	act.sa_handler = sigAlarmHandler;
	sigaction(SIGALRM, &act, NULL);
	alarm(NUM_SECONDS);

	act.sa_handler = sigChildHandler;
	sigaction(SIGCHLD, &act, NULL);

	char line[LINE_LEN];
	char* argv[3];
	argv[0] = GUESSER_PROGNAME;
	argv[2] = NULL;

	answererPID = fork();

	if(answererPID != 0){
		execl(ANSWERER_PROGNAME, ANSWERER_PROGNAME, argv[0], line, (char*) NULL);
	}
	else{
		sleep(1);
		snprintf(line, LINE_LEN, "%d", answererPID);
		guesserPID = fork();

		if(guesserPID == 0)
			execl(GUESSER_PROGNAME, GUESSER_PROGNAME, argv[0], line, (char*) NULL);
		else{
			//alarm(NUM_SECONDS);
			while(shouldRun)
				sleep(1);
			sleep(1);
			sleep(1);
			printf("launcher finished\n");
			return(EXIT_SUCCESS);
		}
	}

}