#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <libcd.h>

int testCDLoad() {
  
  CdlFILE file;
  u_long* buffer;

  if(!CdSearchFile(&file, "\\COOL.TXT;1")) {
    printf( "File not found.\n" ); 
    return 1;
  }

  buffer = (u_long*)malloc3( 2048*((file.size+2047)/2048) );
  CdControl(CdlSetloc, (u_char*)&file.pos, 0);
  CdRead((file.size+2047)/2048, buffer, CdlModeSpeed);
  CdReadSync(0, 0);

  printf((const char*)buffer, "\n");

  free3(buffer);

  return 0;
}

u_long* loadFileFromCD(char* filename) {
  
  CdlFILE file;
  u_long* buffer;

  if(!CdSearchFile(&file, filename)) {
    printf( "File not found.\n" ); 
    return NULL;
  }

  buffer = (u_long*)malloc3( 2048*((file.size+2047)/2048) );
  CdControl(CdlSetloc, (u_char*)&file.pos, 0);
  CdRead((file.size+2047)/2048, buffer, CdlModeSpeed);
  CdReadSync(0, 0);

  return buffer;

}
