/**
 *  \file    update.c
 *  \brief   System updating functions.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           This file contains the system updating functions.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Sat, May 29 2010
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

#include <cyberspace.h>

#include "0config.h"
#include "main.h"



static double update_time(void)
{
        struct timeval current;
        time_t         ds;
        suseconds_t    du;

        gettimeofday(&current, NULL);
        ds = current.tv_sec - sysconfig.last_update.tv_sec;
        du = current.tv_usec - sysconfig.last_update.tv_usec;

        while (du > 1000000)
        {
                du -= 1000000;
                ds += 1;
        }

        memcpy(&sysconfig.last_update, &current, sizeof(struct timeval));

        return ((double)ds + (double)du / 1000000.000) * (double)sysconfig.time_speed;
}


static void update_orbiters(double dt)
{
        list_element * c = orbiters.head;

        while (c)
        {
                object  * o = (object *)c->data;
                orbiter * p = (orbiter *)o->props;
                double    th;
                double    dth;

                if (o->type == OBJ_STAR)
                {
                        continue;
                }

                th = atan2((double)o->pos.y, (double)o->pos.x);
                dth = dt * 2 * M_PI / (p->period * SECS_IN_YEAR);

                o->pos.x = p->dist * cos(th + dth);
                o->pos.y = p->dist * sin(th + dth);
        }
}


static void send_probe_info(client * c)
{
        selection * sel = (selection *)c->data;

        switch (sel->category)
        {
                case PLANET:  break;
                case STATION: break;
                case ORBITER: break;
                case STATICS: break;
                case SHIPS:   break;
                case CLIENTS: break;
        }
}


static void update_clients(void)
{
        list_element * e = clients.head;

        while (e)
        {
                client * c = (client *)c->data;

                switch (c->user)
                {
                        case client_god:   break;
                        case client_probe: send_probe_info(c); break;
                        case client_ship:  break;
                }
        }
}


void update_system(void)
{
        double dt = update_time();

        /*
         *      Parcourir les listes et mettre à jour les éléments.
         *
         *      1. liste des planètes  - calcul des nouvelles positions
         *      2. liste des vaisseaux - calcul des nouvelles positions
         *      3. liste des vaisseaux - calcul des collisions et objets
         *                               détectables
         *      4. liste des clients   - envoi des données
         */
        trace(DBG_UPDT, "Delta T=%f\n", dt);
        update_orbiters(dt);

        update_clients();
}

