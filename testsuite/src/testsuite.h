#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../../include/libgal.h"
#include "../../include/libgll.h"

// notação húngara era o mais conveniente pra usar aqui.
typedef struct __intf__Blob {
   void* blobData;
   size_t szElement;
   int32_t lBlob; // em elementos, não bytes.
} genericBlob;



typedef struct __intf__testCase {
   uint32_t testId;
   genericBlob arrSearch;
   genericBlob arrSeq;
   genericBlob arrOccurenceData; 
} tc_t, *ptc_t;

typedef double (*fTEST_RUNNER)(ptc_t);

typedef struct __intf__testContainer {
   gll_t arrTests;
   int32_t testCount;
   fTEST_RUNNER rt;
   // Apenas definidos após executar o teste:
   float timeOfFlight;
   float  meanTimeOfFlight;
   int32_t passedTests;
} testcontainer_t, *ptestcontainer_t;

testcontainer_t initTestContainer();

void add_test(ptestcontainer_t tc, tc_t t);
// Lê um arquivo .tc e retorna uma struct tc_t.
tc_t read_tc_file(const char* fp);
// Roda um teste
void runtest(ptestcontainer_t);
// troca o runner do container.
void changerunner(ptestcontainer_t t, fTEST_RUNNER r);

// roda o teste e retorna o tof.
double gll_testrunner(ptc_t);
double gal_testrunner(ptc_t);


// Helpers

// Serve para ler o tamanho de cada field do arquivo .tc
uint32_t read_field_length(FILE* f);

void print_tc(ptc_t tc);