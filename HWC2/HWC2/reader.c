//
//  reader.c
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "reader.h"
#include "reader_list.h"
#include <stdlib.h>
#include <unistd.h>


reader_t* reader_init(int size, int delay, char* name) {
    reader_t* new_reader = malloc(sizeof(reader_t));
    new_reader->name = name;
    new_reader->processing_time = delay;
    new_reader->reader_buffer = reader_buffer_init(size);
    new_reader->check = 0;
    return new_reader;
}

void reader_destroy(reader_t* reader) {
    if(reader_list_removeReader(reader)) {
        reader_buffer_destroy(reader->reader_buffer);
        free(reader);
    }
}

void* reader_consume(reader_t* reader) {
    msg_t* temp;
    do {
        temp = reader_buffer_read(reader->reader_buffer);
        sleep(reader->processing_time);
        printf("%s ha letto il messaggio %d\n", reader->name, (int)temp->content);
    }while(temp!=POISON_PILL);
    // Rimuovi dalla lista dei reader questo reader
    reader_destroy(reader);
    pthread_exit(NULL);
}

