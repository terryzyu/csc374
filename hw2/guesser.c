#include "assign2Headers.h"

int shouldRun = 1;

void timeOverHandler(int sigNum){
	printf("Oh no!  The time is up!");
	shouldRun = 0;
}

void winHandler(int sigNum){
	shouldRun = 0;
}

void correctHandler(){
	//ANNOUNCE USER GOT THE GUESS CORRECT
	printf("That was correct\n");
}

void incorrectHandler(){
	//ANNOUCE USER IS WRONG
	printf("That was wrong. Restart.\n");
}


int main(int argc, char* argv[]){
	//Gotta do some other stuff here
	char input[LINE_LEN]; //Gets user input
	int guess; //Parses input into a workable 0 or 1

	pid_t answererPID = atoi(argv[1]); //Stores process ID

	//ALL SIGNAL HANDLER STUFF
	struct sigaction act;
	memset(&act, '\0', sizeof(act));

	act.sa_handler = timeOverHandler;
	sigaction(TIME_OVER_SIGNAL, &act, NULL);

	act.sa_handler = winHandler;
	sigaction(WIN_SIGNAL, &act, NULL);

	act.sa_handler = correctHandler;
	sigaction(CORRECT_SIGNAL, &act, NULL);

	act.sa_handler = incorrectHandler;
	sigaction(INCORRECT_SIGNAL, &act, NULL);


	//Loop to play game and get user input
	while(shouldRun){
		printf("Enter your guess: 0 or 1\n");
		fgets(input, LINE_LEN, stdin);
		guess = atoi(input);

		if(guess == 0)
			kill(answererPID, ZERO_SIGNAL);
		else if(guess == 1)
			kill(answererPID, ONE_SIGNAL);
		sleep(1);
	}

	printf("guesser finished\n");
	return (EXIT_SUCCESS);

}