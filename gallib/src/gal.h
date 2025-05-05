#pragma once

#include <stdlib.h>

typedef char* RAW_MEMORY_OFFSET;
typedef void (*gml_printer_t) (void* element); // Definição de alias p function type.


/*
To-Do: Reimplementar isso só que ao invés de tratar void* como um ponteiro, tratar como um tipo de dados agnostico (e fazer o casting para o tipo concreto quando necessario)
Vai exigir bastante pointer arithmetics, mas é interessante.
*/
typedef struct GenericArrayList {
    void* elements;
    size_t element_size;
    int element_count;
    int element_cap;
} gal_t, *pgal_t;


// Operations

// Cria uma generic array list com capacidade para `element_count` elementos de `element_size` bytes cada.
gal_t gal_create(size_t element_size, int element_count);
// Insere `element` no final da lista.
pgal_t gal_append(pgal_t gal, void* element);
// Obtem o item encontrado no índice `idx`.
void* gal_getn(pgal_t gal, int idx);
// Insere `element` em `gal` na posição `idx` ou na ultima posição, caso idx > element_count;
pgal_t gal_insert_at(pgal_t gal, int idx, void* element);
// Remove o ultimo elemento da array
void* gal_pop(pgal_t gal);
// Define o valor de um elemento em determinado indice
pgal_t gal_setn(pgal_t gal, int idx, void* element);
// Remove o valor de determinado indice
pgal_t gal_remove_at(pgal_t gal, int idx);
// Retorna o indice da primeira ocorrência do elemento element. A busca é realizada via naive search (mn).
int gal_find(pgal_t gal, void* element);
// Retorna uma nova lista gal_t com todos os as ocorrências de element em gal. A busca é realizada via naive search (mn).
gal_t gal_find_all(pgal_t gal, void* element);
// Retorna o indice do primeiro elemento da primeira ocorrência de uma sequência. A busca é realizada via naive search (mn).
int gal_search(pgal_t gal, pgal_t seq);
// Retorna uma nova lista gal_t com o indice onde se inicia todas as ocorrências da sequência enviada. A busca é realizada via naive search (mn).
gal_t gal_search_all(pgal_t gal, pgal_t seq);


// Helpers
int __gal_requires_resize(pgal_t gal);
pgal_t __gal_expand(pgal_t gal);
int __gal_elementwise_cmp(void* set, void* subset, int subset_count, size_t element_size);


// DEFAULT PRINTERS

static void __attribute__((unused)) gml_print_int(void* item) { printf("%d", *(int*)item); } 
static void __attribute__((unused)) gml_print_float(void* item) { printf("%f", *(float*)item); }
static void __attribute__((unused)) gml_print_char(void* item)  { printf("%c", *(char*)item); }

// GALMHL - GAL Macro Helper Lib

// Retorna um ponteiro para o ultimo item de T, onde T é uma generic array list.
#define _GAL_P_LAST_ITEM_OFFSET(T) (RAW_MEMORY_OFFSET)(T)->elements + ((pgal_t)T)->element_count * ((pgal_t)T)->element_size

// Retorna um ponteiro para o i-ésimo item de T, onde T é uma generic array list.
#define _GAL_P_ELEMENT_POSITION(T, I) (void*)(((RAW_MEMORY_OFFSET)((pgal_t)T)->elements + I * ((pgal_t)T)->element_size))

#define _GAL_ENSURE_SIZE(T) \
    do {\
        if (__gal_requires_resize(gal)) {\
            __gal_expand(gal);\
        }\
    }while(0)

#define _LIDX(T) ((pgal_t)T)->element_count - 1


// GAL (Generic Array List) Macro Lib
// Tem por finalidade tornar mais genérica a utilização da biblioteca.
// Obviamente, os gal macro lib só funciona, dentro de funções.

// Versão genérica de append.
#define gml_append(__gal, __val) \
    do { \
        __typeof__(__val) __gal__tmp = (__val); \
        gal_append((__gal), &__gal__tmp); \
    } while(0)


// Versão genérica de getn
#define gml_getn(__gal, __idx, __dst, __default) \
    do {\
        void* __gotten__ = gal_getn((__gal), (__idx));\
        if (!__gotten__) {\
            *(__dst) = __default;\
        } else {\
            *(__dst) = *(__typeof__(*__dst)*)__gotten__;\
            free(__gotten__);\
        }\
    }while(0)

