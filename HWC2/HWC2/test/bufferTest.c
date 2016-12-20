#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../buffer.h"

static buffer_t* buffer_unitario_vuoto = NULL;
static buffer_t* buffer_unitario_pieno = NULL;
static buffer_t* buffer_test = NULL;
static buffer_t* buffer_vuoto = NULL;
static buffer_t* buffer_pieno = NULL;
static buffer_t* buffer_vuoto02 = NULL;
static char messaggio_test[] = "prova";

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

int init_buffer_pieno(void) {
    buffer_pieno = buffer_init(3);
    msg_t* msg_1 = msg_init("test1");
    msg_t* msg_2 = msg_init("test2");
    msg_t* msg_3 = msg_init("test3");
    if(buffer_pieno != NULL) {
        buffer_pieno->queue[0] = *msg_1;
        buffer_pieno->queue[1] = *msg_2;
        buffer_pieno->queue[2] = *msg_3;
        buffer_pieno->k = 3;
        return 0;
    }
    else
        return -1;
}

int clean_buffer_pieno(void) {
    buffer_destroy(buffer_pieno);
    return 0;
}

int init_buffer_vuoto02(void) {
    buffer_vuoto02 = buffer_init(3);
    if(buffer_vuoto02 != NULL)
        return 0;
    else
        return -1;
}

