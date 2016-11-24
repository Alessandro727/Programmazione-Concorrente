#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "/usr/local/opt/cunit/include/CUnit/Basic.h"
#include "buffer.h"

static buffer_t* buffer_unitario_vuoto = NULL;
static buffer_t* buffer_unitario_pieno = NULL;
static char messaggio_test[] = "prova";

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

/* Test produzione di un messaggio in un buffer unitario pieno VERSIONE BLOCCANTE */

void test_produzione_bloccante_buffer_unitario_pieno(void) {
    msg_t* messaggio_inserito = msg_init("pippo");
    put_bloccante(buffer_unitario_pieno, messaggio_inserito);
    CU_ASSERT_EQUAL(buffer_unitario_pieno->k, buffer_unitario_pieno->size);
    CU_ASSERT_PTR_NOT_EQUAL(messaggio_inserito->content, buffer_unitario_pieno->queue[0].content);
    msg_destroy(messaggio_inserito);
    sleep(1);
    pthread_exit(NULL);
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
