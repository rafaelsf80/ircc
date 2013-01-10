/*
 * Copyright (c) 2010 eralsaz rsfuente@it.uc3m.es
 *
 * commands.c
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

#include <string.h>

#include "commands.h"
#include "irc.h"
#include "ircc.h"

/**
 * @brief Performs TCP/UDP active connection
 * @param host Remote hostname
 * @param service Service type ("smtp", "80", ...)
 * @param transport Transport protocol ("tcp" or "udp")
 * @return Socket descriptor
 */
char *
auth_command(char * nick_name, char * user_name, char * host_name, char * server_name, char * real_name) {

  char * irc_message;

	irc_message = (char *)malloc(400);

	/* default vales */
	if (nick_name == NULL)
		strcpy(nick_name, DEFAULT_NICK_NAME);
	if (user_name == NULL)
		strcpy(user_name, DEFAULT_USER_NAME);
	if (host_name == NULL)
		strcpy(host_name, DEFAULT_HOST_NAME);
	if (server_name == NULL)
		strcpy(server_name, DEFAULT_SERVER_NAME);
	if (real_name == NULL)
		strcpy(real_name, DEFAULT_REAL_NAME);

	/* check length according to RFC */
	/* todo */
	memset(irc_message, 0, MAX_SIZE);
	strcpy(irc_message, "NICK ");
	strcat(irc_message, nick_name);
	strcat(irc_message, "\r\nUSER ");
	strcat(irc_message, user_name);
	strcat(irc_message, " ");
	strcat(irc_message, host_name);
	strcat(irc_message, " ");
	strcat(irc_message, server_name);
	strcat(irc_message, " ");
	strcat(irc_message, real_name);
	strcat(irc_message, END_OF_MESSAGE);
	printf("[SEND] %s", irc_message);

	return (char *) irc_message;
}
