/*-------------------------------------------------------------------------*
 *---									---*
 *---		assign2Headers.h					---*
 *---									---*
 *---	    This file includes standard headers and declares constants	---*
 *---	common to the assignment 2 C programs.				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a					Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//---		Common standard header files				---//

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<signal.h>
#include	<sys/types.h>
#include	<sys/wait.h>
#include	<unistd.h>


//---		Common constants:					---//

#define		ZERO_SIGNAL		SIGUSR1

#define		ONE_SIGNAL		SIGUSR2

#define		CORRECT_SIGNAL		SIGUSR1

#define		INCORRECT_SIGNAL	SIGUSR2

#define		WIN_SIGNAL		SIGINT

#define		TIME_OVER_SIGNAL	SIGTERM

#define		GUESSER_PROGNAME	"guesser"

#define		ANSWERER_PROGNAME	"answerer"

#define		LINE_LEN		256

#define		NUM_SECONDS		30