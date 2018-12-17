/*-------------------------------------------------------------------------*
 *---									---*
 *---		answerer.c						---*
 *---									---*
 *---	    This file defines the answerer program for assignment #2.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//---		Inclusion of header files				---//

#include	"assign2Headers.h"


//---		Definition of constants:				---//

#define		PATTERN_LEN	4


//---		Definition of global vars:				---//

int		answer;

int		numCorrect	= 0;

int		shouldRun	= 1;


//---		Definition of global fncs:				---//

void		timeUpHandler	(int 		sig
				)
{
  shouldRun	= 0;
}


void		guessHandler	(int		sig,
				 siginfo_t*	infoPtr,
				 void*		dataPtr
				)
{
  int	toSendBack;
  int	userBit		= (sig == ONE_SIGNAL);
  int	correctBit	= ((answer >> numCorrect) & 0x1);
  int	isCorrect	= (correctBit == userBit);

  printf("position %d: userBit %d, correctBit %d\n",
	 numCorrect,userBit,correctBit
	);

  if  (isCorrect)
  {
    numCorrect++;

    if  (numCorrect >= PATTERN_LEN)
      toSendBack = WIN_SIGNAL;
    else
      toSendBack = CORRECT_SIGNAL;
  }
  else
  {
    numCorrect	= 0;
    toSendBack	= INCORRECT_SIGNAL;
  }

  kill(infoPtr->si_pid,toSendBack);
}


int		main		(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Run program:
  //  II.A.  Initialize random number generator and choice:
  srand(getpid());

  answer	= rand() % (1 << PATTERN_LEN);

printf("(The answer is %d)\n",answer);

  //  II.B.  Install signal handlers:
  struct sigaction	act;

  memset(&act,'\0',sizeof(act));
  act.sa_handler	= timeUpHandler;
  sigaction(TIME_OVER_SIGNAL,&act,NULL);

  act.sa_flags		= SA_SIGINFO;
  act.sa_sigaction	= guessHandler;
  sigaction(ZERO_SIGNAL,&act,NULL);
  sigaction(ONE_SIGNAL,&act,NULL);

  //  II.C.  Hand out while game still active:
  while  ( (numCorrect < PATTERN_LEN)  &&  shouldRun )
    sleep(1);

  //  III.  Finished, return answer:
  printf("answerer finished\n");
  return(EXIT_SUCCESS);
}