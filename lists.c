/**
 *  \file    lists.c
 *  \brief   Lists handling.
 *
 *           Project: cybersystem (Cyberspace solar system emulation software).
 *
 *           Lists handling functions.
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

#include <xmem.h>

#include "0config.h"
#include "version.h"
#include "structures.h"
#include "lists.h"


void list_init(list_head * list)
{
        list->head = NULL;
        list->tail = NULL;
        list->count = 0;
}


void list_add(list_head * list, void * data)
{
        list_element * element = xmalloc(sizeof(list_element));

        element->prev = list->tail;
        element->data = data;
        element->next = NULL;

        if (list->tail)
        {
                list->tail->next = element;
                list->tail = element;
        }
        else
        {
                list->head = element;
                list->tail = element;
        }
        list->count++;
}


void list_ins(list_head * list, void * data)
{
        list_element * element = xmalloc(sizeof(list_element));

        element->prev = list->tail;
        element->data = data;
        element->next = NULL;

        if (list->head)
        {
                list->head->prev = element;
                list->head = element;
        }
        else
        {
                list->head = element;
                list->tail = element;
        }
        list->count++;
}


void list_del(list_head * list, void * data)
{
        list_element * ptr = list->head;
        int            found = 0;

        while (ptr)
        {
                if (ptr->data == data)
                {
                        found = 1;
                        break;
                }
                ptr = ptr->next;
        }

        if (! found)
        {
                return;
        }

        if (list->count > 1)
        {
                if (ptr == list->head)
                {
                        list->head = list->head->next;
                        list->head->prev = NULL;
                }
                else if (ptr == list->tail)
                {
                        list->tail = list->tail->prev;
                        list->tail->next = NULL;
                }
        }
        else
        {
                list->head = NULL;
                list->tail = NULL;
        }
        free(ptr);

        list->count--;
}

