#pragma once

#include <stddef.h>

typedef struct string {
  char* data;
  size_t len;
} string;

void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
void* memset(void* s, int c, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
//if no comment in string.c then function does the same thing as cstring version
size_t strlen(const string* s);
string* strcpy(string* dest, const string* src);
string* strncpy(string* dest, const string* src, size_t n);
int strcmp(const string* s1, const string* s2);
int strncmp(const string* s1, const string* s2, size_t n);
string* strcat(string* dest, const string* src);
string* strncat(string* dest, const string* src, size_t n);
char* strstr(const string* haystack, const string* needle);
char* strchr(const string* s, int c);
char* strrchr(const string* s, int c);