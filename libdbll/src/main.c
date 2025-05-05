#include <stdlib.h>
#include <stdio.h>
#include "libdbll.h"

#define PRINT_DEBUG_DATA(T) printf("Element Count: %d | Element Size %zu\n", (T).element_count, (T).element_size)
#define PRINT_FULL_LIST(T)\
   do{\
      for (pllnode_t __node = (T).head; __node != NULL; __node = __node->next){\
         printf("%c\n", *((char*)(__node->data)));\
      }\
   }while(0)

#define MAP_LIST(T)\
   do{\
      for (pllnode_t __node = (T).head; __node != NULL; __node = __node->next){\
         printf("%c->", *((char*)(__node->data)));\
      }\
      printf("\n");\
   }while(0)

#define PRINT_FULL_LIST_INT(T)\
   do{\
      for (pllnode_t __node = (T).head; __node != NULL; __node = __node->next){\
         printf("%d\n", *((int*)(__node->data)));\
      }\
   }while(0)

#define MAP_LIST_INT(T)\
   do{\
      for (pllnode_t __node = (T).head; __node != NULL; __node = __node->next){\
         printf("%d->", *((int*)(__node->data)));\
      }\
      printf("\n");\
   }while(0)

int main() {
   gll_t my_ll = gll_create(sizeof(char));
   PRINT_DEBUG_DATA(my_ll);
   char chr = 'a';
   pllnode_t nd_a = gll_append(&my_ll, &chr); // IDX 1
   chr = 'x';
   pllnode_t nd_b = gll_append(&my_ll, &chr); // IDX 2
   chr = 'z';
   pllnode_t nd_c = gll_append(&my_ll, &chr); // IDX 3
   chr = 'g';
   pllnode_t nd_d = gll_prepend(&my_ll, &chr);// IDX 0
   chr = 'a';
   pllnode_t nd_h = gll_append(&my_ll, &chr);
   chr = 'z';
   pllnode_t nd_i = gll_append(&my_ll, &chr);
   chr = 'g';
   pllnode_t nd_j = gll_append(&my_ll, &chr);
   chr = 'a';
   pllnode_t nd_k = gll_append(&my_ll, &chr);
   chr = 'l';
   pllnode_t nd_l = gll_append(&my_ll, &chr);
   chr = 'z';
   pllnode_t nd_m = gll_append(&my_ll, &chr);
   chr = 'g';
   pllnode_t nd_p = gll_append(&my_ll, &chr);

   PRINT_FULL_LIST(my_ll);
   PRINT_DEBUG_DATA(my_ll);
   printf("Adding y to index 3\n");

   chr = 'y';
   pllnode_t nd_e = gll_insert(&my_ll, 3, &chr);
   
   PRINT_FULL_LIST(my_ll);
   PRINT_DEBUG_DATA(my_ll);
   
   printf("Inserting X after z\n");
   chr = 'X';
   pllnode_t nd_f =gll_insert_after(nd_c, &chr);
   
   PRINT_FULL_LIST(my_ll);
   PRINT_DEBUG_DATA(my_ll);

   printf("Inserting H before nd_f (c)\n");
   chr = 'H';
   pllnode_t nd_g = gll_insert_before(nd_d, &chr);
   PRINT_FULL_LIST(my_ll);
   PRINT_DEBUG_DATA(my_ll);
   MAP_LIST(my_ll);

   char CHR = 'a';
   printf("All Occurences of CHR (%c): \n", CHR);
   gll_t all_occurences_of_CHR = gll_find_all(&my_ll, &CHR);
   PRINT_FULL_LIST_INT(all_occurences_of_CHR);
   PRINT_DEBUG_DATA(all_occurences_of_CHR);
   MAP_LIST_INT(all_occurences_of_CHR);

   gll_t seq = gll_create(sizeof(char));
   
   chr = 'z';
   gll_append(&seq, &chr);
   chr = 'g';
   gll_append(&seq, &chr);
   //chr = 'a';
   //gll_append(&seq, &chr);

   PRINT_FULL_LIST(seq);
   PRINT_DEBUG_DATA(seq);
   MAP_LIST(seq);
   PRINT_FULL_LIST(my_ll);
   PRINT_DEBUG_DATA(my_ll);
   MAP_LIST(my_ll);


   gll_t result_search_all = gll_search_all(&my_ll, &seq);
   printf("Occurs on:\n");
   PRINT_FULL_LIST_INT(result_search_all);
   PRINT_DEBUG_DATA(result_search_all);
   MAP_LIST_INT(result_search_all);

}