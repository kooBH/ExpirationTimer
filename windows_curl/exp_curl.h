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

  const char* DayStr[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
  const char* MthStr[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                          "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

  SYSTEMTIME SYSTime;
  SYSTEMTIME LOCALTime;

typedef struct
{
  char http_proxy[MAX_STRING1];
  char proxy_user[MAX_STRING1];
  char timeserver[MAX_STRING1];
} conf_t;

size_t SyncTime_CURL_WriteOutput(void* ptr, size_t size, size_t nmemb,
  void* stream)
{
  fwrite(ptr, size, nmemb, (FILE*)stream);
  return (nmemb * size);
}

size_t SyncTime_CURL_WriteHeader(void* ptr, size_t size, size_t nmemb,
  void* stream)
{
  int ShowAllHeader = 1;
  int AutoSyncTime = 1;

  char  TmpStr1[26], TmpStr2[26];
 
  (void)stream;

  if (ShowAllHeader == 1)
    fprintf(stderr, "%s", (char*)(ptr));
  if (strncmp((char*)(ptr), "Date:", 5) == 0) {
    if (ShowAllHeader == 0)
      fprintf(stderr, "HTTP Server. %s", (char*)(ptr));

    if (AutoSyncTime == 1) {
      *TmpStr1 = 0;
      *TmpStr2 = 0;
      if (strlen((char*)(ptr)) > 50) {
        /* Can prevent buffer overflow to
        TmpStr1 & 2? */
        AutoSyncTime = 0;
        printf("ptr  < 50\n");
      }
      else {
        int RetVal = sscanf((char*)(ptr), "Date: %25s %hu %s %hu %hu:%hu:%hu",
          TmpStr1, &SYSTime.wDay, TmpStr2, &SYSTime.wYear,
          &SYSTime.wHour, &SYSTime.wMinute,
          &SYSTime.wSecond);

        if (RetVal == 7) {
          int i;
          SYSTime.wMilliseconds = 500;    /* adjust to midpoint, 0.5 sec */
          for (i = 0; i < 12; i++) {
            if (strcmp(MthStr[i], TmpStr2) == 0) {
              SYSTime.wMonth = i + 1;
              break;
            }
          }
          AutoSyncTime = 3;       /* Computer clock will be adjusted */
        }
        else {
          AutoSyncTime = 0;       /* Error in sscanf() fields conversion */
        }
      }
    }
  }

  if (strncmp((char*)(ptr), "X-Cache: HIT", 12) == 0) {
    fprintf(stderr, "ERROR: HTTP Server data is cached."
      " Server Date is no longer valid.\n");
    AutoSyncTime = 0;
  }
  return (nmemb * size);
}


class exp_curl {

private :
  const char DefaultTimeServer[3][MAX_STRING1] =
  {
    "https://nist.time.gov/",
    "https://www.google.com/"
  };


  CURL* curl;
  conf_t  conf[1];
  int     RetValue;

  int  ShowAllHeader;
  int  AutoSyncTime;

  const int HTTP_COMMAND_HEAD = 0;
  const int HTTP_COMMAND_GET = 1;

  inline int conf_init(conf_t* conf);
  inline void SyncTime_CURL_Init(CURL* curl, char* proxy_port, char* proxy_user_password);
  inline int SyncTime_CURL_Fetch(CURL* curl, char* URL_Str, char* OutFileName, int HttpGetBody);

public :
  inline exp_curl();
  inline ~exp_curl();
  inline void check();

};


exp_curl::exp_curl() {
  ShowAllHeader = 0;    /* Do not show HTTP Header */
  AutoSyncTime = 0;    /* Do not synchronise computer clock */
  RetValue = 0;    /* Successful Exit */
  conf_init(conf);

  if (*conf->timeserver == 0)     /* Use default server for time information */
    snprintf(conf->timeserver, MAX_STRING, "%s", DefaultTimeServer[0]);

  /* Init CURL before usage */
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if (!curl)
    printf("ERROR::exp_curl::Fail to init curl\n");

}

exp_curl::~exp_curl() {

}

void exp_curl::check() {
  struct tm* lt;
  struct tm* gmt;
  time_t tt;
  time_t tt_local;
  time_t tt_gmt;
  double tzonediffFloat;
  int tzonediffWord;
  char timeBuf[61];
  char tzoneBuf[16];

  SyncTime_CURL_Init(curl, conf->http_proxy, conf->proxy_user);

  /* Calculating time diff between GMT and localtime */
  tt = time(0);
  lt = localtime(&tt);
  tt_local = mktime(lt);
  gmt = gmtime(&tt);
  tt_gmt = mktime(gmt);
  tzonediffFloat = difftime(tt_local, tt_gmt);
  tzonediffWord = (int)(tzonediffFloat / 3600.0);

  if ((double)(tzonediffWord * 3600) == tzonediffFloat)
    snprintf(tzoneBuf, 15, "%+03d'00'", tzonediffWord);
  else
    snprintf(tzoneBuf, 15, "%+03d'30'", tzonediffWord);

  /* Get current system time and local time */
  GetSystemTime(&SYSTime);
  GetLocalTime(&LOCALTime);
  snprintf(timeBuf, 60, "%s, %02d %s %04d %02d:%02d:%02d.%03d, ",
    DayStr[LOCALTime.wDayOfWeek], LOCALTime.wDay,
    MthStr[LOCALTime.wMonth - 1], LOCALTime.wYear,
    LOCALTime.wHour, LOCALTime.wMinute, LOCALTime.wSecond,
    LOCALTime.wMilliseconds);

  fprintf(stderr, "Fetch: %s\n\n", conf->timeserver);
  fprintf(stderr, "Before HTTP. Date: %s%s\n\n", timeBuf, tzoneBuf);

  /* HTTP HEAD command to the Webserver */
  SyncTime_CURL_Fetch(curl, conf->timeserver, "index.htm",
    HTTP_COMMAND_HEAD);

  snprintf(timeBuf, 60, "%s, %02d %s %04d %02d:%02d:%02d.%03d, ",
    DayStr[SYSTime.wDayOfWeek], SYSTime.wDay,
    MthStr[SYSTime.wMonth - 1], SYSTime.wYear,
    SYSTime.wHour, SYSTime.wMinute, SYSTime.wSecond,
    SYSTime.wMilliseconds);
  fprintf(stderr, "\nSYS  HTTP. Date: %s%s\n", timeBuf, tzoneBuf);

  GetLocalTime(&LOCALTime);
  snprintf(timeBuf, 60, "%s, %02d %s %04d %02d:%02d:%02d.%03d, ",
    DayStr[LOCALTime.wDayOfWeek], LOCALTime.wDay,
    MthStr[LOCALTime.wMonth - 1], LOCALTime.wYear,
    LOCALTime.wHour, LOCALTime.wMinute, LOCALTime.wSecond,
    LOCALTime.wMilliseconds);
  fprintf(stderr, "\nAfter  HTTP. Date: %s%s\n", timeBuf, tzoneBuf);

  if (AutoSyncTime == 3) {
    /* Synchronising computer clock */
    if (!SetSystemTime(&SYSTime)) {  /* Set system time */
      fprintf(stderr, "ERROR: Unable to set system time.\n");
      RetValue = 1;
    }
    else {
      /* Successfully re-adjusted computer clock */
      GetLocalTime(&LOCALTime);
      snprintf(timeBuf, 60, "%s, %02d %s %04d %02d:%02d:%02d.%03d, ",
        DayStr[LOCALTime.wDayOfWeek], LOCALTime.wDay,
        MthStr[LOCALTime.wMonth - 1], LOCALTime.wYear,
        LOCALTime.wHour, LOCALTime.wMinute, LOCALTime.wSecond,
        LOCALTime.wMilliseconds);
      fprintf(stderr, "\nNew System's Date: %s%s\n", timeBuf, tzoneBuf);
    }
  }

  /* Cleanup before exit */
  conf_init(conf);
  curl_easy_cleanup(curl);
}


int exp_curl::conf_init(conf_t* conf){
  int i;

  *conf->http_proxy = 0;
  for (i = 0; i < MAX_STRING1; i++)
    conf->proxy_user[i] = 0;    /* Clean up password from memory */
  *conf->timeserver = 0;
  return 1;
}

void exp_curl::SyncTime_CURL_Init(CURL* curl, char* proxy_port,
  char* proxy_user_password)
{
  if (strlen(proxy_port) > 0)
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy_port);

  if (strlen(proxy_user_password) > 0)
    curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxy_user_password);

#ifdef SYNCTIME_UA
  curl_easy_setopt(curl, CURLOPT_USERAGENT, SYNCTIME_UA);
#endif
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SyncTime_CURL_WriteOutput);
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, SyncTime_CURL_WriteHeader);

  //https://stackoverflow.com/questions/62025462/curl-easy-perform-failed-ssl-peer-certificate-or-ssh-remote-key-was-not-ok
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYSTATUS, 1);
  curl_easy_setopt(curl, CURLOPT_CAINFO, "cacert.pem");
  curl_easy_setopt(curl, CURLOPT_CAPATH, "cacert.pem");
}



int exp_curl::SyncTime_CURL_Fetch(CURL* curl, char* URL_Str, char* OutFileName,
  int HttpGetBody)
{
  FILE* outfile;
  CURLcode res;

  outfile = NULL;
  if (HttpGetBody == HTTP_COMMAND_HEAD)
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
  else {
    outfile = fopen(OutFileName, "wb");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
  }

  curl_easy_setopt(curl, CURLOPT_URL, URL_Str);
  printf("curl_easy_perform\n");
  res = curl_easy_perform(curl);
  if (res != CURLE_OK)
    fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(res));

  if (outfile)
    fclose(outfile);
  return res;  /* (CURLE_OK) */
}
