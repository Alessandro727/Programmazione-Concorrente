#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "buffer.h"
#include "provider.h"
#include "dispatcher.h"
#include "accepter.h"
#include "/usr/local/opt/cunit/include/CUnit/Basic.h"

static msg_t* m1 = NULL;
static msg_t* m2 = NULL;
static msg_t* m3 = NULL; 
static msg_t* m4 = NULL; 
static buffer_t* buffer_unitario_vuoto = NULL;
static buffer_t* buffer_unitario_pieno = NULL;
static buffer_t* buffer_test = NULL;
static buffer_t* buffer_vuoto = NULL;
static buffer_t* buffer_pieno = NULL;
static buffer_t* buffer_vuoto02 = NULL;
static char messaggio_test[] = "prova";
static char* nameReader = NULL;
static args* arguments = NULL;
static reader_t* reader = NULL;
static reader_t* reader2 = NULL;
static int check_provider_buffer = 0 ;
static pthread_t reader_thread1,reader_thread2, infinity_insert_providerbuffer;


/* INIZIO TEST BUFFER SUITE 1 */

int init_suite_buffer_unitario_vuoto(void) {
    m1 = msg_init(messaggio_test);
    buffer_unitario_vuoto = buffer_init(1);
    if(buffer_unitario_vuoto != NULL)
        return 0;
    else
        return -1;
}

int clean_suite_buffer_unitario_vuoto(void) {
    msg_destroy(m1);
    buffer_destroy(buffer_unitario_vuoto);
    return 0;
}

/* Test produzione di un messaggio in un buffer unitario vuoto VERSIONE BLOCCANTE */

void test_produzione_bloccante_buffer_unitario_vuoto(void) {
    msg_t* messaggio_ritornato = put_bloccante(buffer_unitario_vuoto, m1);
    CU_ASSERT_STRING_EQUAL(buffer_unitario_vuoto->queue[0].content, m1->content);
    msg_destroy(messaggio_ritornato);
}

/* Test produzione di un messaggio in un buffer unitario vuoto VERSIONE NON BLOCCANTE */

void test_produzione_non_bloccante_buffer_unitario_vuoto(void) {
    clean_suite_buffer_unitario_vuoto();
    init_suite_buffer_unitario_vuoto();
    msg_t* messaggio_ritornato = put_non_bloccante(buffer_unitario_vuoto, m1);
    CU_ASSERT_STRING_EQUAL(buffer_unitario_vuoto->queue[0].content, m1->content);
    msg_destroy(messaggio_ritornato);
}


/* INIZIO TEST BUFFER SUITE 2 */

int init_suite_buffer_unitario_pieno(void) {
    m1 = msg_init("pippo");
    buffer_unitario_pieno = buffer_init(1);
    if(buffer_unitario_pieno != NULL) {
        msg_t* msg_prova = msg_init(messaggio_test);
        buffer_unitario_pieno->queue[buffer_unitario_pieno->D] = *msg_prova;
        buffer_unitario_pieno->D = (buffer_unitario_pieno->D)%(buffer_unitario_pieno->size);
        buffer_unitario_pieno->k = 1;
        return 0;
    }
    else
        return -1;
}

int clean_suite_buffer_unitario_pieno(void) {
    msg_destroy(m1);
    buffer_destroy(buffer_unitario_pieno);
    return 0;
}

/* Proxy per lanciare il thread */

void do_put_bloccante(msg_t* messaggio_inserito) {
    put_bloccante(buffer_unitario_pieno, messaggio_inserito);
}

/* Test produzione di un messaggio in un buffer unitario pieno VERSIONE BLOCCANTE */

void test_produzione_bloccante_buffer_unitario_pieno(void) {
    pthread_t prod;
    pthread_create(&prod, NULL, (void*)&do_put_bloccante, m1);
    sleep(5);
    CU_ASSERT_PTR_NOT_EQUAL(m1->content, buffer_unitario_pieno->queue[0].content);
    pthread_kill(prod, NULL);
}

/* Test produzione di un messaggio in un buffer unitario pieno VERSIONE NON BLOCCANTE */

void test_produzione_non_bloccante_buffer_unitario_pieno(void) {
    clean_suite_buffer_unitario_pieno();
    init_suite_buffer_unitario_pieno();
    msg_t* messaggio_ritornato = put_non_bloccante(buffer_unitario_pieno, m1);
    CU_ASSERT_PTR_EQUAL(messaggio_ritornato, BUFFER_ERROR);
    msg_destroy(messaggio_ritornato);
}

