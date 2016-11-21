#include "msg.h"
#include <stdio.h>
#include <stdlib.h>

/* Crea ed alloca un generico messaggio con contenuto content */

msg_t *msg_init(void *content) {
    msg_t* new_msg = malloc(sizeof(msg_t));
    new_msg -> content = content;
    new_msg -> msg_init = msg_init;
    new_msg -> msg_destroy = msg_destroy;
    new_msg -> msg_copy = msg_copy;

    return new_msg;
}

/* dealloca un messaggio */

void msg_destroy(msg_t* msg) {
    free(msg->content);
    free(msg);
}

/* Crea ed alloca un nuovo messaggi contenente il medesimo contenuto di un messaggio dato */

msg_t *msg_copy(msg_t* msg) {
    return msg_init(msg->content);
}
