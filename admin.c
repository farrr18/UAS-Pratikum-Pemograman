#include <stdio.h>
#include <stdlib.h>
#include <string.h>>

#define MAX_BUKU 500
#define PANJANG_BARIS 600

#define FILE_AKUN "user.txt"
#define FILE_BUKU "Books.txt"
#define FILE_TRANSAKSI "peminjaman.txt"

typedef struct {
    char ocaid[64];
    char title[150];
    char author[100];
    char publisher[100];
    int year;
    int page_count;
    int stock;   
} Buku;

typedef struct {
    Buku daftar[MAX_BUKU];
    int jumlah;
} DataBuku;

