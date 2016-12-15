//
//  buffer.h
//  HWC1
//
//  Created by Marco Faretra on 23/11/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#ifndef buffer_h
#define buffer_h
#define BUFFER_ERROR (msg_t*) NULL
#include <stdio.h>
#include "msg.h"
#include <pthread.h>

typedef struct buffer {
    msg_t* queue;
    int T; //indice per le estrazione
    int D; //indice per gli inserimenti
    int size; //dimensione del buffer
    int k; //contatore dei messaggi presenti nel buffer
    pthread_cond_t non_vuoto; //variabile condizione per indicare il buffer non vuoto
    pthread_cond_t non_pieno; //variabile condizione per indicare il buffer non pieno
    pthread_mutex_t buffer; //mutex associato alle variabili condizione
    pthread_mutex_t uso_t; //semaforo per la gestione dell'accesso all'indice t
    pthread_mutex_t uso_d; //semaforo per la gesione dell'accesso all'indice d
}buffer_t;

/* creazione di un buffer vuoto di dimensione massima nota */
buffer_t* buffer_init(unsigned int);

/* deallocazione di un buffer */
void buffer_destroy(buffer_t*);

/* inserimento bloccante, sospende se pieno, effettua l'inserimento appena si libera spazio e return il msg, msg!=NULL */
msg_t* put_bloccante(buffer_t*, msg_t*);

/* inserimento non bloccante, restituisce BUFFER_ERROR se pieno, altrimenti effettua l'inserimento e return msg, msg!=NULL */
msg_t* put_non_bloccante(buffer_t*, msg_t*);

/* estrazione bloccante, sospende se vuoto, quindi restituisce il valore estratto non appena disponibile */
msg_t* get_bloccante(buffer_t*);

/* estrazione non bloccante, restituisce BUFFER_ERROR se vuoto, ed il valore estratto in caso contrario */
msg_t* get_non_bloccante(buffer_t*);

#endif /* buffer_h */
