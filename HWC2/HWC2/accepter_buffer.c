//
//  accepter_buffer.c
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "accepter_buffer.h"
#include <stdlib.h>

void accepter_buffer_init(int size) {
    accepter_buffer = buffer_init(size);
}

void accepter_buffer_destroy() {
    buffer_destroy(accepter_buffer);
}

void* accepter_buffer_insert(void* message) {
    msg_t* messaggio = (msg_t*)&message;
    msg_t* inserted_message = put_bloccante(accepter_buffer, messaggio);
    free(inserted_message);
    pthread_exit(NULL);
}

msg_t* accepter_buffer_remove() {
    return get_bloccante(accepter_buffer);
}

void accepter_buffer_removeAll() {
    int size = accepter_buffer->size;
    int i;
    for(i=0; i<size; i++) {
        msg_t* message = accepter_buffer_remove();
        free(message);
    }
}
