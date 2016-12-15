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




void create_requests(pthread_t requester[]){
    int i;
    for(i=0;i<5;i++){
        msg_t* message=msg_init(&i);
        pthread_create(&requester[i],NULL,accepter_buffer_insert,&message);
        sleep(1);
        msg_destroy(message);
    }
}

void join_requester(pthread_t requester[]){
    int i;
    for(i=0;i<5;i++){
        pthread_join(requester[i],NULL);
    }
}

void *accepter_submiter(char* name){
    submitRequest(name);
    pthread_exit(NULL);
}

/*void accepter_start(pthread_t accepter, char[][]names){
	int i=0;
	do{
 sprintf(nome[i], "Reader %d", i);
 pthread_create(&accepter,NULL,accepter_submiter,&names[i];
 i++;
 pthread_join(accepter,NULL);
	}while(accepter_buffer->size>0);
	
 }*/

void accepter_start(pthread_t accepter){
    char* nome;
    int i=0;
    do{
        sprintf(nome, "Reader %d", i);
        pthread_create(&accepter,NULL,accepter_submiter,&nome);
        i++;
        pthread_join(accepter,NULL);
    }while(accepter_buffer->k>0);
    
}

int main(){
    
    msg_t* msg1 = msg_init("1");
    msg_t* msg2 = msg_init("2");
    msg_t* msg3 = msg_init("3");
    msg_t* msg4 = msg_init("4");
    msg_t* msg5 = msg_init("5");
    
    msg_t* messages[] = {msg1, msg2, msg3, msg4, msg5};
    
    
    pthread_t provider;
    pthread_t accepter;
    pthread_t dispatcher;
    pthread_t requester[5];
    
    
    
    printf("Inizializzazione dei buffer (dimensione=5)\n");
    provider_buffer_init(5);
    accepter_buffer_init(5);
    sleep(1);
    printf("Partenza del provider\n");
    
    pthread_create(&provider, NULL,create_msg_sequence,messages);
    sleep(1);
    printf("Invio delle richieste\n");
    create_requests(requester);
    sleep(1);
    printf("Partenza dell'accepter\n");
    accepter_start(accepter);
    sleep(1);
    
    printf("Partenza del dispatcher\n");
    pthread_create(&dispatcher,NULL,dispatch_message,NULL);
    sleep(1);
    printf("Chiusura dei flussi\n");
    pthread_join(provider,NULL);
    join_requester(requester);
    pthread_join(accepter,NULL);
    pthread_join(dispatcher,NULL);
    printf("cancellazione dei buffer\n");
    provider_buffer_destroy();
    accepter_buffer_destroy();	
    
    return 1;
}
