#ifndef PERPUSTAKAAN_H
#define PERPUSTAKAAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUKU 10000

typedef struct {
    char ocaid[64], title[256], author[200];
    int year, page_count;
    char openLibrary_edition[64];
    char openLibrary_work[64];
    char full_text_url[256];
} Book;

#endif