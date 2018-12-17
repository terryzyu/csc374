/*-------------------------------------------------------------------------*
 *---									---*
 *---		argList.c						---*
 *---									---*
 *---	    This program assembles the command line arguments into a	---*
 *---	linked list.  The linked list is printed, and then free()d.	---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a				i	Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>


//  PURPOSE:  To hold a node in a linked list of strings.
struct		Node
{
  char*		namePtr_;
  struct Node*	nextPtr_;
};


//  PURPOSE:  To create and return a linked list of strings from 'argv[1]' to
//	'argv[argc-1]', or to return 'NULL' if 'argc' <= 1.
struct Node*	makeList	(int		argc,
				 char*		argv[]
				)
{
  struct Node*	list	= NULL;
  struct Node*	end	= NULL;
  struct Node* temp = NULL; //Used for updating the list
  int		i;

  for  (i = 1;  i < argc;  i++)
  {
    // YOUR CODE HERE
    temp = (struct Node*) malloc(sizeof(struct Node));
    temp -> namePtr_ = argv[i];
    temp -> nextPtr_ = NULL;
    if(list == NULL){ //swap, and if it's the first element
      list = temp;
      end = list;
    }
    else{
      end -> nextPtr_ = temp;
      end = temp;
    }

  }

  return(list);
}


//  PURPOSE:  To print the 'namePtr_' values found in 'list'.  No return value.
void		print		(const struct	Node*	list
				)
{
  const struct Node*	run;

  // YOUR CODE HERE
  run = list;
  while(run != NULL){
    printf("%s\n", run -> namePtr_);
    run = run -> nextPtr_;
  }
}


//  PURPOSE:  To do nothing if 'list' is NULL.  Otherwise to 'free()' both
//	'nextPtr_' and 'namePtr_' for 'list', and all of 'nextPtr_' successors.
//	No return value.
void		release		(struct Node*	list
				)
{
  // YOUR CODE HERE
  struct Node* temp;

  while(list != NULL){
    temp = list;
    list = list -> nextPtr_;
    free(temp);
  }
}


//  PURPOSE:  To create, print, and 'free()' a linked list of the 'argc-1'
//	items on 'argv[]', from 'argv[1]' to 'argv[argc-1]'.  Returns
//	'EXIT_SUCCESS' to OS.
int		main		(int		argc,
				 char*		argv[]
				)
{
  //  I. 

  //  II.  :
  struct Node*	list;

  list	= makeList(argc,argv);
  print(list);
  release(list);

  //  III.  Finished:
  return(EXIT_SUCCESS);
}