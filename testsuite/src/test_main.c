#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <dirent.h> // cria dependencia a sistemas posix.
#include "./src/testsuite.h"

int addAllTests(ptestcontainer_t tc, const char* testFolderPath);
int cmpextension(const char* fn, const char* ext);

typedef struct FINAL_RESULT {
   double galTof;
   double galMTof;
   double gllTof;
   double gllMTof;
   int gllPassed;
   int galPassed;
   int totalTests;
} finalResult;

int main() {
   printf("Hello There!\n");
   testcontainer_t tests = initTestContainer();
   finalResult result;

   result.totalTests = addAllTests(&tests, "./testcases/");
   tests.rt = gal_testrunner;
   runtest(&tests); // 
   result.galPassed = tests.passedTests;
   result.galMTof = tests.meanTimeOfFlight;
   result.galTof = tests.timeOfFlight;
   changerunner(&tests, gll_testrunner);
   runtest(&tests);
   result.gllPassed = tests.passedTests;
   result.gllMTof = tests.meanTimeOfFlight;
   result.gllTof = tests.timeOfFlight;

   printf("\n## Final Result ##\n");
   printf("\nTotal Tests: %d\n", tests.testCount);
   printf("\n");
   printf("----Generic Array List:\n");
   printf("       Passed Tests: %d (%.2f%%)\n", result.galPassed, (result.galPassed/(double)tests.testCount) *100);
   printf("       Mean Time of Flight: %.8f\n", result.galMTof);
   printf("       Total Time of Flight: %.8f\n", result.galTof);
   printf("\n");
   printf("----Generic Double Linked List:\n");
   printf("       Passed Tests: %d (%.2f%%)\n", result.gllPassed, (result.gllPassed/(double)tests.testCount) *100);
   printf("       Mean Time of Flight: %.8f\n", result.gllMTof);
   printf("       Total Time of Flight: %.8f\n", result.gllTof);
   return 1;
}

int addAllTests(ptestcontainer_t tc, const char* testFolderPath) {
   DIR *dir = opendir(testFolderPath);

   if(!dir) {
      perror("opendir");
      return 1;
   }
   
   struct dirent *entry;
   int addedTests = 0;
   while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

      if (cmpextension(entry->d_name, ".tc")) {
         //printf("Adding Test %s...\n", entry->d_name);

         char* relPath = calloc(PATH_MAX, sizeof(char));
         strcpy(relPath, testFolderPath);
         strcat(relPath, entry->d_name);

         char* filePath = calloc(PATH_MAX, sizeof(char));
         if (!realpath(relPath, filePath)) {
            perror("realpath");
         }

         tc_t fileData = read_tc_file(filePath);
         add_test(tc, fileData);
         addedTests++;
      }
   }

   printf("Added a total of %d tests\n", addedTests);
   return addedTests;
}

int cmpextension(const char* fn, const char* ext) {
   const char* dot =  strrchr(fn, '.');
   if(!dot || dot == fn) return 0;
   return strcmp(dot, ext) == 0;
}


