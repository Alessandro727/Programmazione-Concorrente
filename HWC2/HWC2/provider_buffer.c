//
//  provider_buffer.c
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "provider_buffer.h"
#include <stdlib.h>


void provider_buffer_init(int dim) {
    provider_buffer = buffer_init(dim);
}

void provider_buffer_destroy() {
    buffer_destroy(provider_buffer);
}

void provider_buffer_insert(msg_t* msg) {
    msg_t* result = put_bloccante(provider_buffer, msg);
    free(result);
}

msg_t* provider_buffer_read() {
    msg_t* result = get_non_bloccante(provider_buffer);
    return result;
}
