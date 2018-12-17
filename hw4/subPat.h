/*-------------------------------------------------------------------------*
 *---									---*
 *---		subPat.h						---*
 *---									---*
 *---	    This file declares C functions and constants common to both	---*
 *---	subPatClient.c and subPatServer.c.				---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2018 November 8		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//---		Header file inclusion					---//

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>	// For unlink()
#include	<sys/types.h>	// For waitpid(), opendir()
#include	<sys/wait.h>	// For waitpid()
#include	<dirent.h>	// For opendir(), readdir(), closedir()
#include	<sys/socket.h>	// For socket()
#include	<netinet/in.h>	// For sockaddr_in and htons()
#include	<netdb.h>	// For getaddrinfo()
#include	<errno.h>	// For errno var
#include	<sys/stat.h>	// For open(), read(),write(), stat()
#include	<fcntl.h>	// and close()


//---		Definition of constants:				---//

#define		BUFFER_LEN		512

#define		DIR_CMD_CHAR		'l'

#define		READ_CMD_CHAR		'r'

#define		WRITE_CMD_CHAR		'w'

#define		SUBSTITUTE_CMD_CHAR	's'

#define		DELETE_CMD_CHAR		'd'

#define		QUIT_CMD_CHAR		'q'

const int	MIN_FILE_NUM		= 0;

const int	MAX_FILE_NUM		= 63;

#define		SUBSTITUTE_PROGNAME	"/usr/bin/sed"

#define		SUBSTITUTE_PROG_OPTION	"-i"

#define		FILENAME_EXTENSION	".txt"