//
//  accepter.c
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "accepter.h"
#include <math.h>
#include <stdlib.h>

void submitRequest(char* name) {
    msg_t* request = accepter_buffer_remove();
    if(request->content!=0) {
        int delay = (rand()%5);
        reader_t* reader = reader_init(10, delay, name);
        reader_list_addReader(reader);
        pthread_create(&reader->thread, NULL, reader_consume, reader);
    }
    else {
        pthread_exit(NULL);
    }
    free(request);
}
