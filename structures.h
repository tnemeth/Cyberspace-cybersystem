/**
 *  \file    structures.h
 *  \brief   Structures definitions.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           This file contains the structures definitions for the program.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Fri, May 14 2010
 */


#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <cyberspace.h>
#include "lists.h"

typedef struct {
        char system_name[LEN_NAME];
        int backup_delay;
        int time_speed;
        int max_clients;
        char restrict_ip[LEN_IPADDR];
} system_config;


typedef struct {
        client_type user;
        char        name[LEN_NAME];
        char        remote_ipaddr[LEN_IPADDR];
        int         remote_port;
        int         local_port;
        int         socket_tcp;
        int         socket_udp;
        void      * data;
} client;


typedef struct {
        long long x;
        long long y;
        long long z;
} coords;


typedef unsigned long long dist;


typedef enum {
        OBJ_STAR,
        OBJ_PLANET,
        OBJ_STATION,
        OBJ_GATE,
        OBJ_ASTEROID,
        OBJ_SHIP
} kind;


typedef struct {
        coords          dir;
        unsigned int    value;
} force;


typedef struct {
        char            name[LEN_NAME];
        kind            type;
        coords          pos;
        force           dir;
        unsigned        radius;
        int             weak;
        void          * props;
        list_head       forces;
} object;


typedef struct {
        dist    dist;
        float   mass;
        float   period;
} orbiter;


typedef struct {
        char    dest[LEN_IPADDR];
} gate;


typedef struct {
        unsigned long   mass;
        unsigned long   fuel;
        unsigned long   money;
        client        * conn;
} ship;


#endif /* STRUCTURES_H */

