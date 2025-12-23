/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/

/*
 * INTENTIONAL VULNERABILITIES FOR SECURITY SCANNER TESTING
 * DO NOT USE IN PRODUCTION
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Vulnerability 1: Buffer overflow */
void test_buffer_overflow(const char *input)
{
  char buffer[32];
  strcpy(buffer, input);  /* Unsafe: no bounds checking */
  printf("Buffer: %s\n", buffer);
}

/* Vulnerability 2: Integer overflow in allocation */
void *test_integer_overflow(size_t count, size_t size)
{
  size_t total = count * size;  /* Can overflow */
  return malloc(total);
}

/* Vulnerability 3: Use-after-free */
void test_use_after_free(void)
{
  char *ptr = malloc(100);
  if(ptr) {
    free(ptr);
    printf("%s\n", ptr);  /* Use after free */
  }
}

/* Vulnerability 4: NULL dereference */
void test_null_deref(const char *input)
{
  char *buffer = malloc(100);
  strcpy(buffer, input);  /* No NULL check */
  free(buffer);
}

/* Vulnerability 5: Format string */
void test_format_string(const char *user_input)
{
  printf(user_input);  /* Format string vulnerability */
}

/* Vulnerability 6: Memory leak */
void test_memory_leak(int count)
{
  for(int i = 0; i < count; i++) {
    char *leak = malloc(1024);
    sprintf(leak, "data %d", i);
    /* No free() */
  }
}

int main(void)
{
  printf("Security test program\n");
  test_buffer_overflow("test");
  test_integer_overflow(1000, 1000);
  test_null_deref("test");
  test_format_string("test");
  test_memory_leak(10);
  return 0;
}
