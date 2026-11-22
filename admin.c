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

void tampilkan_semua_buku(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;

    printf("\n=== DAFTAR SEMUA BUKU ===\n");
    if(!baca_buku(FILE_BUKU, &daftar_buku, &jumlah_buku)) {
        printf("Gagal membaca data buku!\n");
        free(daftar_buku);
        return;
    }

    if (jumlah_buku == 0) {
        printf("Belum ada buku.\n");
        free(daftar_buku);
        return;
    }

    for (int i = 0; i < jumlah_buku; i++){
        printf("\nBuku ke-%d\n", i + 1);
        printf("ID Buku      : %d\n", daftar_buku[i].id);
        printf("Judul Buku   : %s\n", daftar_buku[i].judul);
        printf("Penulis      : %s\n", daftar_buku[i].penulis);
        printf("Tahun terbit : %d\n", daftar_buku[i].tahun);
        printf("Jumlah Halaman: %d\n", daftar_buku[i].halaman);
        printf("Stok Buku    : %d\n", daftar_buku[i].stok);
        
    }

    free(daftar_buku);
}

void edit_buku(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int id_cari;
    int ditemukan = 0;

    if(!baca_buku(FILE_BUKU, &daftar_buku, &jumlah_buku)) {
        printf("Gagal membaca data buku!\n");
        free(daftar_buku);
        return;
}

        printf("\n=== EDIT BUKU ===\n");
        printf("Masukkan ID buku yang ingin diedit: ");
        scanf("%d", &id_cari);
        getchar();

        for(int i = 0; i < jumlah_buku; i++){
            if(daftar_buku[i].id == id_cari) {
                ditemukan = 1;

            printf("\nData buku ditemukan!\n");
            printf("Judul lama   : %s\n", daftar_buku[i].judul);
            printf("Penulis lama : %s\n", daftar_buku[i].penulis);

            printf("\nMasukkan data baru:\n");

            printf("Judul buku   : ");
            fgets(daftar_buku[i].judul, sizeof(daftar_buku[i].judul), stdin); 
            daftar_buku[i].judul [strcspn(daftar_buku[i].judul, "\n")] = '\0';

            printf("Penulis   : ");
            fgets(daftar_buku[i].penulis, sizeof(daftar_buku[i].penulis), stdin); 
            daftar_buku[i].penulis [strcspn(daftar_buku[i].penulis, "\n")] = '\0';

            printf("Tahun Terbit : ");
            scanf("%d", &daftar_buku[i].tahun);

            printf("Jumlah Halaman : ");
            scanf("%d", &daftar_buku[i].halaman);

            printf("Stok buku : ");
            scanf("%d", &daftar_buku[i].stok);

            if (simpan_buku(FILE_BUKU, daftar_buku, jumlah_buku)) {
                printf("\nBuku berhasil diperbarui!\n");
            } else {
                printf("\nGagal menyimpan perubahan buku!\n");
            }

            break;
        }
    }

    if (!ditemukan) {
        printf("\nBuku dengan ID %d tidak ditemukan!\n", id_cari);
    }

    free(daftar_buku);
}

void hapus_buku(void){
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int id_hapus;
    int ditemukan = 0;

    if (!baca_buku(FILE_BUKU, &daftar_buku, &jumlah_buku)) {
        printf("Gagal membaca data buku!\n");
        free(daftar_buku);
        return;
    }

    printf("\n=== HAPUS BUKU ===\n");
    printf("Masukkan ID buku yang ingin dihapus: ");
    scanf("%d", &id_hapus);
     
    for (int i = 0; i < jumlah_buku; i++) {
        if (daftar_buku[i].id == id_hapus) {
            ditemukan = 1;

            printf("\nBuku ditemukan!\n");
            printf("Judul Buku : %s\n", daftar_buku[i].judul);
            printf("Penulis    : %s\n", daftar_buku[i].penulis);

            for(int j = i; j < jumlah_buku - 1; j++){
                daftar_buku[j] = daftar_buku[j + 1];
            }

            jumlah_buku--;

            if (simpan_buku(FILE_BUKU, daftar_buku, jumlah_buku)) {
                printf("\nBuku berhasil dihapus!\n");
            } else {
                printf("\nGagal menghapus buku!\n");
            }

            break;
        }
               
    }

    if (!ditemukan) {
        printf("\nBuku dengan ID %d tidak ditemukan!\n", id_hapus);
    }
    
    free(daftar_buku);
}

    
