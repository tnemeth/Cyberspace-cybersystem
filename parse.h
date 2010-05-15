/**
 *  \file    parse.h
 *  \brief   Commands parsing.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           File description here...
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Sat, May 15 2010
 */


#ifndef PARSE_H
#define PARSE_H

#include "structures.h"

typedef struct {
        unsigned char key;
        int (* cmd)(client * c, unsigned char * buffer);
} cmd_table;


enum {
        STATUS_ERROR = 0,
        STATUS_OK,
        STATUS_REPLIED,
        STATUS_DISCONNECT
};

int parse_table(cmd_table * table, client * c, unsigned char * buf);
int parse_request(client * c, cmd_table * table);

#endif /* PARSE_H */

