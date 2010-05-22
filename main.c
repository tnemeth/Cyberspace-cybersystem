/**
 *  \file    main.c
 *  \brief   Main program.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           This file contains the program's entry point and basic
 *           connections management functions.
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
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>

#include <cyberspace.h>

#include "0config.h"
#include "version.h"
#include "structures.h"
#include "main.h"
#include "lists.h"
#include "parse.h"
#include "parse-requests.h"


#define FD_SETSOCK(sock)                        \
        FD_SET(sock, &rfds);                    \
        if (sock > last_sock)                   \
        {                                       \
                last_sock = sock;               \
        }


list_head       clients;        /* connected clients */
list_head       orbiters;       /* stars / planets / stations */
list_head       fixed;          /* asteroids / gates */
list_head       ships;          /* spaceships */
system_config   sysconfig;


int config_load(const char * config_file)
{
        return STATUS_OK;
}


int config_save(const char * config_file)
{
        return STATUS_OK;
}


static void init_server(void)
{
        list_init(&clients);
        list_init(&orbiters);
        list_init(&fixed);
        list_init(&ships);

        sysconfig.time_speed = 1;
        sysconfig.backup_delay = 10;
        sysconfig.max_clients = 100;
        strcpy(sysconfig.restrict_ip, "127.0.0.1");

        config_load(DEFAULT_CONFIG_FILE);

        signal(SIGPIPE, SIG_IGN);
}


static void accept_client(int socket_listen)
{
        int           socket_service = accept_connection(socket_listen, 0);
        int           nb_read;
        unsigned char buffer[MAX_PACKET_SIZE] = {0};
        char          ip_addr[LEN_IPADDR] = {0};
        client      * client_info;

        if (socket_service < 0)
        {
                return;
        }

        nb_read = packet_read(socket_service, buffer, MAX_PACKET_SIZE);
        if (nb_read < 0)
        {
                close(socket_service);
                return;
        }

        if ((sysconfig.max_clients > 0) && (clients.count >= sysconfig.max_clients))
        {
                trace(DBG_CONN, "Too much clients connected: access denied.\n");
                close(socket_service);
                return;
        }

        socket_remote_ip(socket_service, ip_addr, LEN_IPADDR);
        if (((buffer[2]) != client_ship) && (strcmp(ip_addr, sysconfig.restrict_ip) != 0))
        {
                trace(DBG_CONN, "Access restricted: access denied.\n");
                close(socket_service);
                return;
        }

        client_info = xmalloc(sizeof(client));
        client_info->socket_tcp  = socket_service;
        client_info->user        = buffer[2];
        client_info->remote_port = socket_remote_port(socket_service);
        client_info->local_port  = socket_local_port(socket_service);
        client_info->data        = NULL;
        strncpy(client_info->name, (char *) &buffer[3], LEN_NAME);
        strncpy(client_info->remote_ipaddr, ip_addr, LEN_IPADDR);

        trace(DBG_CONN, "Client information:\n"
                        "  name:        %s\n"
                        "  type:        %d\n"
                        "  from:        %s:%d\n",
                        client_info->name, client_info->user,
                        client_info->remote_ipaddr, client_info->remote_port);

        list_add(&clients, client_info);
        message_send(socket_service, PACKET_MSG_ACK, 1);
}


static void parse_requests(client * client)
{
        switch (client->user)
        {
                case client_god:   parse_god_requests(client); break;
                case client_probe: break;
                case client_ship:  break;
        }
}


int main(void)
{
        int socket_listen;

        init_server();
        socket_listen = install_server(DEFAULT_PORT, NULL, NULL);

        while (1)
        {
                fd_set          rfds;
                int             retval;
                int             last_sock = -1;
                list_element  * cx = clients.head;

                FD_ZERO(&rfds);
                FD_SETSOCK(socket_listen);
                while (cx)
                {
                        client * c = (client *) cx->data;
                        FD_SETSOCK(c->socket_tcp);
                        cx = cx->next;
                }

                retval = select(last_sock + 1, &rfds, NULL, NULL, NULL);
                if (retval <= 0)
                {
                        trace(DBG_CONN, "Connection error.\n");
                        continue;
                }
                if (FD_ISSET(socket_listen, &rfds))
                {
                        trace(DBG_CONN, "Accepting new client.\n");
                        accept_client(socket_listen);
                }
                else
                {
                        cx = clients.head;
                        while (cx)
                        {
                                client * c = (client *)cx->data;
                                if (FD_ISSET(c->socket_tcp, &rfds))
                                {
                                        trace(DBG_RQST, "Request from %s.\n", c->name);
                                        parse_requests(c);
                                        break;
                                }
                                cx = cx->next;
                        }
                }
        }
}