/* INIZIO TEST BUFFER SUITE 3 */

int init_suite_buffer_test(void) {
    m1 = msg_init(messaggio_test);
    buffer_test = buffer_init(1);
    return 0;
}

int clean_suite_buffer_test(void) {
    msg_destroy(m1);
    buffer_destroy(buffer_test);
    return 0;
}

/* Proxy per lanciare il thread di get_bloccante */
void do_get_bloccante_01(msg_t* messaggio_inserito) {
    msg_t* messaggio_estratto = get_bloccante(buffer_test);
    CU_ASSERT_EQUAL(buffer_test->k, 0);
    CU_ASSERT_EQUAL(messaggio_estratto->content, messaggio_inserito->content);
    msg_destroy(messaggio_estratto);
}

/* proxy per la put */
void do_put_bloccante_01(msg_t* messaggio) {
    put_bloccante(buffer_test, messaggio);
}

void do_get_non_bloccante_01(void) {
    msg_t* messaggio_estratto = get_non_bloccante(buffer_test);
    CU_ASSERT_EQUAL(messaggio_estratto, BUFFER_ERROR);
    msg_destroy(messaggio_estratto);
}

void do_put_non_bloccante_01(msg_t* messaggio_inserito) {
    msg_t* ritorno_put = put_bloccante(buffer_test, messaggio_inserito);
    CU_ASSERT_EQUAL(messaggio_inserito->content, ritorno_put->content);
    msg_destroy(ritorno_put);
}

/* Test consumazione e produzione di un messaggio da un buffer unitario; prima il consumatore VERSIONE BLOCCANTE */

void test_consumazione_produzione_bloccante_buffer_unitario_vuoto(void) {
    pthread_t cons;
    pthread_t prod;
    // Faccio la get_bloccante
    pthread_create(&cons, NULL, &do_get_bloccante_01, m1);
    sleep(2);
    CU_ASSERT_EQUAL(buffer_test->k, 0);
    pthread_create(&prod, NULL, &do_put_bloccante_01, m1);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    CU_ASSERT_EQUAL(buffer_test->k, 0);
}

/* Test consumazione e produzione di un messaggio da un buffer unitario; prima il consumatore VERSIONE NON BLOCCANTE */

void test_consumazione_produzione_non_bloccante_buffer_unitario_vuoto(void) {
    clean_suite_buffer_test();
    init_suite_buffer_test();
    pthread_t prod, cons;
    pthread_create(&cons, NULL, &do_get_non_bloccante_01, NULL);
    pthread_create(&prod, NULL, &do_put_non_bloccante_01, m1);
    pthread_join(cons, NULL);
    pthread_join(prod, NULL);
    CU_ASSERT_EQUAL(buffer_test->k, 1);
}


/* INIZIO TEST BUFFER SUITE 4 */

int init_suite_buffer_vuoto(void) {
    m1 = msg_init("test1");
    m2 = msg_init("test2");
    m3 = msg_init("test3");
    m4 = msg_init("test4");
    buffer_vuoto = buffer_init(3);
    if(buffer_vuoto != NULL)
        return 0;
    else
        return -1;
}

int clean_suite_buffer_vuoto(void) {
    msg_destroy(m1);
    msg_destroy(m2);
    msg_destroy(m3);
    msg_destroy(m4);
    buffer_destroy(buffer_vuoto);
    return 0;
}

/* Ho supposto un buffer che può ospitare al più 3 messaggi */

void do_put_bloccante02(msg_t* messaggio) {
    msg_t* messaggio_ritornato = put_bloccante(buffer_vuoto, messaggio);
    pthread_exit(messaggio_ritornato);
}

void do_put_non_bloccante02(msg_t* messaggio) {
    msg_t* messaggio_ritornato = put_non_bloccante(buffer_vuoto, messaggio);
    pthread_exit(messaggio_ritornato);
}

/* Test produzione concorrente di molteplici messaggi in un buffer unitario vuoto; il buffer si satura in corso VERSIONE BLOCCANTE */

