#include <stdio.h>
#include "user.h"

void menu_user(const char *username){
    int pilihan;

    do{
        printf("\n=== MENU PENGGUNA ==\n");
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
                printf("Menu Katalog buku\n");
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