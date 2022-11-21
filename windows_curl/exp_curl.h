#include <stdio.h>
#include <time.h>
#ifndef __CYGWIN__
#include <winsock2.h>
#include <windows.h>
#endif
#include <curl/curl.h>

#define MAX_STRING              256
#define MAX_STRING1             MAX_STRING + 1

#define SYNCTIME_UA "synctime/1.0"


typedef struct
{
  char http_proxy[MAX_STRING1];
  char proxy_user[MAX_STRING1];
  char timeserver[MAX_STRING1];
} conf_t;


class exp_curl {

private :
  const char DefaultTimeServer[3][MAX_STRING1] =
  {
    "https://nist.time.gov/",
    "https://www.google.com/"
  };

  const char* DayStr[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
  const char* MthStr[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

  int  ShowAllHeader;
  int  AutoSyncTime;
  SYSTEMTIME SYSTime;
  SYSTEMTIME LOCALTime;


public :

  

};
