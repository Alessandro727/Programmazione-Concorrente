#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "/usr/local/opt/cunit/include/CUnit/Basic.h"
#include "buffer.h"

static buffer_t* buffer_unitario_vuoto = NULL;
static buffer_t* buffer_unitario_pieno = NULL;
static buffer_t* buffer_test = NULL;
static buffer_t* buffer_vuoto = NULL;
static char messaggio_test[] = "prova";

/* ALLOCATORI E DEALLOCATORI */

int init_buffer_test(void) {
    buffer_test = buffer_init(1);
    return 0;
}

int clean_buffer_test(void) {
    buffer_destroy(buffer_test);
    return 0;
}

int init_buffer_unitario_vuoto(void) {
    buffer_unitario_vuoto = buffer_init(1);
    if(buffer_unitario_vuoto != NULL)
        return 0;
    else
        return -1;
}

int clean_buffer_unitario_vuoto(void) {
    buffer_destroy(buffer_unitario_vuoto);
    return 0;
}

int init_buffer_unitario_pieno(void) {
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

int clean_buffer_unitario_pieno(void) {
    buffer_destroy(buffer_unitario_pieno);
    return 0;
}

int init_buffer_vuoto(void) {
    buffer_vuoto = buffer_init(3);
    if(buffer_vuoto != NULL)
        return 0;
    else
        return -1;
}

int clean_buffer_vuoto(void) {
    buffer_destroy(buffer_vuoto);
    return 0;
}


/* TEST DELLA PRIMA SUITE */

/* Test produzione di un messaggio in un buffer unitario vuoto VERSIONE BLOCCANTE */

void test_produzione_bloccante_buffer_unitario_vuoto(void) {
    msg_t* messaggio_aspettato = msg_init(messaggio_test);
    msg_t* messaggio_ritornato = put_bloccante(buffer_unitario_vuoto, messaggio_aspettato);
    CU_ASSERT_PTR_NOT_EQUAL(messaggio_ritornato, messaggio_aspettato);
    CU_ASSERT_STRING_EQUAL(messaggio_ritornato->content, messaggio_aspettato->content);
    msg_destroy(messaggio_aspettato);
    msg_destroy(messaggio_ritornato);
}

/* Test produzione di un messaggio in un buffer unitario vuoto VERSIONE NON BLOCCANTE */

void test_produzione_non_bloccante_buffer_unitario_vuoto(void) {
    clean_buffer_unitario_vuoto();
    init_buffer_unitario_vuoto();
    msg_t* messaggio_aspettato = msg_init(messaggio_test);
    msg_t* messaggio_ritornato = put_non_bloccante(buffer_unitario_vuoto, messaggio_aspettato);
    CU_ASSERT_PTR_NOT_EQUAL(messaggio_ritornato, messaggio_aspettato);
    CU_ASSERT_STRING_EQUAL(messaggio_ritornato->content, messaggio_aspettato->content);
    msg_destroy(messaggio_aspettato);
    msg_destroy(messaggio_ritornato);
}


/* TEST DELLA SECONDA SUITE CON I METODI DI SUPPORTO */


/* Proxy per lanciare il thread */

void do_put_bloccante(msg_t* messaggio_inserito) {
    put_bloccante(buffer_unitario_pieno, messaggio_inserito);
}

/* Test produzione di un messaggio in un buffer unitario pieno VERSIONE BLOCCANTE */

void test_produzione_bloccante_buffer_unitario_pieno(void) {
    msg_t* messaggio_inserito = msg_init("pippo");
    pthread_t prod;
    pthread_create(&prod, NULL, (void*)&do_put_bloccante, messaggio_inserito);
    sleep(5);
    CU_ASSERT_EQUAL(buffer_unitario_pieno->k, buffer_unitario_pieno->size);
    CU_ASSERT_PTR_NOT_EQUAL(messaggio_inserito->content, buffer_unitario_pieno->queue[0].content);
    CU_ASSERT(pthread_kill(prod, NULL) == 0);
    msg_destroy(messaggio_inserito);
}

/* Test produzione di un messaggio in un buffer unitario pieno VERSIONE NON BLOCCANTE */

void test_produzione_non_bloccante_buffer_unitario_pieno(void) {
    clean_buffer_unitario_pieno();
    init_buffer_unitario_pieno();
    msg_t* messaggio_inserito = msg_init("pippo");
    msg_t* messaggio_ritornato = put_non_bloccante(buffer_unitario_pieno, messaggio_inserito);
    CU_ASSERT_EQUAL(buffer_unitario_pieno->k, buffer_unitario_pieno->size);
    CU_ASSERT_PTR_NOT_EQUAL(messaggio_inserito, messaggio_ritornato);
    CU_ASSERT_PTR_EQUAL(messaggio_ritornato, BUFFER_ERROR);
    msg_destroy(messaggio_ritornato);
    msg_destroy(messaggio_inserito);
}


/* TEST DELLA TERZA SUITE CON I METODI DI SUPPORTO */


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
    msg_t* messaggio_inserito = msg_init(messaggio_test);
    pthread_t cons;
    pthread_t prod;
    // Faccio la get_bloccante
    pthread_create(&cons, NULL, &do_get_bloccante_01, messaggio_inserito);
    sleep(2);
    CU_ASSERT_EQUAL(buffer_test->k, 0);
    pthread_create(&prod, NULL, &do_put_bloccante_01, messaggio_inserito);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    CU_ASSERT_EQUAL(buffer_test->k, 0);
    msg_destroy(messaggio_inserito);
}

