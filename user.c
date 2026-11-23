#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "penyimpanan.h"
#include "user.h"

void menu_user(const char *username){
    int pilihan;

    do{
        printf("\n=== MENU PENGGUNA ===\n");
        printf("Selamat datang, %s !\n", username);
        printf("1. Tampilkan Katalog Buku\n");
        printf("2. Cari Buku\n");
        printf("3. Pinjam Buku\n");
        printf("4. Kembalikan Buku\n");
        printf("5. Riwayat Peminjaman\n");
        printf("0. Logout\n");

        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan){
            case 1:
                tampilkan_katalog_user();
                break;

            case 2:
                cari_buku();
                break;

            case 3:
                printf("Menu peminjaman buku\n");
                break;

            case 4:
                printf("Menu pegembalian buku\n");
                break;

            case 5:
                printf("Menu riwayat peminjaman\n");
                break;

            case 0:
                printf("Berhasil logout!\n");
                break;

            default:
                printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 0);
}

void tampilkan_katalog_user(void){
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;

    printf("\n=== DAFTAR KATALOG BUKU ===\n");

    if(!baca_buku("katalog.txt", &daftar_buku, &jumlah_buku)){
        printf("Gagal membaca katalog buku!\n");
        free(daftar_buku);
        return;
    }

    if (jumlah_buku == 0){
        printf("Belum ada buku dalam katalog.\n");
        free(daftar_buku);
        return;
    }

    for(int i = 0; i <jumlah_buku; i++){
        printf("\nBuku ke-%d\n", i + 1);
        printf("ID Buku        : %d\n", daftar_buku[i].id);
        printf("Judul Buku     : %s\n", daftar_buku[i].judul);
        printf("Penulis        : %s\n", daftar_buku[i].penulis);
        printf("Tahun Terbit   : %d\n", daftar_buku[i].tahun);
        printf("Jumlah Halaman : %d\n", daftar_buku[i].halaman);
        printf("Stok Buku      : %d\n", daftar_buku[i].stok);
    }
    free(daftar_buku);
}

void cari_buku(void){
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int pilihan;
    int id_cari;
    char kata_kunci[256];
    int ditemukan = 0;

    printf("\n=== CARI BUKU ===\n");
    printf("1. Cari bedasarkan ID\n");
    printf("2. Cari berdsarkan Judul\n");
    printf("3. Cari berdasarkan Penulis\n");
    printf("Pilihan motide pencarian: ");
    scanf("%d", &pilihan);
    getchar();

    if (!baca_buku("katalog.txt", &daftar_buku, &jumlah_buku)){
        printf("Gagal membaca katalog buku!\n");
        free(daftar_buku);
        return;
    }

    if (jumlah_buku == 0){
        printf("Katalog buku masih kosong.\n");
        free(daftar_buku);
        return;
    }

    if (pilihan == 1){
        printf("Masukan ID buku: ");
        scanf("%d", &id_cari);
    }
    else if (pilihan == 2){
        printf("Masukkan judul buku: ");
        fgets(kata_kunci, sizeof(kata_kunci), stdin);
        kata_kunci[strcspn(kata_kunci, "\n")] = '\0';
    }
    else if (pilihan == 3){
        printf("Masukan nama penulis: ");
        fgets(kata_kunci, sizeof(kata_kunci), stdin);
        kata_kunci[strcspn(kata_kunci, "\n")] = '\0';
    }
    else {
        printf("Pilihan pencarian tidak valid!\n");
        free(daftar_buku);
        return;
    }

    for (int i = 0; i < jumlah_buku; i++){
        int cocok = 0;

        if(pilihan == 1 && daftar_buku[i].id == id_cari){
            cocok = 1;
        }
        else if(pilihan == 2 && strstr(daftar_buku[i].judul, kata_kunci) != NULL){
            cocok = 1;
        }
        else if (pilihan == 3 && strstr(daftar_buku[i].penulis, kata_kunci) != NULL){
            cocok = 1;
        }

        if (cocok){
            ditemukan = 1;

            printf("\nBuku ditemukan!\n");
            printf("ID Buku         : %d\n", daftar_buku[i].id);
            printf("Judul buku      : %s\n", daftar_buku[i].judul);
            printf("Penulis         : %s\n", daftar_buku[i].penulis);
            printf("Tahun terbit    : %d\n", daftar_buku[i].tahun);
            printf("Jumlah Halaman  : %d\n", daftar_buku[i].halaman);
            printf("Stok Buku       : %d\n", daftar_buku[i].stok);
        }
    }

    if(!ditemukan){
        printf("\nBuku yang dicari tidak ditemukan.\n");
    }

    free(daftar_buku); 
}