void test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_bloccante(void) {
    pthread_t t1,t2,t3,t4;
    msg_t* r1 = NULL;
    msg_t* r2 = NULL;
    msg_t* r3 = NULL;
    pthread_create(&t1, NULL, &do_put_bloccante02, m1);
    pthread_create(&t2, NULL, &do_put_bloccante02, m2);
    pthread_create(&t3, NULL, &do_put_bloccante02, m3);
    pthread_create(&t4, NULL, &do_put_bloccante02, m4);
    sleep(5);
    CU_ASSERT_EQUAL(buffer_vuoto->k, 3);
    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_join(t3, &r3);
    CU_ASSERT_EQUAL(r1->content, m1->content);
    CU_ASSERT_EQUAL(r2->content, m2->content);
    CU_ASSERT_EQUAL(r3->content, m3->content);
    pthread_kill(t4, NULL);
    msg_destroy(r1);
    msg_destroy(r2);
    msg_destroy(r3);
}

/* Test produzione concorrente di molteplici messaggi in un buffer unitario vuoto; il buffer si satura in corso VERSIONE NON BLOCCANTE */

void test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_non_bloccante(void) {
    clean_suite_buffer_vuoto();
    init_suite_buffer_vuoto();
    pthread_t t1,t2,t3,t4;
    msg_t* r1 = NULL;
    msg_t* r2 = NULL;
    msg_t* r3 = NULL;
    msg_t* r4 = NULL;
    pthread_create(&t1, NULL, &do_put_non_bloccante02, m1);
    pthread_create(&t2, NULL, &do_put_non_bloccante02, m2);
    pthread_create(&t3, NULL, &do_put_non_bloccante02, m3);
    pthread_create(&t4, NULL, &do_put_non_bloccante02, m4);
    sleep(5);
    CU_ASSERT_EQUAL(buffer_vuoto->k, 3);
    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_join(t3, &r3);
    pthread_join(t4, &r4);
    CU_ASSERT_EQUAL(r1->content, m1->content);
    CU_ASSERT_EQUAL(r2->content, m2->content);
    CU_ASSERT_EQUAL(r3->content, m3->content);
    CU_ASSERT_EQUAL(r4, BUFFER_ERROR);
    msg_destroy(r1);
    msg_destroy(r2);
    msg_destroy(r3);
    msg_destroy(r4);
}

/* INIZIO TEST BUFFER SUITE 5 */

int init_suite_buffer_pieno(void) {
    buffer_pieno = buffer_init(3);
    m1 = msg_init("test1");
    m2 = msg_init("test2");
    m3 = msg_init("test3");
    if(buffer_pieno != NULL) {
        buffer_pieno->queue[0] = *m1;
        buffer_pieno->queue[1] = *m2;
        buffer_pieno->queue[2] = *m3;
        buffer_pieno->k = 3;
        return 0;
    }
    else
        return -1;
}

int clean_suite_buffer_pieno(void) {
    msg_destroy(m1);
    msg_destroy(m2);
    msg_destroy(m3);
    buffer_destroy(buffer_pieno);
    return 0;
}

/* Ho supposto un buffer pieno con capacità tre e contenente tre messaggi, 'test1', 'test2' e 'test3' rispettivamente */

void do_get_bloccante03(void) {
    msg_t* result = get_bloccante(buffer_pieno);
    pthread_exit(result);
}

void do_get_non_bloccante03(void) {
    msg_t* result = get_non_bloccante(buffer_pieno);
    pthread_exit(result);
}

/* Test consumazione concorrente di molteplici messaggi da un buffer pieno di capacità 3 VERSIONE BLOCCANTE */

void test_consumazione_concorrente_buffer_pieno_bloccante(void) {
    pthread_t t1,t2,t3;
    msg_t* r1 = NULL;
    msg_t* r2 = NULL;
    msg_t* r3 = NULL;
    pthread_create(&t1, NULL, &do_get_bloccante03, NULL);
    pthread_create(&t2, NULL, &do_get_bloccante03, NULL);
    pthread_create(&t3, NULL, &do_get_bloccante03, NULL);
    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_join(t3, &r3);
    CU_ASSERT_EQUAL(r1->content, "test1");
    CU_ASSERT_EQUAL(r2->content, "test2");
    CU_ASSERT_EQUAL(r3->content, "test3");
    CU_ASSERT_EQUAL(buffer_pieno->k, 0);
    msg_destroy(r1);
    msg_destroy(r2);
    msg_destroy(r3);
}

/* Test consumazione concorrente di molteplici messaggi da un buffer pieno di capacità 3 VERSIONE NON BLOCCANTE */

