#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "penyimpanan.h"
#include "admin.h"

#define MAX_BUKU 500
#define PANJANG_BARIS 600

#define FILE_AKUN "user.txt"
#define FILE_BUKU "Books.txt"
#define FILE_TRANSAKSI "peminjaman.txt"

void tambah_buku(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;

    int hasil = baca_buku(FILE_BUKU,&daftar_buku,&jumlah_buku);

    if(!hasil && (daftar_buku != NULL || jumlah_buku != 0)) {
        printf("Gagal membaca data buku!\n");
        free(daftar_buku);
        return;
    }

    Buku buku_baru;

    printf("\n=== TAMBAH BUKU ===\n");

    printf("ID Buku    :");
    scanf("%d", &buku_baru.id);
    getchar();

    printf("Judul Buku  :");
    fgets(buku_baru.judul, sizeof(buku_baru.judul), stdin);
    buku_baru.judul[strcspn(buku_baru.judul, "\n")] ='\0';

    printf("Penulis   :");
    fgets(buku_baru.penulis, sizeof(buku_baru.penulis), stdin);
    buku_baru.penulis[strcspn(buku_baru.penulis, "\n")] ='\0';

    printf("Tahun terbit   :");
    scanf("%d", &buku_baru.tahun);

    printf("Jumlah Halaman    :");
    scanf("%d", &buku_baru.halaman);

    printf("Stok buku    :");
    scanf("%d", &buku_baru.stok);

    Buku *data_baru = realloc(
        daftar_buku,
        (jumlah_buku + 1) * sizeof(Buku)
    );

    if (data_baru == NULL) {
        printf("Gagal menambahkan buku!\n");
        free(daftar_buku);
        return;
    }

    daftar_buku = data_baru;
    daftar_buku[jumlah_buku] = buku_baru;
    jumlah_buku++;

    if (simpan_buku(FILE_BUKU, daftar_buku, jumlah_buku)) {
        printf("\nBuku berhasil ditambahkan!\n");
    } else {
        printf("\nGagal menyimpan buku!\n");
    }

    free(daftar_buku);
}

