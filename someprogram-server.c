/*
 * af_unix daemon for checking the license time and evaluation period for users
 * this is and independent program that is gonna make a IPC with someprogram
 * and someprogram is gonna run if connection to this programm is succesful , evaluation
 * period is the situation that is gonna count for the correct behavior of this program 
 * eduardo.ruiz@hypersec.co.uk
 */
#include <sys/types.h>
#include <setjmp.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define VSOCK "/tmp/becksocket "

int
main (void)
{
  pid_t pid ;
  int fd, len, remoto, fromlen;
  static int counter = 0;
  char *temp = (char *) malloc (32);
  jmp_buf aqui;
  char *pidnum = (char *) malloc (32);
  struct stat *test;
  struct sockaddr_un server, client;
  memset(pidnum,0x0,32);
  memset(temp,0x0,32);
  printf ("Initializing vsomeprogram communication daemon\n");
  /*
  if ((stat ("/opt/someprogram/", &test)) < 0)
    {
      printf ("/opt/someprogram directory does not exists ...\n");
      
      if (getuid() != 0) { fprintf(stderr,"You need root privileges to run this daemon\n"); exit(-1); }
      if ((mkdir ("/opt/someprogram", 0700)) == -1)
	{
	  perror ("mkdir");
	  exit (-1);
	}
	
      printf ("Directory created\n");
    
    } */
#ifdef DEBUG
  printf("pid number saved!\n");
#endif
   if (fork () == 0)
   {
	  pid = (pid_t)getpid();
	  sprintf (pidnum, "%d", pid);
	  setjmp (aqui);
	  counter++;
	  sprintf (temp, " %d\n", counter);
	  printf("creating socket\n");
	  if ((fd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
	    {
	      perror ("socket");
	      exit (-1);
	    }
	  server.sun_family = AF_UNIX;
	  strcpy (server.sun_path, VSOCK);
	  unlink (VSOCK);
	  len = sizeof (server.sun_family) + strlen (server.sun_path);
	  printf("binding socket\n");
	  //system("rm -f /tmp/becksocket");
	  if (bind (fd, (struct sockaddr *)&server, len) < 0)
	    {
	     perror ("server: bind");
	     exit (1);
	   __asm__("nop");
	    }
	    printf("listening socket..\n");
	  if (listen (fd, 5) < 0)
	    {
	      perror ("listen");
	      exit (-1);
	    }
	    printf("accepting connection\n");
	  if ((remoto = accept (fd, (struct sockaddr *)&client, &fromlen)) < 0)
	    {
	      perror ("accept");
	      exit (-1);
	    }
	    printf("sending temp\n");
	  send (remoto, temp , strlen(temp),0);
	    printf("sending pidnum\n");
	  send (remoto, pidnum, strlen (pidnum), 0);
	    printf("shutdown remote connection\n");
	  shutdown (remoto, SHUT_RDWR);
	    printf("shutdown local file descriptor\n");
	  shutdown (fd, SHUT_RDWR);
	  close(fd);
	  close(remoto);
	  longjmp (aqui, 0);
    }
  return -1;
}
