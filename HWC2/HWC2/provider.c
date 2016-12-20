#include "provider.h"

void* create_msg_sequence(void* arguments) {
    args *parameters = (args*)arguments;
    int i=0;
    int size= parameters->size;
    while(i<size){
        provider_buffer_insert(parameters->messages[i]);
        i++;
    }
    provider_buffer_insert(POISON_PILL);
    pthread_exit(NULL);
}
