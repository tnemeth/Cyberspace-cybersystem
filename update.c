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

#include <cyberspace.h>

#include "0config.h"
#include "main.h"


static void update_orbiters(void)
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
                dth = sysconfig.time_speed * 2 * M_PI / (p->period * SECS_IN_YEAR);

                o->pos.x = p->dist * cos(th + dth);
                o->pos.y = p->dist * sin(th + dth);
        }
}


void update_system(void)
{
        /*
         *      Parcourir les listes et mettre à jour les éléments.
         *
         *      1. liste des planètes  - calcul des nouvelles positions
         *      2. liste des vaisseaux - calcul des nouvelles positions
         *      3. liste des vaisseaux - calcul des collisions et objets
         *                               détectables
         *      4. liste des clients   - envoi des données
         */
        update_orbiters();
}

