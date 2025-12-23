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
 * INTENTIONAL SECURITY VULNERABILITIES FOR TESTING
 * 
 * This file contains deliberate security issues to test the security
 * scanning workflow. DO NOT use any code from this file in production.
 * 
 * Each function demonstrates a different vulnerability class.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* VULNERABILITY 1: Buffer overflow with strcpy() */
void unsafe_string_copy(const char *user_input)
{
  char buffer[64];
  
  /* Security Issue: strcpy doesn't check bounds
   * CWE-120: Buffer Copy without Checking Size of Input
   * Severity: HIGH - Can lead to stack buffer overflow
   */
  strcpy(buffer, user_input);
  
  printf("Copied: %s\n", buffer);
}

/* VULNERABILITY 2: Integer overflow in allocation */
void *allocate_buffer(size_t count, size_t size)
{
  void *ptr;
  
  /* Security Issue: Integer overflow in multiplication
   * CWE-190: Integer Overflow or Wraparound
   * Severity: HIGH - Can lead to undersized allocation and heap overflow
   * Example: count=0xFFFFFFFF, size=2 wraps to small value
   */
  size_t total = count * size;
  
  ptr = malloc(total);
  
  return ptr;
}

/* VULNERABILITY 3: Use-after-free */
void use_after_free_bug(void)
{
  char *data = malloc(100);
  
  if(data) {
    strcpy(data, "some data");
    printf("Data: %s\n", data);
    
    /* Free the memory */
    free(data);
    
    /* Security Issue: Use after free
     * CWE-416: Use After Free
     * Severity: CRITICAL - Can lead to code execution
     */
    printf("Freed data: %s\n", data);
  }
}

/* VULNERABILITY 4: NULL pointer dereference */
int process_data(const char *input)
{
  char *buffer;
  
  /* Security Issue: malloc can return NULL but not checked
   * CWE-476: NULL Pointer Dereference
   * Severity: MEDIUM - Causes crash/denial of service
   */
  buffer = malloc(256);
  strcpy(buffer, input);
  
  printf("Processed: %s\n", buffer);
  free(buffer);
  
  return 0;
}

/* VULNERABILITY 5: Format string vulnerability */
void log_message(const char *user_message)
{
  /* Security Issue: User input used directly as format string
   * CWE-134: Use of Externally-Controlled Format String
   * Severity: CRITICAL - Can read/write arbitrary memory
   * Attacker can use %x, %s, %n specifiers
   */
  printf(user_message);
  printf("\n");
}

/* VULNERABILITY 6: Memory leak */
void memory_leak_example(int iterations)
{
  int i;
  
  for(i = 0; i < iterations; i++) {
    char *temp = malloc(1024);
    
    /* Security Issue: Memory leak - malloc without free
     * CWE-401: Missing Release of Memory after Effective Lifetime
     * Severity: MEDIUM - Can cause resource exhaustion
     */
    if(temp) {
      sprintf(temp, "Iteration %d", i);
    }
    /* Missing free(temp) */
  }
}

/* VULNERABILITY 7: Unchecked return value */
void write_to_file(const char *filename, const char *data)
{
  FILE *fp;
  
  /* Security Issue: fopen can fail but not checked
   * CWE-252: Unchecked Return Value
   * Severity: MEDIUM - Can lead to NULL pointer dereference
   */
  fp = fopen(filename, "w");
  fprintf(fp, "%s", data);
  fclose(fp);
}

/* VULNERABILITY 8: Time-of-check to time-of-use (TOCTOU) */
void toctou_file_access(const char *filepath)
{
  /* Security Issue: TOCTOU race condition
   * CWE-367: Time-of-check Time-of-use Race Condition
   * Severity: HIGH - File can be changed between check and use
   */
  
  /* Check file exists */
  FILE *check = fopen(filepath, "r");
  if(check) {
    fclose(check);
    
    /* Time gap here - file could be replaced with symlink */
    
    /* Use file */
    FILE *use = fopen(filepath, "w");
    if(use) {
      fprintf(use, "secret data");
      fclose(use);
    }
  }
}

/* VULNERABILITY 9: Sign conversion issue */
int copy_data(char *dest, const char *src, int len)
{
  /* Security Issue: Sign conversion - negative len becomes large unsigned
   * CWE-195: Signed to Unsigned Conversion Error
   * Severity: HIGH - Can bypass bounds checks
   */
  if(len < 0) {
    return -1;
  }
  
  /* If len is close to INT_MAX, this could overflow */
  memcpy(dest, src, (size_t)len);
  
  return 0;
}

/* VULNERABILITY 10: Double free */
void double_free_bug(void)
{
  char *ptr = malloc(100);
  
  if(ptr) {
    strcpy(ptr, "test data");
    free(ptr);
    
    /* Security Issue: Double free
     * CWE-415: Double Free
     * Severity: HIGH - Can corrupt heap metadata
     */
    free(ptr);
  }
}

/* Main function - demonstrates calling vulnerable functions */
int main(void)
{
  char large_input[1024];
  
  printf("Security Test Program - DO NOT USE IN PRODUCTION\n");
  printf("This program contains intentional vulnerabilities\n\n");
  
  /* Generate test input */
  memset(large_input, 'A', sizeof(large_input) - 1);
  large_input[sizeof(large_input) - 1] = '\0';
  
  /* Call vulnerable functions */
  unsafe_string_copy("normal input");
  allocate_buffer(1000, 1000);
  process_data("test");
  log_message("Log message");
  memory_leak_example(10);
  
  return 0;
}
