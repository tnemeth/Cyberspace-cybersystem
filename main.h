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

#include "0config.h"
#include "lists.h"
#include "structures.h"


#if DEBUG_LEVEL > 0
#  define DBG_CONN 1
#  define DBG_RQST 2
#  define DBG_PRMS 3
#  define DBG_UPDT 4
#  define trace(lvl, fmt, args...)                              \
        if (DEBUG_PART(lvl))                                    \
        {                                                       \
                printf(fmt, ## args);                           \
        }
#else
#  define trace(lvl, fmt, args...)
#endif


#define DEFAULT_PORT            2233
#define DEFAULT_CONFIG_FILE     "cyberspace.rc"
#define SECS_IN_YEAR            31558201.344


extern list_head     clients;
extern list_head     orbiters;
extern list_head     fixed;
extern list_head     ships;
extern system_config sysconfig;


int config_load(const char * config_file);
int config_save(const char * config_file);


#endif /* MAIN_H */

