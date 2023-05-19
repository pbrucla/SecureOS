#include "string.h"

void* memcpy(void* restrict dest, const void* restrict src, size_t n) {
    char* dest_ptr = (char*) dest;
    const char* src_ptr = (char*) src;
    for (size_t i = 0; i < n; i++) {
        dest_ptr[i] = src_ptr[i];
    }
    return dest;
}
void* memmove(void* dest, const void* src, size_t n) {
    char* dest_ptr = (char*) dest;
    const char* src_ptr = (char*) src;
    if (dest_ptr < src_ptr) {
        for (size_t i = 0; i < n; i++) {
            dest_ptr[i] = src_ptr[i];
        }
        return dest;
    }
    for (size_t i = n-1; i >= 0; i--) {
        dest_ptr[i] = src_ptr[i];
    }
    return dest;
}
void* memset(void* s, int c, size_t n) {
    unsigned char uc = (unsigned char) c;
    unsigned char* s_ptr = (unsigned char*) s;
    for (size_t i = 0; i < n; i++) {
        s_ptr[i] = uc;
    }
    return s;
}
int memcmp(const void* s1, const void* s2, size_t n) {
    char* s1_ptr = (char*) s1;
    char* s2_ptr = (char*) s2;
    size_t i = 0;
    for (;i < n; i++) {
        if (s1_ptr[i] != s2_ptr[i]) {
            break;
        }
    }
    if (i == n)
        return 0;
    return ((s1_ptr[i] > s2_ptr[i]) ? 1 : -1);
}
size_t strlen(const char* s);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);
char* strstr(const char* haystack, const char* needle);
char* strchr(const char* s, int c);
char* strrchr(const char* s, int c);