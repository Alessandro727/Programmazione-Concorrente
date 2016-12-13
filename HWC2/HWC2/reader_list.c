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
    pthread_mutex_lock(&(reader_list_mutex));
    int dim = size(reader_list);
    pthread_mutex_unlock(&(reader_list_mutex));
    return dim;
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

/* valido sia per messaggi normali che per POISON_PILL */
void reader_list_insert_broadcast(msg_t* message) {
    pthread_mutex_lock(&(reader_list_mutex));
    iterator = iterator_init(reader_list);
    while(hasNext(iterator)) {
        reader_t* reader = (reader_t*)next(iterator);
        reader_buffer_insert(reader->reader_buffer, message);
    }
    pthread_mutex_unlock(&(reader_list_mutex));
}

reader_t* reader_list_analyze() {
    pthread_mutex_lock(&(reader_list_mutex));
    double treshold = treshold_calculate();
    iterator = iterator_init(reader_list);
    while(hasNext(iterator)) {
        reader_t* reader = (reader_t*)next(iterator);
        if(reader->processing_time > treshold) {
            pthread_mutex_unlock(&(reader_list_mutex));
            return reader;
        }
    }
    iterator_destroy(iterator);
    pthread_mutex_unlock(&(reader_list_mutex));
    return NULL;
}

/* funzione di supporto sul calcolo della soglia del tempo di processamento */
double treshold_calculate() {
    iterator = iterator_init(reader_list);
    int sum_proc_time = 0;
    while(hasNext(iterator)) {
        reader_t* temp = (reader_t*)next(iterator);
        sum_proc_time += temp->processing_time;
    }
    iterator_destroy(iterator);
    int n = reader_list_size();
    double treshold = sum_proc_time/n;
    return treshold;
}




