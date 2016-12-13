//
//  provider_buffer.h
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef provider_buffer_h
#define provider_buffer_h

#include <stdio.h>
#include "buffer.h"

buffer_t* provider_buffer;

//classiche primitive provider_buffer
void provider_buffer_init(int);
void provider_buffer_destroy();
void provider_buffer_insert(msg_t* msg);
msg_t* provider_buffer_read();

#endif /* provider_buffer_h */
