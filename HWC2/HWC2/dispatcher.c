//
//  dispatcher.c
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "dispatcher.h"

void* dispatch_message() {
    while(1) {
        msg_t* msg = provider_buffer_read();
        if(msg->content == 0) break;
        if(!reader_list_isEmpty()) {
            reader_list_insert_broadcast(msg);
            remove_slow_readers();
        }
    }
    if(reader_list_isEmpty())
        pthread_exit(NULL);
    else {
        reader_list_insert_broadcast_poison_pill();
        pthread_exit(NULL);
    }
}

