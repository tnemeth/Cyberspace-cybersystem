/**
 *  \file    parse-proberequest.c
 *  \brief   Probe requests parsing.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           This file contains the functions used to parse probes clients
 *           requests.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Sun, May 30 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "0config.h"
#include "version.h"
#include "structures.h"
#include "lists.h"
#include "parse.h"
#include "main.h"
#include "objects.h"


static int probe_disconnect(client * c, unsigned char * buffer)
{
        return STATUS_DISCONNECT;
}


static int probe_set_selection(client * c, unsigned char * buffer)
{
        selection * sel;

        if (c->data)
        {
                /* For now only 1 selection per probe client. */
                return STATUS_ERROR;
        }

        sel = xmalloc(sizeof(selection));

        sel->category = buffer[3];
        sel->data     = buffer[4] | (buffer[5] << 8);
        sel->flags    = buffer[6] | (buffer[7] << 8);
        c->data = (void *)sel;
        trace(DBG_PRMS, "Selection: cat=%d / dat=%d / flg=%d\n",
                        sel->category, sel->data, sel->flags);

        return STATUS_OK;
}


static cmd_table probe_cmds[] = {
        {CMD_DISCONNECT,        probe_disconnect},
        {CMD_SET_SELECTION,     probe_set_selection},
        {0x00,                  NULL}
};


void parse_probe_requests(client * c)
{
        if (parse_request(c, probe_cmds) == 0)
        {
                close(c->socket_tcp);
                list_del(&clients, c);
                free(c);
                return;
        }
}

