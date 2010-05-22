/**
 *  \file    objects.h
 *  \brief   Objects manipulations.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           This file contains the exported functions used to manipulate
 *           (create) objects.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Sat, May 22 2010
 */


#ifndef OBJECTS_H
#define OBJECTS_H

#include "structures.h"

void object_create_star(char * parameters);
void object_create_planet(char * parameters);
void object_create_station(char * parameters);
void object_create_gate(char * parameters);
void object_create_ship(char * parameters, client * player);

#endif /* OBJECTS_H */

