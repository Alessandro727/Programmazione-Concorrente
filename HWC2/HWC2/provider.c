//
//  provider.c
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "provider.h"

void* create_msg_sequence(msg_t* msg_array, int size) {
    int i = 0;
    for (i=0; i<size; i++) {
        provider_buffer_insert(&msg_array[i]);
    }
    provider_buffer_insert(POISON_PILL);
    pthread_exit(NULL);
}
