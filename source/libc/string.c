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

size_t strlen(const string* s) {
    return s->len;
}
//TODO: add memory reallocation and overlap checking for all of str functions
//INFO: below functions assume no memory overlap right now
//copies src data to dest data
string* strcpy(string* dest, const string* src) {
    dest->data = memcpy(dest->data, src->data, src->len);
    dest->len = src->len;
    return dest;
};

//copies first n char from src to dest, if n > src.len then only src.len char is copied
string* strncpy(string* dest, const string* src, size_t n) {
    size_t len = (n <= src->len) ? n : src->len;
    dest->data = memcpy(dest->data, src->data, len);
    dest->len = len;
    return dest;
}

int strcmp(const string* s1, const string* s2) {
    size_t len = (s1->len <= s2->len) ? s1->len : s2->len;
    return memcmp(s1->data, s2->data, len);
}

int strncmp(const string* s1, const string* s2, size_t n) {
    size_t len = (s1->len <= s2->len) ? s1->len : s2->len;
    len = (len <= n) ? len : n;
    return memcmp(s1->data, s2->data, len);
}

string* strcat(string* dest, const string* src) {
    memcpy(dest->data + dest->len, src->data, src->len);
    dest->len += src->len;
    return dest;
}

string* strncat(string* dest, const string* src, size_t n) {
    size_t len = (src->len < n) ? src->len : n;
    memcpy(dest->data + dest->len, src->data, len);
    dest->len += src->len;
    return dest;
}

char* strstr(const string* haystack, const string* needle) {
    if (haystack->len < needle->len)
        return NULL;
    for (int i = 0; i <= haystack->len-needle->len; i++) {
        if (memcmp(&haystack->data[i], needle->data, needle->len)) {
            return &haystack->data[i];
            break;
        }
    }
    return NULL;
}
char* strchr(const string* s, int c) {
    for (int i = 0; i < s->len; i++) {
        if (s->data[i] == c)
            return &s->data[i];
    }
    return NULL;
}

char* strrchr(const string* s, int c) {
    for (int i = s->len-1; i>=0; i--) {
        if (s->data[i] == c)
            return &s->data[i];
    }
    return NULL;
}