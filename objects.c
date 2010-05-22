/**
 *  \file    objects.c
 *  \brief   Objects manipulations.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           This file contains the objects manipulation functions.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Sat, May 22 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "0config.h"
#include "version.h"
#include "structures.h"
#include "main.h"


#define MASS_STATION    1
#define RADIUS_GATE     3
#define RADIUS_SHIP     1


#define rand_upto(v) (int)((double)rand() / ((double)RAND_MAX + 1) * (v))


typedef unsigned long ulong;


static orbiter * props_create_orbiter(dist dist, float mass, float period)
{
        orbiter * o = xmalloc(sizeof(orbiter));

        o->dist = dist;
        o->mass = mass;
        o->period = period;

        return o;
}


static gate * props_create_gate(const char * destination)
{
        gate * o = xmalloc(sizeof(gate));

        snprintf(o->dest, LEN_IPADDR, destination);

        return o;
}


static ship * props_create_ship(ulong mass, ulong fuel, ulong money)
{
        ship * o = xmalloc(sizeof(ship));

        o->mass = mass;
        o->fuel = fuel;
        o->money = money;
        o->conn = NULL;

        return o;
}


static object * object_create(kind type, char * name, unsigned radius, void * props)
{
        object * o = xmalloc(sizeof(object));

        snprintf(o->name, LEN_NAME, name);
        o->type = type;

        o->pos.x = 0;
        o->pos.y = 0;
        o->pos.z = 0;

        o->dir.dir.x = 0;
        o->dir.dir.y = 0;
        o->dir.dir.z = 0;
        o->dir.value = 0;

        o->radius = radius;
        o->weak = 0;
        o->props = props;

        list_init(&o->forces);

        return o;
}


void object_create_star(char * parameters)
{
        orbiter * orb;
        object  * obj;
        char    * opts = strchr(parameters, ':');
        char    * name;
        char    * mass;
        char    * radius;

        if ((opts == NULL) || (orbiters.count > 0))
        {
                return;
        }
        opts++;

        name = strtok(opts, "/");
        mass = strtok(NULL, "/");
        radius = strtok(NULL, "/");

        orb = props_create_orbiter(0, strtod(mass, NULL), 0.0);
        obj = object_create(OBJ_STAR, name, strtoul(radius, NULL, 0), (void *)orb);

        list_add(&orbiters, obj);
}


void object_create_planet(char * parameters)
{
        orbiter * orb;
        object  * obj;
        char    * opts = strchr(parameters, ':');
        char    * name;
        char    * s_dist;
        char    * mass;
        char    * radius;
        char    * period;
        double    theta = rand_upto(360) * M_PI / 180;
        dist      dist;

        if ((opts == NULL) || (orbiters.count < 1))
        {
                return;
        }
        opts++;

        name = strtok(opts, "/");
        s_dist = strtok(NULL, "/");
        mass = strtok(NULL, "/");
        radius = strtok(NULL, "/");
        period = strtok(NULL, "/");

        dist = strtoul(s_dist, NULL, 0);
        orb = props_create_orbiter(dist, strtod(mass, NULL), strtod(period, NULL));
        obj = object_create(OBJ_PLANET, name, strtoul(radius, NULL, 0), (void *)orb);
        obj->pos.x = (int)((double)dist * cos(theta));
        obj->pos.y = (int)((double)dist * sin(theta));

        list_add(&orbiters, obj);
}


void object_create_station(char * parameters)
{
        orbiter * orb;
        object  * obj;
        char    * opts = strchr(parameters, ':');
        char    * name;
        char    * s_dist;
        char    * radius;
        char    * period;
        double    theta = rand_upto(360) * M_PI / 180;
        dist      dist;

        if ((opts == NULL) || (orbiters.count < 1))
        {
                return;
        }
        opts++;

        name = strtok(opts, "/");
        s_dist = strtok(NULL, "/");
        radius = strtok(NULL, "/");
        period = strtok(NULL, "/");

        dist = strtoul(s_dist, NULL, 0);
        orb = props_create_orbiter(dist, MASS_STATION, strtod(period, NULL));
        obj = object_create(OBJ_STATION, name, strtoul(radius, NULL, 0), (void *)orb);
        obj->pos.x = (int)((double)dist * cos(theta));
        obj->pos.y = (int)((double)dist * sin(theta));

        list_add(&orbiters, obj);
}


void object_create_gate(char * parameters)
{
        gate   * gate;
        object * obj;
        char   * opts = strchr(parameters, ':');
        char   * name;
        char   * host;
        char   * xpos;
        char   * ypos;
        char   * zpos;

        if (opts == NULL)
        {
                return;
        }
        opts++;

        name = strtok(opts, "/");
        host = strtok(NULL, "/");
        xpos = strtok(NULL, "/");
        ypos = strtok(NULL, "/");
        zpos = strtok(NULL, "/");

        gate = props_create_gate(host);
        obj = object_create(OBJ_GATE, name, RADIUS_GATE, (void *)gate);
        obj->pos.x = strtoul(xpos, NULL, 0);
        obj->pos.y = strtoul(ypos, NULL, 0);
        obj->pos.z = strtoul(zpos, NULL, 0);

        list_ins(&fixed, obj);
}


void object_create_ship(char * parameters, client * player)
{
        ship         * ship;
        object       * obj;
        char         * opts = strchr(parameters, ':');
        char         * name;
        char         * mass;
        char         * fuel;
        char         * money;
        char         * from;
        list_element * cont = fixed.head;
        object       * where;

        if (opts == NULL)
        {
                return;
        }
        opts++;

        name = strtok(opts, "/");
        mass = strtok(NULL, "/");
        fuel = strtok(NULL, "/");
        money = strtok(NULL, "/");
        from = strtok(NULL, "/");

        ship = props_create_ship(strtoul(mass, NULL, 0),
                                 strtoul(fuel, NULL, 0),
                                 strtoul(money, NULL, 0));
        ship->conn = player;
        obj = object_create(OBJ_SHIP, name, RADIUS_SHIP, (void *)ship);

        while (cont)
        {
                gate * host;

                where = (object *)cont->data;
                host  = (gate *)where->props;
                if (strcmp(host->dest, from) == 0)
                {
                        int dx, dy, dz;

                        dx = rand_upto(10);
                        dy = rand_upto(10);
                        dz = rand_upto(10);

                        obj->pos.x = where->pos.x + (rand_upto(1) == 1 ? dx : -dx);
                        obj->pos.y = where->pos.y + (rand_upto(1) == 1 ? dy : -dy);
                        obj->pos.z = where->pos.z + (rand_upto(1) == 1 ? dz : -dz);
                        break;
                }
                cont = cont->next;
        }

        if (! cont)
        {
                free(ship);
                free(obj);
                return;
        }

        list_add(&ships, obj);
}

