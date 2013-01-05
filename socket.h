/*
 * Copyright (c) 2010 eralsaz rsfuente@it.uc3m.es
 *
 * socket.h
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

#ifndef SOCKET_H_
#define SOCKET_H_

int
active_socket(const char *host, const char *service,const char *transport);

int
passive_socket(const char *service, const char *transport, int qlen);

void
get_ip(char *dirIP);

#endif /* SOCKET_H_ */
