//
//  buffer.c
//  HWC1
//
//  Created by Marco Faretra on 23/11/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>

buffer_t* buffer_init(unsigned int maxsize) {
    buffer_t* new_buffer = malloc(sizeof(buffer_t));
    new_buffer->queue = malloc(sizeof(msg_t)*maxsize);
    new_buffer->T = 0;
    new_buffer->D = 0;
    new_buffer->k = 0;
    new_buffer->size = maxsize;
    pthread_cond_init(&(new_buffer->non_pieno), NULL);
    pthread_cond_init(&(new_buffer->non_vuoto), NULL);
    pthread_mutex_init(&(new_buffer->buffer), NULL);
    pthread_mutex_init(&(new_buffer->uso_d), NULL);
    pthread_mutex_init(&(new_buffer->uso_t), NULL);
    
    return new_buffer;
}

void buffer_destroy(buffer_t* buffer) {
    free(buffer->queue);
    pthread_cond_destroy(&(buffer->non_pieno));
    pthread_cond_destroy(&(buffer->non_vuoto));
    pthread_mutex_destroy(&buffer->buffer);
    pthread_mutex_destroy(&buffer->uso_d);
    pthread_mutex_destroy(&buffer->uso_t);
    free(buffer);
}

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg) {
    //alloco una copia del messaggio che poi andrà inserita nel buffer
    msg_t* copia = msg_copy(msg);
    
    //faccio attesa attiva finché non si libera spazio
    pthread_mutex_lock(&buffer->buffer);
    while (buffer->k == buffer->size) {
        pthread_cond_wait(&buffer->non_pieno, &buffer->buffer);
    }
    //procedo con il blocco  di uso_d per salvare il messaggio nel buffer
    pthread_mutex_lock(&(buffer->uso_d));
    buffer->queue[buffer->D] = *copia;
    buffer->D = (buffer->D+1)%(buffer->size);
    buffer->k = buffer->k +1;
    pthread_mutex_unlock(&(buffer->uso_d));
    pthread_cond_signal(&buffer->non_vuoto);
    pthread_mutex_unlock(&buffer->buffer);
    
    return copia;
}

msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg) {
    //alloco una copia del messaggio che poi andrà inserita nel buffer
    msg_t* copia = msg_copy(msg);
    
    //se il buffer è pieno ritorno BUFFER_ERROR
    pthread_mutex_lock(&buffer->buffer);
    if (buffer->k == buffer->size) {
        pthread_mutex_unlock(&buffer->buffer);
        return BUFFER_ERROR;
    }
    
    //procedo con il blocco  di uso_d per salvare il messaggio nel buffer
    pthread_mutex_lock(&(buffer->uso_d));
    buffer->queue[buffer->D] = *copia;
    buffer->D = (buffer->D+1)%(buffer->size);
    buffer->k = buffer->k +1;
    pthread_mutex_unlock(&(buffer->uso_d));
    pthread_cond_signal(&buffer->non_vuoto);
    pthread_mutex_unlock(&buffer->buffer);
    
    return copia;
}

msg_t* get_bloccante(buffer_t* buffer) {
    msg_t* msg = NULL;
    //faccio attesa attiva finché non entra almeno un msg nel buffer
    pthread_mutex_lock(&buffer->buffer);
    while(buffer->k == 0) {
        pthread_cond_wait(&buffer->non_vuoto, &buffer->buffer);
    }
    //procedo con la consumazione del messaggio
    pthread_mutex_lock(&buffer->uso_t);
    msg = &buffer->queue[buffer->T];
    buffer->T = (buffer->T+1)%(buffer->size);
    buffer->k = buffer->k - 1;
    pthread_mutex_unlock(&buffer->uso_t);
    pthread_cond_signal(&buffer->non_pieno);
    pthread_mutex_unlock(&buffer->buffer);
    
    return msg;
}

msg_t* get_non_bloccante(buffer_t* buffer) {
    msg_t* msg;
    
    //se il buffer è vuoto ritorno BUFFER_ERROR
    pthread_mutex_lock(&buffer->buffer);
    if(buffer->k == 0) {
        pthread_mutex_unlock(&buffer->buffer);
        return BUFFER_ERROR;
    }
    
    //procedo con la consumazione del messaggio
    pthread_mutex_lock(&buffer->uso_t);
    msg = &buffer->queue[buffer->T];
    buffer->T = (buffer->T+1)%(buffer->size);
    buffer->k = buffer->k - 1;
    pthread_mutex_unlock(&buffer->uso_t);
    pthread_cond_signal(&buffer->non_pieno);
    pthread_mutex_unlock(&buffer->buffer);
    
    return msg;
}