int clean_buffer_vuoto02(void) {
    buffer_destroy(buffer_vuoto02);
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

/* Test produzione concorrente di molteplici messaggi in un buffer unitario vuoto; il buffer si satura in corso VERSIONE NON BLOCCANTE */

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



/* TEST DELLA QUINTA SUITE CON I METODI DI SUPPORTO */
/* Ho supporto un buffer pieno con capacità tre e contenente tre messaggi, 'test1', 'test2' e 'test3' rispettivamente */

void do_get_bloccante03(void) {
    msg_t* m1 = get_bloccante(buffer_pieno);
    pthread_exit(m1);
}

void do_get_non_bloccante03(void) {
    msg_t* m1 = get_non_bloccante(buffer_pieno);
    pthread_exit(m1);
}

/* Test consumazione concorrente di molteplici messaggi da un buffer pieno di capacità 3 VERSIONE BLOCCANTE */

void test_consumazione_concorrente_buffer_pieno_bloccante(void) {
    pthread_t t1,t2,t3;
    msg_t* m1 = NULL;
    msg_t* m2 = NULL;
    msg_t* m3 = NULL;
    pthread_create(&t1, NULL, &do_get_bloccante03, NULL);
    pthread_create(&t2, NULL, &do_get_bloccante03, NULL);
    pthread_create(&t3, NULL, &do_get_bloccante03, NULL);
    pthread_join(t1, &m1);
    pthread_join(t2, &m2);
    pthread_join(t3, &m3);
    CU_ASSERT_EQUAL(m1->content, "test1");
    CU_ASSERT_EQUAL(m2->content, "test2");
    CU_ASSERT_EQUAL(m3->content, "test3");
    CU_ASSERT_EQUAL(buffer_pieno->k, 0);
}

/* Test consumazione concorrente di molteplici messaggi da un buffer pieno di capacità 3 VERSIONE NON BLOCCANTE */

void test_consumazione_concorrente_buffer_pieno_non_bloccante(void) {
    clean_buffer_pieno();
    init_buffer_pieno();
    pthread_t t1,t2,t3;
    msg_t* m1 = NULL;
    msg_t* m2 = NULL;
    msg_t* m3 = NULL;
    pthread_create(&t1, NULL, &do_get_non_bloccante03, NULL);
    pthread_create(&t2, NULL, &do_get_non_bloccante03, NULL);
    pthread_create(&t3, NULL, &do_get_non_bloccante03, NULL);
    sleep(2);
    pthread_join(t1, &m1);
    pthread_join(t2, &m2);
    pthread_join(t3, &m3);
    CU_ASSERT_EQUAL(m1->content, "test1");
    CU_ASSERT_EQUAL(m2->content, "test2");
    CU_ASSERT_EQUAL(m3->content, "test3");
    CU_ASSERT_EQUAL(buffer_pieno->k, 0);
}



/* TEST DELLA SESTA SUITE CON I METODI DI SUPPORTO */
/* Ho supposto un buffer vuoto con capacità 3. */
void do_put_bloccante04(msg_t* messaggio) {
    msg_t* ritornato = put_bloccante(buffer_vuoto02, messaggio);
    pthread_exit(ritornato);
}

void do_get_bloccante04(void) {
    msg_t* ritornato = get_bloccante(buffer_vuoto02);
    pthread_exit(ritornato);
}

void do_put_non_bloccante04(msg_t* messaggio) {
    msg_t* ritornato = put_non_bloccante(buffer_vuoto02, messaggio);
    pthread_exit(ritornato);
}

void do_get_non_bloccante04(void) {
    msg_t* ritornato = get_non_bloccante(buffer_vuoto02);
    pthread_exit(ritornato);
}

/* Test consumazioni e produzioni concorrenti di molteplici messaggi in un buffer VERSIONE BLOCCANTE */
void test_produttori_consumatori_molteplici_messaggi_buffer_bloccante(void) {
    pthread_t t1,t2,t3,t4,t5,t6, t7;
    msg_t* m1 = msg_init("test1");
    msg_t* m2 = msg_init("test2");
    msg_t* m3 = msg_init("test3");
    msg_t* m4 = msg_init("test4");
    msg_t* r1 = NULL;
    msg_t* r2 = NULL;
    msg_t* r3 = NULL;
    msg_t* r4 = NULL;
    pthread_create(&t1, NULL, &do_get_bloccante04, NULL);
    pthread_create(&t2, NULL, &do_get_bloccante04, NULL);
    pthread_create(&t3, NULL, &do_put_bloccante04, m1);
    pthread_join(t3, NULL);
    pthread_join(t1, &r1);
    CU_ASSERT_EQUAL(m1->content, r1->content);
    CU_ASSERT_EQUAL(buffer_vuoto02->k, 0);
    pthread_create(&t4, NULL, &do_get_bloccante04, NULL);
    pthread_create(&t5, NULL, &do_put_bloccante04, m2);
    pthread_create(&t6, NULL, &do_put_bloccante04, m3);
    pthread_create(&t7, NULL, &do_put_bloccante04, m4);
    sleep(2);
    pthread_join(t5, NULL);
    pthread_join(t6, NULL);
    pthread_join(t7, &r4);
    pthread_join(t2, &r2);
    pthread_join(t4, &r3);
    CU_ASSERT_EQUAL(m2->content, r2->content);
    CU_ASSERT_EQUAL(m3->content, r3->content);
    CU_ASSERT_EQUAL(buffer_vuoto02->queue[buffer_vuoto02->T].content, r4->content);
    msg_destroy(m1);
    msg_destroy(m2);
    msg_destroy(m3);
    msg_destroy(m4);
}

/* Test consumazioni e produzioni concorrenti di molteplici messaggi in un buffer VERSIONE NON BLOCCANTE */
void test_produttori_consumatori_molteplici_messaggi_buffer_non_bloccante(void) {
    clean_buffer_vuoto02();
    init_buffer_vuoto02();
    pthread_t t1,t2,t3,t4,t5,t6, t7;
    msg_t* m1 = msg_init("test1");
    msg_t* m2 = msg_init("test2");
    msg_t* m3 = msg_init("test3");
    msg_t* m4 = msg_init("test4");
    msg_t* r1 = NULL;
    msg_t* r2 = NULL;
    msg_t* r3 = NULL;
    pthread_create(&t1, NULL, &do_get_non_bloccante04, NULL);
    pthread_create(&t2, NULL, &do_put_non_bloccante04, m1);
    pthread_join(t1, &r1);
    pthread_join(t2, NULL);
    CU_ASSERT_EQUAL(r1, BUFFER_ERROR);
    CU_ASSERT_EQUAL(buffer_vuoto02->k, 1);
    pthread_create(&t4, NULL, &do_get_non_bloccante04, NULL);
    pthread_create(&t5, NULL, &do_put_non_bloccante04, m2);
    pthread_create(&t6, NULL, &do_put_non_bloccante04, m3);
    pthread_create(&t7, NULL, &do_get_non_bloccante04, NULL);
    pthread_join(t4, &r2);
    pthread_join(t5, NULL);
    pthread_join(t6, NULL);
    pthread_join(t7, &r3);
    CU_ASSERT_EQUAL(m1->content, r2->content);
    CU_ASSERT_EQUAL(m2->content, r3->content);
    CU_ASSERT_EQUAL(buffer_vuoto02->queue[buffer_vuoto02->T].content, m3->content);
    msg_destroy(m1);
    msg_destroy(m2);
    msg_destroy(m3);
    msg_destroy(m4);
}
