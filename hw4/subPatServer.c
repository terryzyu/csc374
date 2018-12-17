/*-------------------------------------------------------------------------*
 *---									---*
 *---		subPatServer.c						---*
 *---									---*
 *---	    This file defines a C program that gets file-sys commands	---*
 *---	from client via a socket, executes those commands in their own	---*
 *---	threads, and returns the corresponding output back to the	---*
 *---	client.								---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2018 November 8		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//	Compile with:
//	$ gcc subPatServer.c -o subPatServer -lpthread

//---		Header file inclusion					---//

#include	"subPat.h"
#include	<pthread.h>// For pthread_create()


//---		Definition of constants:				---//

#define		STD_OKAY_MSG		"Okay"

#define		STD_ERROR_MSG		"Error doing operation"

#define		STD_BYE_MSG		"Good bye!"

#define		THIS_PROGRAM_NAME	"subPatServer"

#define		OUTPUT_FILENAME		"out.txt"

#define		ERROR_FILENAME		"err.txt"

const int	ERROR_FD		= -1;


//---		Definition of global vars:				---//

//  PURPOSE:  To be non-zero for as long as this program should run, or '0'
//	otherwise.


//---		Definition of functions:				---//

//  YOUR CODE HERE


//  PURPOSE:  To run the server by 'accept()'-ing client requests from
//	'listenFd' and doing them.

void* dirCMD(int clientFd){
  DIR* dirPtr = opendir("."); //attempt to open dir
  char buffer[BUFFER_LEN];
  if(dirPtr  == NULL) //error message if can't open or DNE
    write(clientFd, STD_ERROR_MSG, sizeof(STD_ERROR_MSG));

  struct dirent* entryPtr;

  while((entryPtr = readdir(dirPtr)) != NULL){
    strncat(buffer, entryPtr->d_name, BUFFER_LEN);
    strncat(buffer, "\n", BUFFER_LEN);
  }

  write(clientFd, buffer, strlen(buffer));
  closedir(dirPtr); //close directory
} //dirCMD()


void* readCMD(int clientFd, int fileNum){
  char buffer[BUFFER_LEN];
    //Create the filename from fileNum with:
  char fileName[BUFFER_LEN];
  snprintf(fileName,BUFFER_LEN,"%d%s",fileNum,FILENAME_EXTENSION);

  //Opening the file to read
  fileNum = open(fileName, O_RDONLY, 0440);

  //then send std error msg back to client
  if(fileNum < 0)
    write(clientFd, STD_ERROR_MSG, sizeof(STD_ERROR_MSG));

  //Read up to buffer len chars from that file
  read(fileNum, buffer, BUFFER_LEN);
  strcat(buffer, "\0"); //add null character to end
  write(clientFd, buffer, strlen(buffer)); //send buffer to client
  close(fileNum); //close file
} //readCMD()

void* writeCMD(int clientFd, int fileNum, void* textPtr){
  char* text = (char*) textPtr;
  char fileName[BUFFER_LEN];
  snprintf(fileName,BUFFER_LEN,"%d%s",fileNum,FILENAME_EXTENSION);

  int length = strlen(text);
  int writtenSoFar;

  int outputFile = open(fileName, O_WRONLY|O_CREAT, 0660);

  if(length <= BUFFER_LEN)
    writtenSoFar = write(outputFile, text, length);
  else
    writtenSoFar = write(outputFile, text, BUFFER_LEN);

  if(!(outputFile == -1 || writtenSoFar == -1))
    write(clientFd, STD_OKAY_MSG, sizeof(STD_OKAY_MSG));
  else
    write(clientFd, STD_ERROR_MSG, sizeof(STD_ERROR_MSG));

  free(textPtr);
  close(outputFile);
}//writeCMD()


void* sedCMD(int clientFd, int fileNum, char text[], char text2[]){
  char fileName[BUFFER_LEN]; 

  //create file name as before
  snprintf(fileName,BUFFER_LEN,"%d%s",fileNum,FILENAME_EXTENSION);
  int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0660);
  char substitution[BUFFER_LEN]; 
  
  //create the command to tell sed the substitution with
  snprintf(substitution, BUFFER_LEN, "s/%s/%s/g", text, text2);
  int sedFork = fork(); //fork it
  int status;
  if (sedFork == 0) {//should do an execl
    if (execl(SUBSTITUTE_PROGNAME, substitution, fileName, NULL) == -1)
      return((void *)EXIT_FAILURE);
    else
      return(EXIT_SUCCESS);
  }
  
  else { //wait for it, worked or failed
    int p = waitpid(sedFork, &status, 0);
    if (WIFEXITED(status)) {
      write(clientFd, STD_OKAY_MSG, sizeof(STD_OKAY_MSG));
    }
    else {
      write(clientFd, STD_ERROR_MSG, sizeof(STD_ERROR_MSG));
    }
  }
}

void* deleteCMD(int clientFd, int fileNum){
  char fileName[BUFFER_LEN];
  int cond;

  snprintf(fileName,BUFFER_LEN,"%d%s",fileNum,FILENAME_EXTENSION);

  cond = unlink(fileName);

  if(cond != -1)
    write(clientFd, STD_OKAY_MSG, sizeof(STD_OKAY_MSG));
  else
    write(clientFd, STD_ERROR_MSG, sizeof(STD_ERROR_MSG));
}


void* handleClient(void* vPtr){
  //  I.  Application validity check:

  //  II.  Handle client:
  //  II.A.  Get file descriptor:
  int*     intArray = (int *)vPtr;// YOUR CODE HERE
  int      clientFd = intArray[0];// YOUR CODE HERE
  int    threadNum  = intArray[1];// YOUR CODE HERE

  free(intArray);

  // YOUR CODE HERE
  printf("Thread %d starting.\n",threadNum);
  //  II.B.  Read command:
  char  buffer[BUFFER_LEN];
  char  command;
  int fileNum;
  char  text[BUFFER_LEN];
  char  text2[BUFFER_LEN];
  int   shouldContinue  = 1;

  while  (shouldContinue)
  {
    text[0] = '\0';
    text2[0]  = '\0';

    read(clientFd,buffer,BUFFER_LEN);
    printf("Thread %d received: %s\n",threadNum,buffer);
    sscanf(buffer,"%c %d \"%[^\"]\" \"%[^\"]\"",&command,&fileNum,text,text2);


    // YOUR CODE HERE
    if(command == DIR_CMD_CHAR){
      dirCMD(clientFd);
    }

    else if(command == READ_CMD_CHAR){
      readCMD(clientFd, fileNum);
    } 

      else if(command == WRITE_CMD_CHAR){
        char* textPtr = (char*) malloc(sizeof(char)*strlen(text));
        strncpy(textPtr, text, strlen(text));
        writeCMD(clientFd, fileNum, (void*) textPtr);
      }

      else if(command == SUBSTITUTE_CMD_CHAR){
        sedCMD(clientFd, fileNum, text, text2);
      }

      else if(command == DELETE_CMD_CHAR){
       deleteCMD(clientFd, fileNum);
      }

      else if(command == QUIT_CMD_CHAR){
        write(clientFd, STD_BYE_MSG, sizeof(STD_BYE_MSG));
        shouldContinue = 0;
      }
    }

      
      printf("Thread %d quitting. \n", threadNum);
      return(NULL);
  }



//  PURPOSE:  To decide a port number, either from the command line arguments
//	'argc' and 'argv[]', or by asking the user.  Returns port number.
int		getPortNum	(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Get listening socket:
  int	portNum;

  if  (argc >= 2)
    portNum	= strtol(argv[1],NULL,0);
  else
  {
    char	buffer[BUFFER_LEN];

    printf("Port number to monopolize? ");
    fgets(buffer,BUFFER_LEN,stdin);
    portNum	= strtol(buffer,NULL,0);
  }

  //  III.  Finished:  
  return(portNum);
}
void    doServer  (int    listenFd
        )
{
  //  I.  Application validity check:

  //  II.  Server clients:
  pthread_t   threadId;
  pthread_attr_t  threadAttr;
  int     threadCount = 0;


  int* twoInts;

  pthread_attr_init(&threadAttr);

    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
  while  (1)
  {
    //  YOUR CODE HERE
    int connectFd = accept(listenFd, NULL, NULL); //connect to listenFD
    twoInts = (int *)malloc(2*sizeof(int)); //malloc for 2 integers
    twoInts[0] = connectFd; //put file descriptor in one of those spaces
    twoInts[1] = threadCount++; //put threadCount then increment

    pthread_create(&threadId, &threadAttr, handleClient, (void*) twoInts); //create detached thread

  }
  pthread_attr_destroy(&threadAttr);

  //  III.  Finished:
}

//  PURPOSE:  To attempt to create and return a file-descriptor for listening
//	to the OS telling this server when a client process has connect()-ed
//	to 'port'.  Returns that file-descriptor, or 'ERROR_FD' on failure.
int		getServerFileDescriptor
				(int		port
				)
{
  //  I.  Application validity check:

  //  II.  Attempt to get socket file descriptor and bind it to 'port':
  //  II.A.  Create a socket
  int socketDescriptor = socket(AF_INET, // AF_INET domain
			        SOCK_STREAM, // Reliable TCP
			        0);

  if  (socketDescriptor < 0)
  {
    perror(THIS_PROGRAM_NAME);
    return(ERROR_FD);
  }

  //  II.B.  Attempt to bind 'socketDescriptor' to 'port':
  //  II.B.1.  We'll fill in this datastruct
  struct sockaddr_in socketInfo;

  //  II.B.2.  Fill socketInfo with 0's
  memset(&socketInfo,'\0',sizeof(socketInfo));

  //  II.B.3.  Use TCP/IP:
  socketInfo.sin_family = AF_INET;

  //  II.B.4.  Tell port in network endian with htons()
  socketInfo.sin_port = htons(port);

  //  II.B.5.  Allow machine to connect to this service
  socketInfo.sin_addr.s_addr = INADDR_ANY;

  //  II.B.6.  Try to bind socket with port and other specifications
  int status = bind(socketDescriptor, // from socket()
		    (struct sockaddr*)&socketInfo,
		    sizeof(socketInfo)
		   );

  if  (status < 0)
  {
    perror(THIS_PROGRAM_NAME);
    return(ERROR_FD);
  }

  //  II.B.6.  Set OS queue length:
  listen(socketDescriptor,5);

  //  III.  Finished:
  return(socketDescriptor);
}


int		main		(int	argc,
				 char*	argv[]
				)
{
  //  I.  Application validity check:

  //  II.  Do server:
  int 	      port	= getPortNum(argc,argv);
  int	      listenFd	= getServerFileDescriptor(port);
  int	      status	= EXIT_FAILURE;

  if  (listenFd >= 0)
  {
    doServer(listenFd);
    close(listenFd);
    status	= EXIT_SUCCESS;
  }

  //  III.  Finished:
  return(status);
}	      