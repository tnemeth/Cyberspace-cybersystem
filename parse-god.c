/**
 *  \file    parse-god.c
 *  \brief   Administrator requests parsing.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           File description here...
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Fri, May 14 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include <errors.h>
#include <sockets.h>
#include <packets.h>
#include <tags.h>

#include "0config.h"
#include "version.h"
#include "structures.h"
#include "lists.h"
#include "parse.h"
#include "main.h"


static int god_set_parameter(client * c, unsigned char * buffer)
{
        switch (buffer[3])
        {
                case 't': sysconfig.time_speed = buffer[4];     break;
                case 'b': sysconfig.backup_delay = buffer[4];   break;
                case 'm': sysconfig.max_clients = buffer[4];    break;
                case 'r': strncpy(sysconfig.restrict_ip,
                                  (char *)&buffer[4], LEN_IPADDR);
                          break;
                default:  return 0;
        }
        return 1;
}


static int god_get_parameter(client * c, unsigned char * buffer)
{
        unsigned char data[MAX_DATA_SIZE] = {0};
        unsigned char packet[MAX_PACKET_SIZE] = {0};

        switch (buffer[3])
        {
                case 't': data[0] = sysconfig.time_speed;     break;
                case 'b': data[0] = sysconfig.backup_delay;   break;
                case 'm': data[0] = sysconfig.max_clients;    break;
                case 'r': strncpy((char *)data, sysconfig.restrict_ip,
                                  LEN_IPADDR);
                          break;
                default:  return 0;
        }

        packet_create(PACKET_MSG_ACK, data, LEN_IPADDR, packet);
        packet_send(c->socket_tcp, packet);

        return 2;
}


static int god_add_object(client * c, unsigned char * buffer)
{
        switch (buffer[3])
        {
                case 'S': break; /* star */
                case 'p': break; /* planet */
                case 's': break; /* station */
                case 'g': break; /* gate */
                case 'a': break; /* asteroid */
                default:  return 0;
        }
        return 1;
}


static int god_del_object(client * c, unsigned char * buffer)
{
        switch (buffer[3])
        {
                case 'S': break; /* star */
                case 'p': break; /* planet */
                case 's': break; /* station */
                case 'g': break; /* gate */
                case 'a': break; /* asteroid */
                default:  return 0;
        }
        return 1;
}


static int god_load_config(client * c, unsigned char * buffer)
{
        return config_load((char *) &buffer[3]);
}


static int god_save_config(client * c, unsigned char * buffer)
{
        return config_save((char *) &buffer[3]);
}


static cmd_table god_cmds[] = {
        {CMD_SET_PARAM,         god_set_parameter},
        {CMD_GET_PARAM,         god_get_parameter},
        {CMD_ADD_OBJECT,        god_add_object},
        {CMD_DEL_OBJECT,        god_del_object},
        {CMD_LOAD_CONFIG,       god_load_config},
        {CMD_SAVE_CONFIG,       god_save_config},
        {0x00,                  NULL}
};


void parse_god_requests(client * c)
{
        if (parse_request(c, god_cmds) == 0)
        {
                close(c->socket_tcp);
                list_del(&clients, c);
                free(c);
                return;
        }
}
