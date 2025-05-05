#pragma once

#include <stdlib.h>
#define BLOB const void*
#define RAW_MEMORY_OFFSET char*

// O element size poderia ser POR node, ai além de genérica ela seria um container que suporta, simultaneamente, varios tipos
typedef struct GenericDBLLNode {
   void* data;
   struct GenericDBLLNode* prev;
   struct GenericDBLLNode* next;
   struct GenericDoubleLinkedList* parent; 
} llnode_t, *pllnode_t;

typedef const llnode_t* pcllnode_t;

typedef struct GenericDoubleLinkedList {
   size_t element_size;
   int element_count;
   pllnode_t head;
   pllnode_t tail;
} gll_t, *pgll_t;


/*
   MAIN FUNCTIONS
*/

// Cria uma double linked list genérica
gll_t gll_create(size_t element_size);
// Adiciona e retorna elemento ao final da dbll genérica
pllnode_t gll_append(pgll_t gll, BLOB element);
// Adiciona e retorna elemento ao incio da dbll genérica
pllnode_t gll_prepend(pgll_t gll, BLOB element);
// Adiciona e retorna elemento no indice *at* da dbll
pllnode_t gll_insert(pgll_t gll, int at, BLOB element);
// Adiciona e retorna um elemento após *node*
pllnode_t gll_insert_after(pllnode_t node, BLOB element);
// Adiciona e retorna um elemento before *node*
pllnode_t gll_insert_before(pllnode_t node, BLOB element);
// Remove e retorna ultimo elemento da dbll
pllnode_t gll_pop(pgll_t gll);
// Remove e libera a memoria do ultimo elemento da dbll
void gll_popf(pgll_t gll);
// Remove e retorna elemento no inicio da dbll
pllnode_t gll_drop(pgll_t gll);
// Remove e libera a memoria do primeiro elemento da dbll
void gll_dropf(pgll_t gll);
// Remove e retorna elemento no indice *at* da dbll
pllnode_t gll_remove(pgll_t gll, int at);
// Remove e libera a memoria de um elemento no indice *at*
void gll_removef(pgll_t gll, int at);
// Remove um node (modifica o node e o parent in-place.)
void gll_detach(pllnode_t node);
// Retorna node no indice *at* da dbll
pllnode_t gll_get(pgll_t gll, int at);
// Define o valor armazenado pelo node no indice *at* da dbll
pllnode_t gll_set(pgll_t gll, int at, BLOB element);
// Encontra e retorna o indice da primeira ocorrencia de element na dbll
int gll_find(pgll_t gll, BLOB element);
// Encontra todas as ocorrencias de element na dbll e retorna uma nova gll com os indices das ocorrencias na gll original
gll_t gll_find_all(pgll_t gll, BLOB element);
// Encontra a primeira ocorrência de uma sequencia de elementos na dbll
int gll_search(pgll_t gll, pgll_t target);
// Encontra todas as ocorrencias de uma sequencia de elementos na dbll 
gll_t gll_search_all(pgll_t gll, pgll_t target);

// HELPERS

/*
   Cria um novo node. Os parâmetros prev e next são passados como ponteiros para constante porque não
   está no escopo da função gll_node_create alterar os valores dentro destes nodes. Isso deve ser realizado
   no/pelo caller.

   A função recebe um parâmetro parent para que possa definir, obviamente, o parent do node mas também para acessar
   a informação de tamanho do elemento. Isso cria o constraint de que um node não pode ser orfão.
*/
 pllnode_t gll_node_create(BLOB data, pcllnode_t  prev, pcllnode_t next, pgll_t parent);
// Define o valor de Next de um Node
void gll_node_setNext(pllnode_t node, pcllnode_t next);
// Define o valor de Prev de um Node
void gll_node_setPrev(pllnode_t node, pcllnode_t prev);
// Libera a memoria do node
void gll_node_free(pllnode_t node);
// Identifica se, a partir do node atual, todos os caracteres correspondem a sequencia em target.
int gll_identify_sequence_match(pcllnode_t starting_node, pgll_t target);