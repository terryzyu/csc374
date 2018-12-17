/*-------------------------------------------------------------------------*
 *---                 ---*
 *---   subPatClient.c            ---*
 *---                 ---*
 *---     This file defines a C program that gets file-sys commands ---*
 *--- from the user, and sends them to a server via a socket, waits ---*
 *--- for a reply, and outputs the response to the user.    ---*
 *---                 ---*
 *--- ----  ----  ----  ----  ----  ----  ----  ----  ---*
 *---                 ---*
 *--- Version 1a    2018 November 8   Joseph Phillips ---*
 *---                 ---*
 *-------------------------------------------------------------------------*/

//  Compile with:
//  $ gcc subPatClient.c -o subPatClient

//---   Header file inclusion         ---//

#include  "subPat.h"


//---   Definition of constants:        ---//

#define DEFAULT_HOSTNAME  "localhost"



//---   Definition of functions:        ---//

//  PURPOSE:  To ask the user for the name and the port of the server.  The
//  server name is returned in 'url' up to length 'urlLen'.  The port
//  number is returned in '*portPtr'.  No return value.
void  obtainUrlAndPort  (int    urlLen,
         char*    url,
         int*   portPtr
        )
{
  //  I.  Application validity check:
  if  ( (url == NULL)  ||  (portPtr == NULL) )
  {
    fprintf(stderr,"BUG: NULL ptr to obtainUrlAndPort()\n");
    exit(EXIT_FAILURE);
  }

  if   (urlLen <= 1)
  {
    fprintf(stderr,"BUG: Bad string length to obtainUrlAndPort()\n");
    exit(EXIT_FAILURE);
  }

  //  II.  Get server name and port number:
  //  II.A.  Get server name:
  printf("Machine name [%s]? ",DEFAULT_HOSTNAME);
  fgets(url,urlLen,stdin);

  char* cPtr  = strchr(url,'\n');

  if  (cPtr != NULL)
    *cPtr = '\0';

  if  (url[0] == '\0')
    strncpy(url,DEFAULT_HOSTNAME,urlLen);

  //  II.B.  Get port numbe:
  char  buffer[BUFFER_LEN];

  printf("Port number? ");
  fgets(buffer,BUFFER_LEN,stdin);

  *portPtr = strtol(buffer,NULL,10);

  //  III.  Finished:
}


//  PURPOSE:  To attempt to connect to the server named 'url' at port 'port'.
//  Returns file-descriptor of socket if successful, or '-1' otherwise.
int attemptToConnectToServer  (const char*  url,
           int    port
          )
{
  //  I.  Application validity check:
  if  (url == NULL)
  {
    fprintf(stderr,"BUG: NULL ptr to attemptToConnectToServer()\n");
    exit(EXIT_FAILURE);
  }


  //  II.  Attempt to connect to server:
  //  II.A.  Create a socket:
  int socketDescriptor = socket(AF_INET, // AF_INET domain
        SOCK_STREAM, // Reliable TCP
        0);

  //  II.B.  Ask DNS about 'url':
  struct addrinfo* hostPtr;
  int status = getaddrinfo(url,NULL,NULL,&hostPtr);

  if (status != 0)
  {
    fprintf(stderr,gai_strerror(status));
    return(-1);
  }

  //  II.C.  Attempt to connect to server:
  struct sockaddr_in server;
  // Clear server datastruct
  memset(&server, 0, sizeof(server));

  // Use TCP/IP
  server.sin_family = AF_INET;

  // Tell port # in proper network byte order
  server.sin_port = htons(port);

  // Copy connectivity info from info on server ("hostPtr")
  server.sin_addr.s_addr =
  ((struct sockaddr_in*)hostPtr->ai_addr)->sin_addr.s_addr;

  status = connect(socketDescriptor,(struct sockaddr*)&server,sizeof(server));

  if  (status < 0)
  {
    fprintf(stderr,"Could not connect %s:%d\n",url,port);
    return(-1);
  }

  //  III.  Finished:
  return(socketDescriptor);
}


//  PURPOSE:  To ask the user for a file number, and to return that number.
//  No parameters.
int   getFileNumber ()
{
  //  I.  Application validity check:

  //  II.  Get number:
  char  buffer[BUFFER_LEN];
  int choice;

  do
  {
    printf("  File number (%d-%d)? ",MIN_FILE_NUM,MAX_FILE_NUM);
    fgets(buffer,BUFFER_LEN,stdin);
    choice  = strtol(buffer,NULL,10);
  }
  while ( (choice < MIN_FILE_NUM)  ||  (choice > MAX_FILE_NUM) );

  //  III.  Finished:
  return(choice);
}


