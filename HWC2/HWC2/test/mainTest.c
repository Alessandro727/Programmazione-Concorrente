#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bufferTest.c"
#include "/usr/local/opt/cunit/include/CUnit/Basic.h"

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
    
    /*QUINTA SUITE */
    pSuite = CU_add_suite("Consumazione concorrente di molteplici messaggi da un buffer pieno", init_buffer_pieno, clean_buffer_pieno);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_consumazione_concorrente_buffer_pieno_bloccante))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_consumazione_concorrente_buffer_pieno_non_bloccante))) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    /*SESTA SUITE */
    pSuite = CU_add_suite("Consumazioni e produzioni concorrenti di molteplici messaggi in un buffer", init_buffer_vuoto02, clean_buffer_vuoto02);
    if(NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if((NULL == CU_add_test(pSuite, "Test put_bloccante()", test_produttori_consumatori_molteplici_messaggi_buffer_bloccante))||
       (NULL == CU_add_test(pSuite, "Test put_non_bloccante()", test_produttori_consumatori_molteplici_messaggi_buffer_non_bloccante))) {
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