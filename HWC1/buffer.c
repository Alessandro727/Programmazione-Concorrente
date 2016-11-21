#include "buffer.h"
#include <stdlib.h>

buffer_t* buffer_init(unsigned int maxsize) {
    buffer_t* new_buffer = malloc(sizeof(buffer_t));
    new_buffer->queue = malloc(sizeof(msg_t)*maxsize);
    new_buffer->T = 0;
    new_buffer->D = 0;
    new_buffer->size = maxsize;
    new_buffer->k = 0;
    pthread_cond_init(&(new_buffer->non_vuoto), NULL);
    pthread_cond_init(&(new_buffer->non_pieno), NULL);
    pthread_mutex_init(&(new_buffer->buffer), NULL);
    pthread_mutex_init(&(new_buffer->uso_d), NULL);
    pthread_mutex_init(&(new_buffer->uso_t), NULL);

    return new_buffer;
}

void buffer_destroy(buffer_t* buffer) {
    free(buffer->queue);
    pthread_cond_destroy(&buffer->non_pieno);
    pthread_cond_destroy(&buffer->non_vuoto);
    pthread_mutex_destroy(&buffer->buffer);
    pthread_mutex_destroy(&buffer->uso_d);
    pthread_mutex_destroy(&buffer->uso_t);
    free(buffer);
}

msg_t* put_bloccante(buffer_t* buffer, msg_t* msg) {

}

msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg) {

}

msg_t* get_bloccante(buffer_t* buffer) {

}

msg_t* get_non_bloccante(buffer_t* buffer) {

}