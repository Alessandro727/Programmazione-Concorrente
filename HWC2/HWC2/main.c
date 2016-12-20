//
//  main.c
//  HWC2
//
//  Created by Marco Faretra on 07/12/16.
//  Copyright © 2016 Marco Faretra. All rights reserved.
//

#include "accepter_buffer.h"
#include "accepter.h"
#include "buffer.h"
#include "msg.h"
#include "provider.h"
#include "provider_buffer.h"
#include "reader.h"
#include "dispatcher.h"
#include "reader.h"
#include "reader_buffer.h"
#include "reader_list.h"
#include <unistd.h>
#include <stdlib.h>


void create_requests(pthread_t requester[]){
    int i;
    for(i=0;i<4;i++){
        int* valueInsert = malloc(sizeof(int));
        *valueInsert = i;
        msg_t* message=msg_init(valueInsert);
        pthread_create(&requester[i],NULL,accepter_buffer_insert,message);
        pthread_join(requester[i], NULL);
        sleep(1);
        free(valueInsert);
        msg_destroy(message);
    }
}

void join_requester(pthread_t requester[]){
    int i;
    for(i=0;i<5;i++){
        pthread_join(requester[i],NULL);
    }
}

void accepter_start(pthread_t accepter){
    char* nome;
    int i=0;
    for(i=0; ;i++) {
        nome = malloc(sizeof(char)*30);
        sprintf(nome, "Reader %d", i);
        submitRequest(nome);
    }
}

int main(){
    
    msg_t* msg1 = msg_init("1");
    msg_t* msg2 = msg_init("2");
    msg_t* msg3 = msg_init("3");
    msg_t* msg4 = msg_init("4");
    
    pthread_t provider;
    pthread_t accepter;
    pthread_t dispatcher;
    pthread_t requester[5];
    
    args* param = malloc(sizeof(args));
    param->messages = malloc(sizeof(msg_t*)*4);
    param->size = 4;
    param->messages[0] = msg1;
    param->messages[1] = msg2;
    param->messages[2] = msg3;
    param->messages[3] = msg4;
    
    printf("Inizializzazione dei buffer (dimensione=5)\n");
    provider_buffer_init(5);
    accepter_buffer_init(5);
    reader_list_init();
    sleep(1);
    
    printf("Invio delle richieste\n");
    create_requests(requester);
    sleep(1);
    printf("Partenza dell'accepter\n");
    pthread_create(&accepter, NULL, accepter_start, NULL);
    sleep(1);
    printf("Partenza del dispatcher\n");
    pthread_create(&dispatcher,NULL,dispatch_message,NULL);
    sleep(1);
    printf("Partenza del provider\n");
    pthread_create(&provider, NULL, create_msg_sequence, param);
    sleep(1);
    
    printf("Chiusura dei flussi\n");
    pthread_join(provider,NULL);
    accepter_buffer_insert_poison_pill();
    //join_requester(requester);
    pthread_join(accepter,NULL);
    pthread_join(dispatcher,NULL);
    printf("Terminazione dei reader\n");
    // inserisco questa sleep perché altrimenti la join prende la sleep del messaggio come fine
    // dell'istruzione.
    sleep(20);
    reader_list_join_threads();
    printf("cancellazione dei buffer\n");
    free(param->messages);
    free(param);
    msg_destroy(msg1);
    msg_destroy(msg2);
    msg_destroy(msg3);
    msg_destroy(msg4);
    provider_buffer_destroy();
    accepter_buffer_destroy();
    reader_list_destroy();

    return 1;
}
