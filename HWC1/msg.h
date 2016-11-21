#ifndef HWC1_MSG_H
#define HWC1_MSG_H

typedef struct msg {
    void *content;
    struct msg * (*msg_init)(void*); //creazione msg
    void (*msg_destroy)(struct msg *); //deallocazione msg
    struct msg * (*msg_copy)(struct msg *); //copia del msg
}msg_t;

msg_t *msg_init(void*);

void msg_destroy(msg_t*);

msg_t *msg_copy(msg_t*);

#endif //HWC1_MSG_H
