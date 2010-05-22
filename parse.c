/**
 *  \file    parse.c
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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include <cyberspace.h>

#include "0config.h"
#include "version.h"
#include "structures.h"
#include "parse.h"
#include "main.h"


int parse_table(cmd_table * table, client * c, unsigned char * buf)
{
        int i;
        int ok = -1;


        for (i = 0 ; table[i].key ; i++)
        {
                if ((buf[2] & table[i].key) == table[i].key)
                {
                        trace(DBG_RQST, " -> Request key: 0x%02X\n", buf[2]);
                        ok = table[i].cmd(c, buf);
                        break;
                }
        }

        return ok;
}


int parse_request(client * c, cmd_table * table)
{
        unsigned char buffer[MAX_PACKET_SIZE] = {0};
        int           nb_read;
        int           ok;

        nb_read = packet_read(c->socket_tcp, buffer, MAX_PACKET_SIZE);
        if (nb_read <= 0)
        {
                trace(DBG_CONN, "Connection closed: read error.\n");
                return 0;
        }

        ok = parse_table(table, c, buffer);

        switch (ok)
        {
                case 3:  return 0;
                case 2:  break;
                case 1:  message_send(c->socket_tcp, PACKET_MSG_ACK, 1);   break;
                case 0:  message_send(c->socket_tcp, PACKET_MSG_NACK, 1);  break;
                case -1: message_send(c->socket_tcp, PACKET_MSG_ERROR, 1); break;
        }
        trace(DBG_RQST, "Request status: %d (read %d bytes).\n", ok, nb_read);

        return 1;
}
