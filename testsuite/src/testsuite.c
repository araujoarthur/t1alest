#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "testsuite.h"
#include "../../include/libgll.h"


testcontainer_t initTestContainer() {
   testcontainer_t tc;
   tc.arrTests = gll_create(sizeof(tc_t));
   tc.testCount = 0;
   tc.passedTests = 0;
   return tc;
}

void changerunner(ptestcontainer_t tc, fTEST_RUNNER r) {
   tc->meanTimeOfFlight = 0;
   tc->timeOfFlight = 0;
   tc->passedTests = 0;
   tc->rt = r;
}

void add_test(ptestcontainer_t tc, tc_t t) {
   gll_append(&(tc->arrTests), &t); //For some reason, gll's memcpy is segfaulting here
   tc->testCount++;
}

uint32_t read_field_length(FILE* f) {
   uint8_t buf[4];
   if (fread(buf, 1, 4, f) != 4) {
      fprintf(stderr, "Erro ao ler o tamanho do próximo campo");
      exit(1);
   }

   // retorna ja invertendo endianess
   return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}


// Lê um arquivo .tc e retorna uma struct tc_t.
tc_t read_tc_file(const char* fp) {
   FILE* f = fopen(fp, "rb");
   if (!f) {
      perror("fopen");
      exit(1);
   }
   tc_t res;

   // Extrai o ID do teste.
   char* startChar = strrchr(fp, '-') + 1;
   char* endChar = strrchr(fp, '.');

   size_t idLen = endChar-startChar;

   char strId[256];
   strncpy(strId, startChar, idLen);
   strId[idLen] = '\0';
   if(sscanf(strId, "%d", &res.testId) != 1) {
      printf("Could not parse test id\n");
      return res;
   }
   

   // Obtem a string onde será realizada a busca
   res.arrSearch.lBlob = read_field_length(f);
   res.arrSearch.szElement = sizeof(char);
   res.arrSearch.blobData = malloc(res.arrSearch.szElement * res.arrSearch.lBlob);
   fread(res.arrSearch.blobData, res.arrSearch.szElement, res.arrSearch.lBlob, f);


   // Obtem a string que está sendo buscada
   res.arrSeq.lBlob = read_field_length(f);
   res.arrSeq.szElement = sizeof(char);
   res.arrSeq.blobData = malloc(res.arrSeq.szElement * res.arrSeq.lBlob);
   fread(res.arrSeq.blobData, res.arrSeq.szElement, res.arrSeq.lBlob, f);

   

   // Obtem a quantidade de ocorrencias
   res.arrOccurenceData.lBlob = read_field_length(f);
   res.arrOccurenceData.szElement = sizeof(uint32_t); // deve ser 4bytes, mas JIC.
   res.arrOccurenceData.blobData = malloc(res.arrOccurenceData.szElement * res.arrOccurenceData.lBlob);
   for (uint32_t i = 0; i < res.arrOccurenceData.lBlob; i++) {
      ((uint32_t*)res.arrOccurenceData.blobData)[i] = read_field_length(f);
   } 
   fclose(f);

   return res;
}

double gll_testrunner(ptc_t t) {
   double tof = 0;
   // Setup do Teste
   printf("\n\n === Executando Teste ID: %d Com Linked List === \n\n",t->testId);
   gll_t text = gll_create(sizeof(char));
   gll_t pattern = gll_create(sizeof(char));

   int32_t countOccurences = t->arrOccurenceData.lBlob;
   int32_t* occurencesPositions = t->arrOccurenceData.blobData;

   // preencher text
   for(int i = 0; i < t->arrSearch.lBlob; i++) {
      char* current_char = (((char *)t->arrSearch.blobData) + i);
      gll_append(&text, current_char);
   }

   // preencher seq
   for(int i = 0; i < t->arrSeq.lBlob; i++) {
      char* current_char = (((char *)t->arrSeq.blobData) + i);
      gll_append(&pattern, current_char);
   }
   
   clock_t start = clock();
   gll_t result = gll_search_all(&text, &pattern);
   clock_t stop = clock();

   // comparar resultados.
   if (result.element_count == countOccurences) {
      printf("#1 PASSED: Quantidade de ocorrencias esperada (%d) está de acordo com a encontrada (%d)\n", countOccurences, result.element_count);
   }
   else {
      printf("#1 FAILED [GLL]: Quantidade de ocorrencias esperada (%d) NÃO está de acordo com a encontrada (%d)\n", countOccurences, result.element_count);
      print_tc(t);
      pllnode_t found = result.head;
      printf("Found Positions: ");
      while(found) {
         printf("%d, ", *((int*)(found->data)));
         found = found->next;
      }
      printf("\n");
      printf("\n ### Finalizado [COM ERRO] Teste ID: %d Com Linked List ### \n",t->testId);
      return -1.0;
   }

   for (int i=0; i<result.element_count; i++) {
      int exp_pos = -1;
      int fnd_pos = -1;

      pllnode_t fnd_pos_node = (gll_get(&result, i));
      fnd_pos = *((int*) fnd_pos_node->data);
      exp_pos = *(((int*) occurencesPositions) + i);

      if (fnd_pos == exp_pos) {
         printf("#2 PASSED [GLL]: Ocorrencia esperada ocorreu (Esperado %d)->(Encontrado %d)\n", exp_pos, fnd_pos);
      } else {
         printf("#2 FAILED [GLL]: Ocorrencia esperada não aconteceu (Esperado %d)->(Encontrado %d)\n", exp_pos, fnd_pos);
         printf("\n ### Finalizado [COM ERRO] Teste ID: %d Com Linked List ### \n",t->testId);
         return -1.0;
      }
   }
   printf("\n ### [SUCESSO-GLL] Finalizado Teste ID: %d Com Linked List ### \n",t->testId);
   tof = (double) stop - start;
   return tof;
}