void test_consumazione_concorrente_buffer_pieno_non_bloccante(void) {
    clean_suite_buffer_pieno();
    init_suite_buffer_pieno();
    pthread_t t1,t2,t3;
    msg_t* r1 = NULL;
    msg_t* r2 = NULL;
    msg_t* r3 = NULL;
    pthread_create(&t1, NULL, &do_get_non_bloccante03, NULL);
    pthread_create(&t2, NULL, &do_get_non_bloccante03, NULL);
    pthread_create(&t3, NULL, &do_get_non_bloccante03, NULL);
    sleep(2);
    pthread_join(t1, &r1);
    pthread_join(t2, &r2);
    pthread_join(t3, &r3);
    CU_ASSERT_EQUAL(r1->content, "test1");
    CU_ASSERT_EQUAL(r2->content, "test2");
    CU_ASSERT_EQUAL(r3->content, "test3");
    CU_ASSERT_EQUAL(buffer_pieno->k, 0);
    msg_destroy(r1);
    msg_destroy(r2);
    msg_destroy(r3);
}

/* INIZIO TEST PROVIDER */ 

int init_suite_provider(void) {
    provider_buffer_init(5);
    arguments = malloc(sizeof(args));
    msg_t* m1 = msg_init("1");
    msg_t* m2 = msg_init("2");
    arguments->messages = malloc(sizeof(msg_t*)*2);
    arguments->size = 2;
    arguments->messages[0] = m1;
    arguments->messages[1] = m2;
    return 0;
}

int clean_suite_provider(void) {
    msg_destroy(arguments->messages[0]);
    msg_destroy(arguments->messages[1]);
    free(arguments->messages);
    free(arguments);
    provider_buffer_destroy();
    return 0;
}

void test_create_msg_sequence_provider(void) {
    pthread_t provider;
    pthread_create(&provider, NULL, create_msg_sequence, arguments);
    pthread_join(provider, NULL);
    CU_ASSERT_EQUAL(provider_buffer->queue[0].content, arguments->messages[0]->content);
    CU_ASSERT_EQUAL(provider_buffer->queue[1].content, arguments->messages[1]->content);
}


/* INIZIO TEST READER */

int init_suite_reader(void) {
    char* name = malloc(sizeof(char)*30);
    strcpy(name, "reader1");
    reader = reader_init(5, 0, name);
    reader_list_init();
    reader_list_addReader(reader);
    reader_list_insert_broadcast(msg_init("1"));
    reader_list_insert_broadcast(msg_init("2"));
    reader_list_insert_broadcast_poison_pill();
    return 0;
}

int clean_suite_reader(void) {
    reader_list_destroy();
    return 0;
}

void test_reader_consume(void) {
    pthread_t t1;
    pthread_create(&t1, NULL, reader_consume, reader);
    pthread_join(t1,NULL);
    CU_ASSERT_EQUAL(reader_list_isEmpty(), 1);
}

/* INIZIO TEST ACCEPTER */

int init_suite_accepter(void) {
    reader_list_init();
    accepter_buffer_init(5);
    pthread_t t1;
    pthread_create(&t1, NULL, accepter_buffer_insert, msg_init("1"));
    pthread_join(t1, NULL);
    nameReader = malloc(sizeof(char)*30);
    strcpy(nameReader, "reader1");
    return 0;
}

int clean_suite_accepter(void) {
    reader_list_insert_broadcast_poison_pill();
    accepter_buffer_destroy();
    reader_list_destroy();
    return 0;
}

void test_submit_request(void) {
    submitRequest(nameReader);
    CU_ASSERT_EQUAL(reader_list_size(), 1);
    accepter_buffer_insert_poison_pill();
}

/* INIZIO TEST DISPATCHER */

void helpers_function_dispatcher(void) {
    while(1) {
        if(check_provider_buffer == 1) {
            break;
        } 
        provider_buffer_insert(msg_init("1"));
    }
    pthread_exit(NULL);
}

int init_suite_dispatcher(void) {
    check_provider_buffer = 0;
    reader_list_init();
    provider_buffer_init(4);
    char* name = malloc(sizeof(char)*30);
    strcpy(name, "reader1");
    reader = reader_init(1, 0, name);
    char* name2 = malloc(sizeof(char)*30);
    strcpy(name2, "reader2");
    reader2 = reader_init(1, 7, name2);
    reader_list_addReader(reader);
    reader_list_addReader(reader2);
    pthread_create(&reader_thread1, NULL, reader_consume, reader);
    pthread_create(&reader_thread2, NULL, reader_consume, reader2);
    pthread_create(&infinity_insert_providerbuffer, NULL, helpers_function_dispatcher, NULL);
    return 0;
}

