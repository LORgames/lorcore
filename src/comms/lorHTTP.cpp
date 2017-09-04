#include "lorHTTP.h"
#include "lorPlatform.h"

#include <stdio.h>
//#include "curl/curl.h"

void lorAuth_Init()
{
  //curl_global_init(CURL_GLOBAL_DEFAULT);
}

void lorAuth_Close()
{
  //curl_global_cleanup();
}

void lorAuth_Test()
{
  //CURL *curl;
  //CURLcode res;
  //
  //curl = curl_easy_init();
  //if (curl)
  //{
  //  char buffer[256];
  //  lorSprintf(buffer, 256, "tok=getsession&os=%s&ram=%d&cpucores=%d", SDL_GetPlatform(), SDL_GetSystemRAM(), SDL_GetCPUCount());
  //
  //  curl_easy_setopt(curl, CURLOPT_URL, "https://lorgames.com/pusher/analytics.php");
  //  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
  //
  //  if(curl_easy_setopt(curl, CURLOPT_CAINFO, ASSETDIR "LORgamesSSLCACertificate.pem") != CURLE_OK)
  //    lorLog("Bad CA Cert!");
  //
  //  /* Perform the request, res will get the return code */
  //  res = curl_easy_perform(curl);
  //
  //  /* Check for errors */
  //  if (res != CURLE_OK)
  //  {
  //    lorLog("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  //  }
  //  else
  //  {
  //    lorLog("Got Message!\n");
  //  }
  //
  //  /* always cleanup */
  //  curl_easy_cleanup(curl);
  //}
}
