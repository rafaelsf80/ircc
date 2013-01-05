/*
 * Copyright (c) 2010 eralsaz rsfuente@it.uc3m.es
 *
 * irc.h
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

#ifndef IRC_H_
#define IRC_H_

/* protocol constants */
const int SERVERNAME_MAX_LENGTH   = 63; /* RFC2812 1.1 */
const int NICKNAME_MAX_LENGTH     = 9;  /* RFC2812 1.2.1, longer
                                           nicknames are not supported
                                           as no future evolutions of
                                           the protocol are supported by
                                           this software */
const int    CHANNELNAME_MAX_LENGTH  = 50;  /* RFC2812 1.3 */
const int    MESSAGE_MAX_PARAMS      = 15;  /* RFC2812 2.3 */
const char   PREFIX_PREFIX           = ':'; /* RFC2812 2.3 */
const int    MESSAGE_MAX_LENGTH      = 152; /* RFC2812 2.3 */
const char   MESSAGE_SEPARATOR       = ' '; /* RFC2812 2.3 */
const char * END_OF_MESSAGE          = "\r\n"; /* RFC2812 2.3 */

#endif /* IRC_H_ */
