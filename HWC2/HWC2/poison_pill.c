//
//  poison_pill.c
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "poison_pill.h"
#include <stdio.h>

const msg_t POISON_PILL_MSG = {
    NULL,
    msg_init_pill,
    msg_destroy_pill,
    msg_copy_pill
};

msg_t* msg_init_pill(void* content) {
    return POISON_PILL;
}

msg_t* msg_copy_pill(msg_t* msg) {
    return POISON_PILL;
}

void msg_destroy_pill(msg_t* msg) {
    //do nothing
}


