/**
 *  \file    main.h
 *  \brief   Main definitions.
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


#ifndef MAIN_H
#define MAIN_H

#include "lists.h"
#include "structures.h"


#define DEFAULT_PORT            2233
#define DEFAULT_CONFIG_FILE     "cyberspace.rc"


extern list_head        clients;
extern system_config    sysconfig;


int config_load(const char * config_file);
int config_save(const char * config_file);


#endif /* MAIN_H */

