/*
 * Copyright (c) 2010 Rafael Sanchez rsfuente@it.uc3m.es
 *
 * ircc.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "socket.h"
#include "commands.h"
/*#include "irc.h"*/

#define STDIN 0

const char * TCP = "tcp";

const size_t MAX_SIZE = 512;
const size_t MAX_BUFFER = 4096;
const size_t LOOP_TIMEOUT_SECS   = 10;
const long LOOP_TIMEOUT_MICROSECS  = 0;

const char * DEFAULT_NICK_NAME    = "pepito";
const char * DEFAULT_USER_NAME    = "Rafa";
const char * DEFAULT_HOST_NAME    = "host";
const char * DEFAULT_SERVER_NAME  = "rafaelsanchez.irc.dyndns.net";
const char * DEFAULT_REAL_NAME    = "Rafa";


/**
 * @brief main function
 * Example: ./irc -p 6667 -s localhost -u rafaelsf80 -n rafa -h localhost -r Rafael
 * @param argc
 * @param argv -p port -s server -u username -n nickname -h hostname -r realname
 * @return int
 */
int
main(int argc, char **argv) {
  int c;
	int port;
	int sockfd;
	ssize_t numbytes;
	char *servername;
	char *username;
	char *nickname;
	char *hostname;
	char *realname;
	char *irc_command;
	char *buf;
	char *first;
	char *active_channel;


	servername = (char *)malloc(MAX_SIZE);
	username = (char *)malloc(MAX_SIZE);
	nickname = (char *)malloc(MAX_SIZE);
	hostname = (char *)malloc(MAX_SIZE);
	realname = (char *)malloc(MAX_SIZE);
	irc_command = (char *)malloc(MAX_SIZE);
	buf = (char *)malloc(MAX_SIZE);
	first = (char *)malloc(MAX_SIZE);
	active_channel = (char *)malloc(MAX_SIZE);

	/* set Eclipse CDT console */
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	/* params parsing */
	opterr = 0;
	while ((c = getopt (argc, argv, "p:s:u:n:h:r:")) != -1)
         switch (c) {
           case 'p':
             port = atoi(optarg);
             break;
           case 's':
        	 servername = optarg;
        	 break;
           case 'u':
             username = optarg;
             break;
           case 'n':
             nickname = optarg;
             break;
           case 'h':
             hostname = optarg;
             break;
		   case 'r':
             realname = optarg;
             break;
           case '?':
             /*if ((optopt == 't') || (optopt == 's') || (optopt == 'u') ||
                 (optopt == 'n') || (optopt == 'h') || (optopt == 'r'))
               printf (stderr, "Option -%c requires an argument.\n", optopt);
             else if (isprint (optopt))
               printf (stderr, "Unknown option `-%c'.\n", optopt);
             else
               printf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);*/
             return 1;
           default:
             abort ();
           }

	printf("Hola\n");
	sockfd = active_socket("localhost", "6667", TCP);

	irc_command = auth_command("eralsaz", "rafa", "pepe", "rafaelsf80.irc.net", ":Rafa");

	send_to_server(sockfd, irc_command);

	printf("Connection to IRC server successful.\n");

	/* Welcome message and commands **/
	printf("***/connect <host> <port>   - connect to a server\n***     /nick <nick>             - send your nick to the server\
*** /user <nick> <full name> - send your auth to the server\n\
***     /list                    - list the channels in the server\n\
***     /who                     - list users in your channel\n\
***     /join [<channel>]        - join a channel or leave the channel\n\
***                                if no arg\n\
***     /msg <txt>               - send <txt> to the channel\n\
***     /disconnect              - disconnect from server\n\
***     /quit                    - close program\n\
***     /echo <txt>              - echoes <txt> back (debugging)\n\
***     /nop                     - no operation, do nothing (debugging)\n\
***     /sleep [<secs>]          - pause for <secs> seconds (debugging)\n\
***     /help                    - show this text\n");



	/* MAIN LOOP */
	struct timeval tv;
	tv.tv_sec = LOOP_TIMEOUT_SECS;
	tv.tv_usec = LOOP_TIMEOUT_MICROSECS;

	do {

		memset(buf, 0, MAX_SIZE);

		printf("\nEnter IRC command:\n");
		int max_fds = 0;

		fd_set read_fds;
		FD_ZERO(&read_fds);
		FD_SET(STDIN, &read_fds);
		FD_SET(sockfd, &read_fds);
		max_fds = sockfd;

	    int retval;
	    if ((retval = select(max_fds+1, &read_fds, NULL, NULL, &tv)) < 0) {
	    	/*debug("select");*/
	    	perror("select");
	    	exit(EXIT_FAILURE);
	    }

	    /* select's timeout expire: reset timer */
	    if (retval == 0) {
	         /*         debug("main_loop(): timeout expire!\n");*/
	         tv.tv_sec = LOOP_TIMEOUT_SECS;
	         tv.tv_usec = LOOP_TIMEOUT_MICROSECS;
	         continue;
	    }

       /* user input */
       if (FD_ISSET(STDIN, &read_fds)) {

	         int bytes_read;

	         bytes_read = read(STDIN, (void*)buf, MAX_BUFFER);
	         printf("TECLADO: %s\n", buf);
	         strncpy(first, buf, 1);


	         if (strstr(first, "/") != NULL) {
	        	 /* es comando IRC */
	        	 printf("comando1: %s\n", buf);
	        	 strcpy(irc_command, buf+1);

	         }
	         else {
	        	 /* es mensaje al canal */
	        	 printf("comando2: %s\n", buf);
	        	 strcpy(irc_command, "PRIVMSG #");
	        	 strcat(irc_command, "ro2");
	        	 strcat(irc_command, " ");
	        	 strcat(irc_command, buf);
	         }

	         printf("A ENVIAR: %s\n", irc_command);
	         if (send(sockfd, irc_command, strlen(irc_command),0) < 0)
	         {
	         	perror("send");
	         	/*close(sockfd);*/
	         	exit(EXIT_FAILURE);
	         }
	         else {
	         	printf("[ENV] %s\n", buf);
	         		         }
       } else {
		   if (FD_ISSET(sockfd, &read_fds)) {
			   if ((numbytes = recv(sockfd, buf, MAX_BUFFER, 0)) == -1)
			   {
				 perror("recv");
				 exit(EXIT_FAILURE);
			   }
			   buf[numbytes] = '\0';
			   printf("numbytes: %d [RECV] %s\n", numbytes, buf);
			   if (numbytes == 0) {
				   printf("Server closed connection. numbytes es 0. Salgo -->");
				   exit(1);
			   }

		  }
       }

	  /* process line: build a command from the line and execute it */






		/*if ((numbytes = recv(sockfd, buf, MAX_BUFFER, 0)) == -1)
		{
			perror("recv");
			exit(1);
		}*/
		/*buf[numbytes] = '\0';
		printf("[RECV] %s\n", buf);*/

		/*printf("\nEnter IRC command:\n");*/
		/*fgets(comando, MAX_SIZE, stdin);
		 fgets also returns '\r\n'
		printf("Comando introducido: %s Longitud:%d\n", comando, strlen(comando));
		comando[strlen(comando)] = '\0';*/


	} while ((strcmp(buf, "exit") != 0) && (strcmp(buf, "EXIT") != 0));
	return 0;
}

int
send_to_server(int sockfd, char * irc_message) {

	int nbytes;
	if ((nbytes=send(sockfd, irc_message, strlen(irc_message),0)) < 0)
		{
			perror("send_to_server");
			/*close(sockfd);*/
			exit(EXIT_FAILURE);
		}
	return nbytes;
}