//  PURPOSE:  To remove the newline from the text pointed to by 'cPtr',
//  if there is a newline.  No return value.
void    removeNewline (char*    cPtr
        )
{
  cPtr  = strchr(cPtr,'\n');

  if  (cPtr != NULL)
    *cPtr = '\0';
}


//  PURPOSE:  To ask the user to enter text, and to get that text.  Returns
//  address of the entered text in statically allocated memory.  No
//  parameters.
const char*     getText   ()
{
  //  I.  Application validity check:

  //  II.  :
  static
  char  buffer[BUFFER_LEN-2*sizeof(int)];

  printf("  Original text? ");
  fgets(buffer,BUFFER_LEN-2*sizeof(int),stdin);
  removeNewline(buffer);

  //  III.  Finished:
  return(buffer);
}


//  PURPOSE:  To ask the user to enter new text, and to get that text.  Returns
//  address of the entered text in statically allocated memory.  No
//  parameters.
const char*     getNewText  ()
{
  //  I.  Application validity check:

  //  II.  :
  static
  char  buffer[BUFFER_LEN-2*sizeof(int)];

  printf("  New text? ");
  fgets(buffer,BUFFER_LEN-2*sizeof(int),stdin);
  removeNewline(buffer);

  //  III.  Finished:
  return(buffer);
}


//  PURPOSE:  To do the work of the application.  Gets letter from user, sends
//  it to server over file-descriptor 'socketFd', and prints returned text.
//  No return value.
void    communicateWithServer
        (int    socketFd
        )
{
  //  I.  Application validity check:

  //  II.  Do work of application:
  //  II.A.  Get letter from user:
  char  buffer[BUFFER_LEN+1];
  int shouldContinue  = 1;

  while  (shouldContinue)
  {
    int choice;
    int fileNum;
    int numBytes;

    do
    {
      printf
  ("What would you like to do:\n"
   "(1) List files\n"
   "(2) Read a file\n"
   "(3) Write a file\n"
   "(4) Substitute a pattern in a file\n"
   "(5) Delete a file\n"
   "(0) Quit\n"
   "Your choice? "
  );
      fgets(buffer,BUFFER_LEN,stdin);
      choice = strtol(buffer,NULL,10);
    }
    while  ( (choice < 0)  ||  (choice > 5) );

    switch  (choice)
    {
    case 0 :
      shouldContinue  = 0;
      snprintf(buffer,BUFFER_LEN,"%c",QUIT_CMD_CHAR);
      break;

    case 1 :
      snprintf(buffer,BUFFER_LEN,"%c",DIR_CMD_CHAR);
      break;

    case 2 :
      snprintf(buffer,BUFFER_LEN,"%c %d",
         READ_CMD_CHAR,getFileNumber()
        );
      break;

    case 3 :
      snprintf(buffer,BUFFER_LEN,"%c %d \"%s\"",
         WRITE_CMD_CHAR,getFileNumber(),getText()
        );
      break;

    case 4 :
      snprintf(buffer,BUFFER_LEN,"%c %d \"%s\" \"%s\"",
         SUBSTITUTE_CMD_CHAR,getFileNumber(),getText(),getNewText()
        );
      break;

    case 5 :
      snprintf(buffer,BUFFER_LEN,"%c %d",DELETE_CMD_CHAR,getFileNumber());
      break;

    }

    printf("Sending \"%s\"\n",buffer);
    write(socketFd,buffer,strlen(buffer)+1);
    numBytes = read (socketFd,buffer,BUFFER_LEN);

    if  (numBytes > 0)
      buffer[numBytes] = '\0';

    printf("%s\n",buffer);
  }

  //  III.  Finished:
}


//  PURPOSE:  To do the work of the client.  Ignores command line parameters.
//  Returns 'EXIT_SUCCESS' to OS on success or 'EXIT_FAILURE' otherwise.
int main  ()
{
  char    url[BUFFER_LEN];
  int   port;
  int   socketFd;

  obtainUrlAndPort(BUFFER_LEN,url,&port);
  socketFd  = attemptToConnectToServer(url,port);

  if  (socketFd < 0)
    exit(EXIT_FAILURE);

  communicateWithServer(socketFd);
  close(socketFd);
  return(EXIT_SUCCESS);
}       