/* Test consumazione e produzione di un messaggio da un buffer unitario; prima il consumatore VERSIONE NON BLOCCANTE */

void test_consumazione_produzione_non_bloccante_buffer_unitario_vuoto(void) {
    init_buffer_test();
    pthread_t prod, cons;
    msg_t* messaggio_inserito = msg_init(messaggio_test);
    pthread_create(&cons, NULL, &do_get_non_bloccante_01, NULL);
    pthread_create(&prod, NULL, &do_put_non_bloccante_01, messaggio_inserito);
    pthread_join(cons, NULL);
    pthread_join(prod, NULL);
    CU_ASSERT_EQUAL(buffer_test->k, 1);
    msg_destroy(messaggio_inserito);
}


/* TEST DELLA QUARTA SUITE CON I METODI DI SUPPORTO */

/* Ho supposto un buffer che può ospitare al più 3 messaggi */
/* Test produzione concorrente di molteplici messaggi in un buffer unitario vuoto; il buffer si satura in corso VERSIONE BLOCCANTE */

void do_put_bloccante02(msg_t* messaggio) {
    msg_t* messaggio_ritornato = put_bloccante(buffer_vuoto, messaggio);
    pthread_exit(messaggio_ritornato);
}

void do_put_non_bloccante02(msg_t* messaggio) {
    msg_t* messaggio_ritornato = put_non_bloccante(buffer_vuoto, messaggio);
    pthread_exit(messaggio_ritornato);
}

void test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_bloccante(void) {
    pthread_t t1,t2,t3,t4;
    msg_t* m1 = msg_init("test1");
    msg_t* m2 = msg_init("test2");
    msg_t* m3 = msg_init("test3");
    msg_t* m4 = msg_init("test4");
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
    CU_ASSERT(pthread_kill(t4, NULL) == 0);
    msg_destroy(m1);
    msg_destroy(m2);
    msg_destroy(m3);
    msg_destroy(m4);
    msg_destroy(r1);
    msg_destroy(r2);
    msg_destroy(r3);
}

void test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_non_bloccante(void) {
    clean_buffer_vuoto();
    init_buffer_vuoto();
    pthread_t t1,t2,t3,t4;
    msg_t* m1 = msg_init("test1");
    msg_t* m2 = msg_init("test2");
    msg_t* m3 = msg_init("test3");
    msg_t* m4 = msg_init("test4");
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
    msg_destroy(m1);
    msg_destroy(m2);
    msg_destroy(m3);
    msg_destroy(m4);
    msg_destroy(r1);
    msg_destroy(r2);
    msg_destroy(r3);
    msg_destroy(r4);
}


int main() {
    /* inizializzazione del registro test */
    CU_pSuite pSuite = NULL;
    
    if(CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    
    /* PRIMA SUITE */
    pSuite = CU_add_suite("Produzione di un solo messaggio in un buffer vuoto", init_buffer_unitario_vuoto, clean_buffer_unitario_vuoto);
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
    pSuite = CU_add_suite("Produzione in un buffer pieno", init_buffer_unitario_pieno, clean_buffer_unitario_pieno);
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
    pSuite = CU_add_suite("Consumazione e produzione concorrente di un messaggio da un buffer unitario; prima il consumatore", init_buffer_test, clean_buffer_test);
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
    pSuite = CU_add_suite("Produzione concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso", init_buffer_vuoto, clean_buffer_vuoto);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_bloccante))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_produzione_concorrente_molteplici_messaggi_buffer_vuoto_non_bloccante))) {
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
