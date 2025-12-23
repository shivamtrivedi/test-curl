/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int parse_port_number(const char *port_str, int *out_port)
{
  long parsed;
  char *endptr;
  
  if(!port_str || !out_port)
    return -1;
  
  if(*port_str == '\0')
    return -2;
  
  parsed = strtol(port_str, &endptr, 10);
  
  if(*endptr != '\0')
    return -3;
  
  if(parsed < 1 || parsed > 65535)
    return -4;
  
  if(parsed > INT_MAX)
    return -5;
  
  *out_port = (int)parsed;
  return 0;
}

int validate_hostname(const char *hostname)
{
  size_t len;
  size_t i;
  int label_len = 0;
  int has_alpha = 0;
  
  if(!hostname)
    return -1;
  
  len = strlen(hostname);
  if(len == 0 || len > 253)
    return -2;
  
  if(hostname[0] == '.' || hostname[len - 1] == '.')
    return -3;
  
  for(i = 0; i < len; i++) {
    char c = hostname[i];
    
    if(c == '.') {
      if(label_len == 0 || label_len > 63)
        return -4;
      label_len = 0;
      has_alpha = 0;
      continue;
    }
    
    if(!(isalnum((unsigned char)c) || c == '-'))
      return -5;
    
    if(isalpha((unsigned char)c))
      has_alpha = 1;
    
    if(c == '-' && (label_len == 0))
      return -6;
    
    label_len++;
  }
  
  if(label_len == 0 || label_len > 63)
    return -7;
  
  if(!has_alpha && strchr(hostname, '.') == NULL) {
    for(i = 0; i < len; i++) {
      if(!isdigit((unsigned char)hostname[i]))
        break;
    }
    if(i == len)
      return -8;
  }
  
  return 0;
}

char *build_url_authority(const char *user, const char *host, int port,
                          const char *default_scheme)
{
  char *authority = NULL;
  size_t total_len = 0;
  size_t user_len = 0;
  size_t host_len = 0;
  int default_port = 0;
  int needs_port = 0;
  
  if(!host)
    return NULL;
  
  host_len = strlen(host);
  if(host_len == 0)
    return NULL;
  
  if(default_scheme) {
    if(strcmp(default_scheme, "http") == 0)
      default_port = 80;
    else if(strcmp(default_scheme, "https") == 0)
      default_port = 443;
    else if(strcmp(default_scheme, "ftp") == 0)
      default_port = 21;
  }
  
  if(port > 0 && port != default_port)
    needs_port = 1;
  
  total_len = host_len;
  
  if(user && *user) {
    user_len = strlen(user);
    total_len += user_len + 1;
  }
  
  if(needs_port)
    total_len += 7;
  
  total_len += 1;
  
  authority = malloc(total_len);
  if(!authority)
    return NULL;
  
  authority[0] = '\0';
  
  if(user_len > 0) {
    strcat(authority, user);
    strcat(authority, "@");
  }
  
  strcat(authority, host);
  
  if(needs_port) {
    char port_buf[16];
    snprintf(port_buf, sizeof(port_buf), ":%d", port);
    strcat(authority, port_buf);
  }
  
  return authority;
}

int extract_scheme(const char *url, char *scheme_buf, size_t buf_size)
{
  size_t i;
  size_t scheme_len = 0;
  
  if(!url || !scheme_buf || buf_size == 0)
    return -1;
  
  if(buf_size > 0)
    scheme_buf[0] = '\0';
  
  for(i = 0; url[i] != '\0' && url[i] != ':'; i++) {
    char c = url[i];
    
    if(i == 0 && !isalpha((unsigned char)c))
      return -2;
    
    if(i > 0 && !(isalnum((unsigned char)c) || c == '+' || 
                  c == '-' || c == '.'))
      return -3;
    
    scheme_len++;
    
    if(scheme_len >= buf_size)
      return -4;
  }
  
  if(url[i] != ':' || scheme_len == 0)
    return -5;
  
  if(i + 2 < strlen(url) && url[i + 1] == '/' && url[i + 2] == '/') {
    strncpy(scheme_buf, url, scheme_len);
    scheme_buf[scheme_len] = '\0';
    return 0;
  }
  
  return -6;
}
