/**
 *  \file    lists.h
 *  \brief   Lists handling.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           List handling functions.
 *
 *  \author  Thomas Nemeth
 *
 *  \version 1.0.0
 *  \date    Fri, May 14 2010
 */


#ifndef LISTS_H
#define LISTS_H

typedef struct t_list_element {
        void                  * data;
        struct t_list_element * prev;
        struct t_list_element * next;
} list_element;


typedef struct {
        list_element * head;
        list_element * tail;
        int            count;
} list_head;


void list_init(list_head * list);
void list_add(list_head * list, void * data);
void list_ins(list_head * list, void * data);
void list_del(list_head * list, void * data);

#endif /* LISTS_H */

