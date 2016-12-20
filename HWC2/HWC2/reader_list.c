//
//  reader_list.c
//  HWC2
//
//  Created by Marco Faretra on 12/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "reader_list.h"

void reader_list_init() {
    reader_list = list_init();
    pthread_mutex_init(&(reader_list_mutex), NULL);
}

void reader_list_destroy() {
    pthread_mutex_destroy(&(reader_list_mutex));
    list_destroy(reader_list);
}

int reader_list_size() {
    return size(reader_list);
}

int reader_list_isEmpty() {
    pthread_mutex_lock(&(reader_list_mutex));
    int vuota = isEmpty(reader_list);
    pthread_mutex_unlock(&(reader_list_mutex));
    return vuota;
}

void reader_list_addReader(void* reader) {
    pthread_mutex_lock(&(reader_list_mutex));
    addElement(reader_list, reader);
    pthread_mutex_unlock(&(reader_list_mutex));
}

int reader_list_removeReader(void* reader) {
    pthread_mutex_lock(&(reader_list_mutex));
    int isDeleted = removeElement(reader_list, reader);
    pthread_mutex_unlock(&(reader_list_mutex));
    return isDeleted;
}

void reader_list_insert_broadcast(msg_t* message) {
    pthread_mutex_lock(&(reader_list_mutex));
    iterator = iterator_init(reader_list);
    while(hasNext(iterator)) {
        reader_t* reader = (reader_t*)next(iterator);
        msg_t* messaggio_inserito = reader_buffer_insert(reader->reader_buffer, message);
        if(messaggio_inserito == BUFFER_ERROR) {
            reader->check = 1;
        }
    }
    iterator_destroy(iterator);
    pthread_mutex_unlock(&(reader_list_mutex));
}

void reader_list_insert_broadcast_poison_pill() {
    pthread_mutex_lock(&(reader_list_mutex));
    iterator = iterator_init(reader_list);
    while(hasNext(iterator)) {
        reader_t* reader = (reader_t*)next(iterator);
        reader_buffer_insert_bloccante(reader->reader_buffer);
    }
    iterator_destroy(iterator);
    pthread_mutex_unlock(&(reader_list_mutex));
}

void remove_slow_readers() {
    pthread_mutex_lock(&reader_list_mutex);
    iterator = iterator_init(reader_list);
    while(hasNext(iterator)) {
        reader_t* reader_temp = (reader_t*)next(iterator);
        if(reader_temp->check == 1) {
            reader_buffer_insert_bloccante(reader_temp->reader_buffer);
        }
    }
    iterator_destroy(iterator);
    pthread_mutex_unlock(&reader_list_mutex);
}

void reader_list_join_threads() {
    pthread_mutex_lock(&reader_list_mutex);
    iterator = iterator_init(reader_list);
    while(hasNext(iterator)) {
        reader_t* reader = (reader_t*)next(iterator);
        pthread_join(reader->thread, NULL);
    }
    iterator_destroy(iterator);
    pthread_mutex_unlock(&reader_list_mutex);
}