// Versão genérica de insert_at
#define gml_insert_at(__gal, __idx, __val)\
    do{\
        __typeof__(__val) __gal__tmp = (__val);\
        gal_insert_at((__gal), (__idx), &__gal__tmp);\
    }while(0)


// Versão genérica de setn
#define gml_setn(__gal, __idx, __val) \
    do{\
        __typeof__(__val) __gal__tmp = (__val);\
        gal_setn((__gal), (__idx), &__gal__tmp);\
    }while(0)

// Versão Generica de find
#define gml_find(__gal, __val) \
    ({\
        __typeof__(__val) __gal__tmp = (__typeof__(__val)) __val;\
        gal_find((pgal_t)__gal, &__gal__tmp);\
    })


// Versão Generica de find_all [!UNTESTED!]
#define gml_find_all(__gal, __val) \
    ({\
        __typeof__(__val) __gal__tmp = (__typeof__(__val)) __val;\
        gal_find_all((pgal_t)__gal, &__gal__tmp);\
    })
// Versão genérica de pop

/* 
    (((
    SINCERAMENTE, ESSA EXPLICAÇÃO FOI ESCRITA SÓ PARA ORGANIZAR NA MINHA CABEÇA O CODIGO Q ERA PRA
    ESCREVER. ANALISAR O RESULTADO É MUITO MAIS SALUBRE PARA O ENTENDIMENTO.
    )))

    Esse macro é um dirty-little-trick para emular um comportamento de overloading em C.
    Ele serve para selecionar qual macro será utilizado posteriormente na eventual chamada para o
    macro gml_pop. 
    _1 recebe o valor do primeiro argumento posicional passado, _2 recebe o valor do segundo argumento posicional passado
    _3 recebe o valor do terceiro argumento posicional passado e NAME recebe o valor do quarto argumento posicional passado e
    retorna NAME. Depois disso o truque está em como utilizar o dispatch. A chamada será realizada de forma semelhante a:
    _GML_POP_MACRO_DISPATCH(..., gal_pop_retrieve,<<sim, aqui fica um parametro vazio>>,gal_pop_discard). Se a chamada para pop tiver sido realizada com apenas 1 argumento
    ou seja, apenas uma referência uma instância de GenericArrayList, a chamada traduzirá finalmente para:
    _GML_POP_MACRO_DISPATCH(gal, , gal_pop_retrieve, gal_pop_discard), nesse caso, NAME será gal_pop_discard, e essa será a função chamada.
    No caso de realizarmos uma chamada com _GML_POP_MACRO_DISPATCH(gal, dst, default, gal_pop_retrieve, gal_pop_discard) o elemento NAME será gal_pop_retrieve.
*/
#define _GML_POP_MACRO_DISPATCH(_1, _2, _3, NAME, ...) NAME

#define __gml_pop_discard(__gal) \
    do {\
        void* __gmlpd_popped = gal_pop(__gal);\
        free(__gmlpd_popped);\
    }while(0)

#define __gml_pop_retrieve(__gal, __dst, __default)\
    do{\
        void* __gmlpr_popped = gal_pop(__gal);\
        if(__gmlpr_popped) {\
            *(__typeof__(*__dst)*) __dst = *(__typeof__(*__dst)*) __gmlpr_popped;\
            free(__gmlpr_popped);\
        }\
    }while(0)

//                         VVVVVVVVV obtem o macro                                                 vvvvvvvvv Invoca o macro e passa os mesmos __VA_ARGS__ pra ele.
#define gml_pop(...) _GML_POP_MACRO_DISPATCH(__VA_ARGS__, __gml_pop_retrieve, , __gml_pop_discard)(__VA_ARGS__)

// Mostra a array em stdout de modo formatado.
#define gml_printarr(__gal, __printer_func) \
    do {\
        printf("[");\
        for(int i = 0; i < (__gal)->element_count; i++) {\
            void* el = _GAL_P_ELEMENT_POSITION(__gal, i);\
            if (i != 0) {\
                printf(", ");\
            }\
            __printer_func(el);\
        }\
        printf("]\n");\
    }while(0)


