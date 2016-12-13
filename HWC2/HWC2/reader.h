//
//  reader.h
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef reader_h
#define reader_h

#include <stdio.h>
#include <pthread.h>
#include "reader_buffer.h"
#include "poison_pill.h"
#include "reader_list.h"

typedef struct {
    char* name;
    int processing_time;
    reader_buffer_t* reader_buffer;
}reader_t;

reader_t* reader_init(int, int, char*);
void reader_destroy(reader_t*);
void* reader_consume(reader_t*);

#endif /* reader_h */
