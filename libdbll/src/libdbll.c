#include <memory.h>
#include <stdio.h>
#include "libdbll.h"

gll_t gll_create(size_t element_size) {
   gll_t new = {.element_size = element_size, .element_count = 0, .head = NULL, .tail = NULL};
   return new;
}


pllnode_t gll_node_create(BLOB data, pcllnode_t prev, pcllnode_t next, pgll_t parent) {
   if (!parent) return NULL;


   pllnode_t new_node = malloc(sizeof(llnode_t));

   if (!new_node) return NULL;

   new_node->parent = parent;

   size_t data_size = parent->element_size;

   new_node->data = malloc(data_size);

   if (!new_node->data) {
      free(new_node);
      return NULL;
   }

   memcpy(new_node->data, data, data_size);

   new_node->prev = (pllnode_t) prev;

   new_node->next = (pllnode_t) next;
   
   return new_node;
}

void gll_node_setNext(pllnode_t node, pcllnode_t next) {
   if (!node) return;
   node->next = (pllnode_t) next;
}

/*tested*/
void gll_node_setPrev(pllnode_t node, pcllnode_t prev) {
   if (!node) return;
   node->prev = (pllnode_t) prev;
}

/*tested*/
pllnode_t gll_append(pgll_t gll, BLOB element) {
   if (!gll) return NULL;
   
   //Cria o novo node
   pllnode_t node = gll_node_create(element, NULL, NULL, gll);

   // Verifica se há head.
   if(gll->head) {
      // lógica para adicionar ao final
      gll_node_setNext(gll->tail, node);
      gll_node_setPrev(node, gll->tail);
      gll->tail = node;
   } else {
      gll->head = node;
      gll->tail = node;
   }

   gll->element_count++;
   return node;
}

/*tested*/
pllnode_t gll_prepend(pgll_t gll, BLOB element) {
   if (!gll) return NULL;

   // Cria o novo node
   pllnode_t node = gll_node_create(element, NULL, NULL, gll);

   if(gll->head) {
      gll_node_setNext(node, gll->head);
      gll_node_setPrev(gll->head, node);
      gll->head = node;
   } else {
      gll->head = node;
      gll->tail = node;
   }

   gll->element_count++;
   return node;
}

/*tested*/
pllnode_t gll_insert(pgll_t gll, int at, BLOB element) {
   if (!gll) return NULL;
   if (at < 0 || at > gll->element_count) return NULL;
   if (at == gll->element_count) return gll_append(gll, element);

   pllnode_t current_node = gll->head;
   for(int i = 0; i < at; i++) {
      current_node = current_node->next;
   }

   pllnode_t new_node = gll_node_create(element, NULL, NULL, gll);
   if (!new_node) return NULL;

   gll_node_setNext(new_node, current_node);
   gll_node_setPrev(new_node, current_node->prev);
   gll_node_setNext(current_node->prev, new_node);
   gll_node_setPrev(current_node, new_node);

   gll->element_count++;
   return new_node;  
}

/*tested*/
pllnode_t gll_insert_after(pllnode_t node, BLOB element) {
   if (!node) return NULL;
   pllnode_t new_node = gll_node_create(element, node, node->next, node->parent);
   if (!new_node) return NULL;
   if (node->next) {
      gll_node_setPrev(node->next, new_node);
   } else {
      node->parent->tail = new_node;
   }
   gll_node_setNext(node, new_node);
   
   node->parent->element_count++;
   return new_node;
}

/*tested*/
pllnode_t gll_insert_before(pllnode_t node, BLOB element) {
   if (!node) return NULL;
   pllnode_t new_node = gll_node_create(element, node->prev, node, node->parent);
   if (!new_node) return NULL;

   if (node->prev == NULL) {
      node->parent->head = new_node;
   }

   gll_node_setNext(node->prev, new_node);
   gll_node_setPrev(node, new_node);

   node->parent->element_count++;
   return new_node;
}

/*tested*/
/*
   Lembrar que pop (e drop, e remove) não limpam os campos prev, next nem parent.
*/
pllnode_t gll_pop(pgll_t gll) {
   if (!gll) return NULL;

   gll->element_count--;
   pllnode_t current_tail = gll->tail;
   if(gll->tail == gll->head) { // Quando só há 1 elem
      gll->head = NULL;
      gll->tail = NULL;
      return current_tail;
   }

   gll->tail = gll->tail->prev;
   gll->tail->next = NULL;
   return current_tail;
}

/*tested*/
void gll_popf(pgll_t gll) {
   pllnode_t res = gll_pop(gll);
   gll_node_free(res);
}

/*tested*/
pllnode_t gll_drop(pgll_t gll) {
   if (!gll) return NULL;
   gll->element_count--;
   pllnode_t current_head = gll->head;
   if (gll->head == gll->tail) {
      gll->head = NULL;
      gll->tail = NULL;
      return current_head;
   }

   pllnode_t new_head = current_head->next;
   gll_node_setPrev(new_head, NULL);
   gll->head= new_head;
   return current_head;
}

