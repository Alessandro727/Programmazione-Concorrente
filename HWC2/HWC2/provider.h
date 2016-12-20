//
//  provider.h
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef provider_h
#define provider_h

#include <stdio.h>
#include "msg.h"
#include "provider_buffer.h"
#include "poison_pill.h"
#include <pthread.h>

void* create_msg_sequence(void*);

#endif /* provider_h */
