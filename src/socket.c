/*
 * Copyright (c) 2010 eralsaz rsfuente@it.uc3m.es
 *
 * socket.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include "socket.h"


/**
 * @brief Performs TCP/UDP active connection
 * @param host Remote hostname
 * @param service Service type ("smtp", "80", ...)
 * @param transport Transport protocol ("tcp" or "udp")
 * @return Socket descriptor
 */
int
active_socket(const char *host, const char *service,const char *transport)
{
    struct hostent  *phe;   /* información sobre el host */
    struct servent  *pse;   /* pointer to service information entry */
    struct sockaddr_in sin; /* an Internet endpoint address         */
    int     s;              /* Descriptor de socket */

    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;

    /* Mapea servicio a puerto */

    if (( pse = getservbyname(service, transport) ))
      sin.sin_port = pse->s_port;
    else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
    	printf("servicio \"%s\" desconocido\n", service);

    /**** Mapea nombre de host a direccion IP ****/
    if (( phe = gethostbyname(host) ))
    	memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
    	printf("nombre de host \"%s\" desconocido\n", host);

    /**** Abre socket ****/
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s > 0) {
    	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
	    s = -1;
	    strerror(errno);
	}
    } else
	    s = -1;
    return s;
}


/**
 * @brief Performs TCP/UDP passive connection
 * @param service Service type ("smtp", "80", ...)
 * @param transport Transport protocol ("tcp" or "udp")
 * @param qlen Maximum incoming connections (listen() function parameter)
 * @return Socket descriptor
 */
int
passive_socket(const char *service, const char *transport, int qlen)
{
  struct servent  *pse;   /* pointer to service information entry */
  struct sockaddr_in sin; /* an Internet endpoint address         */
  int     s;              /* descriptor de socket */

  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;

  /* Mapea servicio a numero de puerto */
  if (( pse = getservbyname(service, transport) ))
    sin.sin_port = htons(ntohs((u_short)pse->s_port)+ 1000);

  else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
    printf("can't get \"%s\" service entry\n", service);

  /* Abre socket */
  s = socket(PF_INET, SOCK_STREAM, 0);
  if (s < 0)
    printf("can't create socket: %s\n", strerror(errno));

  /* Enlaza socket a un puerto y se pone a la escucha */
  if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    printf("can't bind to %s port: %s\n", service,strerror(errno));

  if (listen(s, qlen) < 0)
    printf("can't listen on %s port: %s\n", service,
	    strerror(errno));
  return s;
}

/**
 * @brief Get IP from interface, not from /etc/hosts (hostnames)
 *
 * It does not use gethostname()+gethostbyname(), but ioctl(), that
 * returns a different address than 127.0.0.1 in case of IP dynamic
 * assignment (DHCP, PPP, ...)
 * @param dirIP Returned IP address
 */
void
get_ip(char *dirIP) {

	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];

   if (getifaddrs(&ifaddr) == -1) {
	   perror("getifaddrs");
	   exit(EXIT_FAILURE);
   }

   /* Walk through linked list, maintaining head pointer so we
	  can free list later */

   for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
	   family = ifa->ifa_addr->sa_family;

	   /* Display interface name and family (including symbolic
		  form of the latter for the common families) */

	   printf("%s  address family: %d%s\n",
			   ifa->ifa_name, family,
			   (family == 17)      ? " (AF_PACKET)" :
			   (family == AF_INET) ?   " (AF_INET)" :
			   (family == AF_INET6)?  " (AF_INET6)" : "");

	   /* For an AF_INET* interface address, display the address */

	   if (family == AF_INET || family == AF_INET6) {
		   s = getnameinfo(ifa->ifa_addr,
				   (family == AF_INET) ? sizeof(struct sockaddr_in) :
										 sizeof(struct sockaddr_in6),
				   host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
		   if (s != 0) {
			   printf("getnameinfo() failed: %s\n", gai_strerror(s));
			   exit(EXIT_FAILURE);
		   }
		   printf("\taddress: <%s>\n", host);
		   strcpy(dirIP, host);
		   if ((strcmp(dirIP, "127.0.0.1") != 0) &&
		   	   (strcmp(dirIP, "0.0.0.0")   != 0)   &&
		   	   (strstr(dirIP, "169.254.")  == NULL)) {
		   		freeifaddrs(ifaddr);
		   		return;
		   	   }
	   }

	}
   freeifaddrs(ifaddr);
}
