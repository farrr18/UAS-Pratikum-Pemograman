#include <stdio.h>
#include <stdlid.h>
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
                printf("Menu pencarian buku\n");
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
        printf("Belum ada buku dalma katalog.\n");
        free(daftar_buku);
        return;
    }

    for(int i = 0; i <jumlah_buku; i++){
        printf("\nBuku ke-%d\n", i + 1);
        printf("ID Buku        : %d\n", daftar_buku[i].id);
        printf("Judul Buku     : %d\n", daftar_buku[i].judul);
        printf("Penulis        : %d\n", daftar_buku[i].penulis);
        printf("Tahun Terbit   : %d\n", daftar_buku[i].tahun);
        printf("Jumlah Halaman : %d\n", daftar_buku[i].halaman);
        printf("Stok Buku      : &d\n", daftar_buku[i].stok);
    }
    free(daftar_buku);
}