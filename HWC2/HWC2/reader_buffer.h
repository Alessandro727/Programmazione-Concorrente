//
//  reader_buffer.h
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef reader_buffer_h
#define reader_buffer_h

#include <stdio.h>
#include "buffer.h"

typedef struct {
    buffer_t* reader_buffer;
}reader_buffer_t;

// primitive del reader buffer
reader_buffer_t* reader_buffer_init(int);
void reader_buffer_destroy(reader_buffer_t*);
void reader_buffer_insert(reader_buffer_t*, msg_t*);
msg_t* reader_buffer_read(reader_buffer_t*);


#endif /* reader_buffer_h */
