//
//  dispatcher.c
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "dispatcher.h"

void* dispatch_message() {
    msg_t* message = provider_buffer_read();
    if(reader_list_isEmpty()) {
        printf("Il dispatcher ha letto il messaggio %d\n", (int)message->content);
    }
    else {
        while(!reader_list_isEmpty()) {
            if(message == POISON_PILL) {
                reader_list_insert_broadcast(POISON_PILL);
                break;
            }
            else if(message!=NULL) {
                reader_list_insert_broadcast(message);
            }
            else break;
            message = provider_buffer_read();
        }
    }
    pthread_exit(NULL);
}

void* remove_slow_readers() {
    reader_t* slow_reader = NULL;
    do {
        slow_reader = reader_list_analyze();
        if(slow_reader != NULL) {
            clean_buffer(slow_reader->reader_buffer->reader_buffer);
            reader_buffer_insert(slow_reader->reader_buffer, POISON_PILL);
        }
    }while(slow_reader!=NULL);
    pthread_exit(NULL);
}
