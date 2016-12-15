//
//  reader_buffer.c
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "reader_buffer.h"
#include <stdlib.h>

reader_buffer_t* reader_buffer_init(int size) {
    reader_buffer_t* new_reader_buffer = malloc(sizeof(reader_buffer_t));
    new_reader_buffer->reader_buffer = buffer_init(size);
    return new_reader_buffer;
}

void reader_buffer_destroy(reader_buffer_t* buffer) {
    buffer_destroy(buffer->reader_buffer);
    free(buffer);
}

msg_t* reader_buffer_insert(reader_buffer_t* buffer, msg_t* msg) {
    msg_t* result = put_non_bloccante(buffer->reader_buffer, msg);
    return result;
}

/* inserisce poison_pill con la put bloccante */
void reader_buffer_insert_bloccante(reader_buffer_t* buffer) {
    msg_t* result = put_bloccante(buffer->reader_buffer, POISON_PILL);
    free(result);
}

msg_t* reader_buffer_read(reader_buffer_t* buffer) {
    msg_t* result = get_non_bloccante(buffer->reader_buffer);
    return result;
}

