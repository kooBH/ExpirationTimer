#include "exp_serial.h"

bool validation_with_serial(const char* serial) {

  char buffer[2048];

  FILE* pipe = _popen("wmic diskdrive get Model, SerialNumber", "r");
  //FILE* pipe = _popen("wmic diskdrive get SerialNumber", "r");

  if (!pipe) {
    printf("ERROR::Can not open 'wmic'. This is default Windows utility. Make sure you are in valid environment.\n");
    exit(-1);
  }
  int idx = 0;
  int cmp;
  int i;

  while (!feof(pipe)) {
    i = 0;
   // printf("line : %d : ",idx++);
    if (fgets(buffer, 128, pipe) != NULL) {

      // remove space
      while (buffer[i]) {
        if (isspace((unsigned char)buffer[i])) {
          buffer[i] = '\0';
          break;
        }
        else
          i++;
      }

      // compare with serial
      cmp = strcmp(serial, buffer);

      //printf("%s(%d) | %s(%d)  : %d\n", buffer,strlen(buffer), serial,strlen(serial),cmp);
      if (cmp == 0)return true;
    }
  }
  _pclose(pipe);

  return false;
}
