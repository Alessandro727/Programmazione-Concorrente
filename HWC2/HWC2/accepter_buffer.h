//
//  accepter_buffer.h
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef accepter_buffer_h
#define accepter_buffer_h

#include <stdio.h>
#include "buffer.h"
#include "poison_pill.h"

buffer_t* accepter_buffer;

void accepter_buffer_init(int);
void accepter_buffer_destroy();

void* accepter_buffer_insert(void*);
msg_t* accepter_buffer_remove();
void accepter_buffer_removeAll();
void accepter_buffer_insert_poison_pill();

#endif /* accepter_buffer_h */
