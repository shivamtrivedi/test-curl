/*
 * URL helper utilities for curl
 */

#include "curl_setup.h"

/*
 * Validates if a given string is a well-formed URL
 * Returns 1 if valid, 0 otherwise
 */
int curl_url_is_valid(const char *url)
{
  if(!url || !*url)
    return 0;

  /* Basic scheme check */
  if(strncmp(url, "http://", 7) == 0 ||
     strncmp(url, "https://", 8) == 0)
    return 1;

  return 0;
}