int clean_suite_dispatcher(void) {
    pthread_join(infinity_insert_providerbuffer, NULL);
    pthread_join(reader_thread1,NULL);
    pthread_join(reader_thread2, NULL);
    provider_buffer_destroy();
    reader_list_removeReader(reader);
    reader_list_destroy();
    return 0;
}

void test_dispatch_message(void) {
    pthread_t dispatcher;
    pthread_create(&dispatcher, NULL, dispatch_message, NULL);
    check_provider_buffer = 1;
    provider_buffer_insert(POISON_PILL);
    sleep(30);
    pthread_join(dispatcher, NULL);
    CU_ASSERT_EQUAL(reader_list_size(), 0);
}

void test_remove_slow_readers(void) {
    clean_suite_dispatcher();
    init_suite_dispatcher();
    pthread_t dispatcher;
    pthread_create(&dispatcher, NULL, dispatch_message, NULL);
    sleep(30);
    iterator = iterator_init(reader_list);
    while(hasNext(iterator)) {
        reader_t* reader_alive = (reader_t*)next(iterator);
        //strcmp torna 0 se le due stringhe sono uguali
        CU_ASSERT_EQUAL(strcmp(reader_alive->name, "reader1"), 0);
    }
    check_provider_buffer = 1;
    provider_buffer_insert(POISON_PILL);
    pthread_join(dispatcher, NULL);
}


int main() {
   /* inizializzazione del registro test */
    CU_pSuite pSuite = NULL;
    
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* PRIMA SUITE */
    pSuite = CU_add_suite("Produzione di un solo messaggio in un buffer vuoto", init_suite_buffer_unitario_vuoto, clean_suite_buffer_unitario_vuoto);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_produzione_bloccante_buffer_unitario_vuoto))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_produzione_non_bloccante_buffer_unitario_vuoto))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /*SECONDA SUITE */
    pSuite = CU_add_suite("Produzione in un buffer pieno", init_suite_buffer_unitario_pieno, clean_suite_buffer_unitario_pieno);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_produzione_bloccante_buffer_unitario_pieno))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_produzione_non_bloccante_buffer_unitario_pieno))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /*TERZA SUITE */
    pSuite = CU_add_suite("Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il consumatore", init_suite_buffer_test, clean_suite_buffer_test);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_consumazione_produzione_bloccante_buffer_unitario_vuoto))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_consumazione_produzione_non_bloccante_buffer_unitario_vuoto))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /*QUARTA SUITE */
    pSuite = CU_add_suite("Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso", init_suite_buffer_vuoto, clean_suite_buffer_vuoto);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_bloccante))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_non_bloccante))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /*QUINTA SUITE */
    pSuite = CU_add_suite("Consumazione concorrente di molteplici messaggi da un buffer pieno", init_suite_buffer_pieno, clean_suite_buffer_pieno);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_consumazione_concorrente_buffer_pieno_bloccante))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_consumazione_concorrente_buffer_pieno_non_bloccante))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*SESTA SUITE : PROVIDER*/
    pSuite = CU_add_suite("inserimento messaggi nel provider_buffer.", init_suite_provider, clean_suite_provider);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "create_msg_sequence(arguments)", test_create_msg_sequence_provider))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /*SETTIMA SUITE : READER*/
    pSuite = CU_add_suite("consumo messaggi da parte di un reader.", init_suite_reader, clean_suite_reader);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "reader_consume(reader)", test_reader_consume))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*OTTAVA SUITE : ACCEPTER*/
    pSuite = CU_add_suite("Creazione di un reader ed inserito nella reader_list.", init_suite_accepter, clean_suite_accepter);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "submit_request(name)", test_submit_request))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /*NONA SUITE : DISPATCHER*/
    pSuite = CU_add_suite("Dispatch di 3 messaggi all'interno di due reader_buffer con due reader che attendono di consumare.", init_suite_dispatcher, clean_suite_dispatcher);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "dispatch_message()", test_dispatch_message))||
       (NULL == CU_add_test(pSuite, "remove_slow_readers()", test_remove_slow_readers))) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* esegui tutti i test */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");
    
    /* pulisci il registro */
    CU_cleanup_registry();
    return CU_get_error();

}