//
//  poison_pill.h
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef poison_pill_h
#define poison_pill_h

#include <stdio.h>
#include "msg.h"

#define POISON_PILL ((msg_t*)&POISON_PILL_MSG)

msg_t* msg_init_pill(void*);
msg_t* msg_copy_pill(msg_t*);
void msg_destroy_pill(msg_t*);

extern const msg_t POISON_PILL_MSG;

#endif /* poison_pill_h */
