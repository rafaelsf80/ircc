/*
 * Copyright (c) 2010 eralsaz rsfuente@it.uc3m.es
 *
 * ircc.h
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

#ifndef IRCC_H_
#define IRCC_H_

extern const char * TCP;

extern const size_t MAX_SIZE;
extern const size_t MAX_BUFFER;
extern const size_t LOOP_TIMEOUT_SECS;
extern const long LOOP_TIMEOUT_MICROSECS;

extern const char * DEFAULT_NICK_NAME;
extern const char * DEFAULT_USER_NAME;
extern const char * DEFAULT_HOST_NAME;
extern const char * DEFAULT_SERVER_NAME;
extern const char * DEFAULT_REAL_NAME;

#define STDIN 0

#endif /* IRCC_H_ */