double gal_testrunner(ptc_t t) {
   double tof = 0;
   // Setup do Teste
   printf("\n\n === Executando Teste ID: %d Com Array List === \n\n",t->testId);
   gal_t text = gal_create(sizeof(char), 0);
   gal_t pattern = gal_create(sizeof(char), 0);

   int32_t countOccurences = t->arrOccurenceData.lBlob;
   int32_t* occurencesPositions = t->arrOccurenceData.blobData;
   
   // Preencher Text
   for(int i = 0; i < t->arrSearch.lBlob; i++) {
      char* current_char = (((char *)t->arrSearch.blobData) + i);
      gal_append(&text, current_char);
   }

   // Preencher Pattern
   for(int i = 0; i < t->arrSeq.lBlob; i++) {
      char* current_char = (((char *)t->arrSeq.blobData) + i);
      gal_append(&pattern, current_char);
   }

   // Realizar busca
   clock_t start = clock();
   gal_t result = gal_search_all(&text, &pattern);
   clock_t stop = clock();


   // comparar resultados.
   if (result.element_count == countOccurences) {
      printf("#1 PASSED [GAL]: Quantidade de ocorrencias esperada (%d) está de acordo com a encontrada (%d)\n", countOccurences, result.element_count);
   }
   else {
      printf("#1 FAILED [GAL]: Quantidade de ocorrencias esperada (%d) NÃO está de acordo com a encontrada (%d)\n", countOccurences, result.element_count);
      print_tc(t);
      printf("\n ### Finalizado [COM ERRO] Teste ID: %d Com Array List ### \n",t->testId);
      return -1.0;
   }

   for (int i=0; i<result.element_count; i++) {
      int exp_pos = -1;
      int fnd_pos = -1;

      gml_getn(&result, i, &fnd_pos, -1);
      exp_pos = *(((int*) occurencesPositions) + i);

      if (fnd_pos == exp_pos) {
         printf("#2 PASSED [GAL]: Ocorrencia esperada ocorreu (Esperado %d)->(Encontrado %d)\n", exp_pos, fnd_pos);
      } else {
         printf("#2 FAILED [GAL]: Ocorrencia esperada não aconteceu (Esperado %d)->(Encontrado %d)\n", exp_pos, fnd_pos);
         printf("\n ### Finalizado [COM ERRO] Teste ID: %d Com Array List ### \n",t->testId);
         return -1.0;
      }
   }

   printf("\n ### [SUCESSO-GAL] Finalizado Teste ID: %d Com Array List ### \n",t->testId);
   tof = (double) stop - start;
   return tof;
}


void runtest(ptestcontainer_t t) {
   double totalTof = 0;
   pllnode_t curr_test_node = t->arrTests.head;

   while(curr_test_node) {

      ptc_t curr_test = (ptc_t) curr_test_node->data;

      double current_test_tof = t->rt(curr_test);

      if (current_test_tof > 0) {
         totalTof += current_test_tof;
         t->passedTests++;
      }

      curr_test_node = curr_test_node->next;
   }

   t->timeOfFlight =(double) totalTof / CLOCKS_PER_SEC;
   t->meanTimeOfFlight = ( (double) totalTof/CLOCKS_PER_SEC) /(double) t->arrTests.element_count;
}

void print_tc(ptc_t tc) {
   printf("Search String: [");
   for(int i = 0; i < tc->arrSearch.lBlob; i++) {
      char c = *(((char*)(tc->arrSearch.blobData)) + i);
      if (i == tc->arrSearch.lBlob - 1) {
         printf("%c(%d)]\n", c, i);
      } else {
         printf("%c(%d), ", c, i);
      }
   }
   
   printf("Sequence String: [");
   for(int i = 0; i < tc->arrSeq.lBlob; i++) {
      char c = *(((char*)(tc->arrSeq.blobData)) + i);
      if (i == tc->arrSeq.lBlob - 1) {
         printf("%c]\n", c);
      } else {
         printf("%c ", c);
      }
   }

   printf("Occurence Count: %d\n",tc->arrOccurenceData.lBlob);
   
   printf("Occurence Positions: [");
   for(int i = 0; i < tc->arrOccurenceData.lBlob; i++) {
      if (i == tc->arrOccurenceData.lBlob -  1) {
         printf("%d]\n", *((int*)(tc->arrOccurenceData.blobData)+i));
      } else {
         printf("%d,", *((int*)(tc->arrOccurenceData.blobData)+i));
      }
   }
}