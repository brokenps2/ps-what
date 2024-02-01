#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <libcd.h>

void testCDLoad(char* filename) {

  CdlFILE file;
  char* realfnamep1 = strcat("\\", filename);
  char* realfname = strcat(realfnamep1, ";1");

  if(!CdSearchFile(&file, realfname)) {
    printf( "File not found.\n" );  
  } else {
    
  }

}