/*untested*/
void gll_dropf(pgll_t gll) {
   pllnode_t res = gll_drop(gll);
   gll_node_free(res);
}

/*tested*/
pllnode_t gll_remove(pgll_t gll, int at) {
   if (!gll) return NULL;
   if (at < 0 || at >= gll->element_count) return NULL;
   if (at == gll->element_count-1) return gll_pop(gll);

   pllnode_t current_node = gll->head;
   for(int i = 0; i < at && current_node != NULL; i++) {
      current_node = current_node->next;
   }

   if (!current_node) return NULL;

   gll->element_count--;
   if(current_node->prev) {
      gll_node_setNext(current_node->prev, current_node->next);
   }
   
   if(current_node->next) {
      gll_node_setPrev(current_node->next, current_node->prev);
   }

   if(current_node == current_node->parent->head) {
      current_node->parent->head = current_node->next;
   }

   return current_node;
}

/*untested*/
void gll_removef(pgll_t gll, int at) {
   pllnode_t res = gll_remove(gll, at);
   gll_node_free(res);
}

/*untested*/
void gll_detach(pllnode_t node) {
   if (!node) return;

   if (node->next) {
      node->next->prev = node->prev;
   }

   if (node->prev) {
      node->prev->next = node->next;
   }

   node->prev = NULL;
   node->next = NULL;
   return;
}

/*untested*/
pllnode_t gll_get(pgll_t gll, int at) {
   if (!gll || at < 0 || at >= gll->element_count) return NULL;
   
   pllnode_t current_node = gll->head;
   for(int i = 0; i < at && current_node != NULL; i++) {
      current_node = current_node->next;
   }

   return current_node;
}

/*untested*/
pllnode_t gll_set(pgll_t gll, int at, BLOB element) {
   if (!gll || at < 0 || at >= gll->element_count) return NULL;
   pllnode_t node = gll_get(gll, at);
   if (!node) return NULL;

   memcpy(node->data, element, gll->element_size);
   return node;
}

/*tested*/
int gll_find(pgll_t gll, BLOB element) {
   if (!gll) return -1;

   pllnode_t current_node = gll->head;
   int idx = 0;

   while (current_node) {
      if(memcmp(current_node->data, element, gll->element_size) == 0) {
         return idx;
      }
      current_node = current_node->next;
      idx++;
   }

   return -1;
}

/*tested*/
gll_t gll_find_all(pgll_t gll, BLOB element) {
   gll_t occurences_indexes = gll_create(sizeof(int));
   if (!gll) return occurences_indexes;

   pllnode_t current_node = gll->head;
   int idx = 0;
   while(current_node){
      if(memcmp(current_node->data, element, gll->element_size) == 0) {
         gll_append(&occurences_indexes, &idx);
      }      
      current_node = current_node->next;
      idx++;
   }
   return occurences_indexes;
}

/*tested*/
int gll_search(pgll_t gll, pgll_t target) {
   if (!gll || !target) return -1;
   pllnode_t current_node = gll->head;
   pllnode_t target_head = target->head;
   int idx = 0;
   while(current_node) {
      if(memcmp(current_node->data, target_head->data, gll->element_size) == 0) {
         if (target->element_count == 1) {
            return idx;
         }
      
         if(gll_identify_sequence_match(current_node, target)) {
            return idx;
         } // test it.
      }

      current_node = current_node->next;
      idx++;
   }
   return -1;
}

/*tested*/
gll_t gll_search_all(pgll_t gll, pgll_t target) {
   gll_t res = gll_create(sizeof(int));

   if (!gll || !target) return res;

   pllnode_t current_node = gll->head;
   pllnode_t target_head = target->head;
   int idx = 0;

   while(current_node) {
      if(memcmp(current_node->data, target_head->data, gll->element_size) == 0) {
         if (target->element_count == 1) {
            gll_append(&res, &idx);
         } else if(gll_identify_sequence_match(current_node, target)) {
            gll_append(&res, &idx);
         } // test it.
      }

      current_node = current_node->next;
      idx++;
   }
   return res;
}


/* HELPER DEFS */
void gll_node_free(pllnode_t node) {
   if (!node) return;
   free(node->data);
   free(node);
}

int gll_identify_sequence_match(pcllnode_t starting_node, pgll_t target) {

   pllnode_t current_target_node = target->head;
   pllnode_t current_searched_node = (pllnode_t) starting_node;

   while(current_target_node) {
      if (!current_searched_node) return 0;

      if (memcmp(current_target_node->data, current_searched_node->data, target->element_size) != 0) {
         return 0;
      }
      current_target_node = current_target_node->next;
      current_searched_node = current_searched_node->next;
   }

   return 1;
}