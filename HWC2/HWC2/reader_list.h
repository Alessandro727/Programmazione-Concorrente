//
//  reader_list.h
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef reader_list_h
#define reader_list_h

#include <stdio.h>
#include "reader.h"
#include "list.h"

typedef struct {
    reader_buffer_t* buffer;
    msg_t* msg;
}parameters;

list_t* reader_list;
iterator_t* iterator;
pthread_mutex_t reader_list_mutex;

void reader_list_init();
void reader_list_destroy();
int reader_list_size();
int reader_list_isEmpty();

void reader_list_addReader(void*);
int reader_list_removeReader(void*);
void reader_list_insert_broadcast(msg_t*);
void reader_list_insert_broadcast_poison_pill();
void remove_slow_readers();

#endif /* reader_list_h */
