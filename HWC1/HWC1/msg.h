//
//  msg.h
//  HWC1
//
//  Created by Marco Faretra on 23/11/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef msg_h
#define msg_h

typedef struct message {
    void *content;
    struct message * (*msg_init)(void*); //creazione msg
    void (*msg_destroy)(struct message *); //deallocazione msg
    struct message * (*msg_copy)(struct message *); //copia del msg
}msg_t;

msg_t *msg_init(void*);

void msg_destroy(msg_t*);

msg_t *msg_copy(msg_t*);

#endif /* msg_h */
