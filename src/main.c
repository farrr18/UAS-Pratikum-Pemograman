#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "penyimpanan.h"
#include "admin.h"
#include "user.h"

#ifdef _WIN32
#include <windows.h>
#endif

static int baca_teks_login(const char *prompt, char *buffer, size_t ukuran) {
    size_t panjang;
    printf("%s", prompt);
    if (fgets(buffer, (int)ukuran, stdin) == NULL) return 0;
    panjang = strcspn(buffer, "\r\n");
    if (buffer[panjang] == '\0' && panjang == ukuran - 1) {
        int karakter;
        while ((karakter = getchar()) != '\n' && karakter != EOF) {}
        return 0;
    }
    buffer[panjang] = '\0';
    return buffer[0] != '\0';
}

static int login(const User daftar_user[], int jumlah_user,
                 char username_login[32], char role_login[12]) {
    char password[32];
    int i;

    printf("\n=== LOGIN PERPUSTAKAAN ===\n");
    if (!baca_teks_login("Username: ", username_login, 32) ||
        !baca_teks_login("Password: ", password, sizeof(password))) {
        printf("Username atau password tidak valid.\n");
        return 0;
    }

    for (i = 0; i < jumlah_user; i++) {
        if (strcmp(username_login, daftar_user[i].username) == 0 &&
            strcmp(password, daftar_user[i].password) == 0) {
            strcpy(role_login, daftar_user[i].role);
            return 1;
        }
    }

    printf("Username atau password salah.\n");
    return 0;
}

static int pastikan_file_transaksi(void) {
    FILE *fp = fopen(PATH_TRANSAKSI, "a");
    if (fp == NULL) return 0;
    return fclose(fp) == 0;
}

int main(void) {
    Buku *daftar_buku = NULL;
    User *daftar_user = NULL;
    Peminjaman *daftar_pinjam = NULL;
    int jumlah_buku = 0;
    int jumlah_user = 0;
    int jumlah_pinjam = 0;
    char username_login[32];
    char role_login[12];

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (!buat_katalog_awal(PATH_DATASET_BUKU, PATH_KATALOG_BUKU)) {
        printf("Gagal menyiapkan katalog perpustakaan.\n");
        return 1;
    }

    if (!pastikan_file_transaksi()) {
        printf("Gagal menyiapkan file transaksi.\n");
        return 1;
    }

    /* Memuat katalog, akun, dan transaksi ke memori saat inisialisasi. */
    if (!muat_data(&daftar_buku, &jumlah_buku,
                   &daftar_user, &jumlah_user,
                   &daftar_pinjam, &jumlah_pinjam)) {
        printf("Gagal memuat data perpustakaan. Periksa file dalam folder data/.\n");
        bebaskan_memori(&daftar_buku, &jumlah_buku,
                        &daftar_user, &jumlah_user,
                        &daftar_pinjam, &jumlah_pinjam);
        return 1;
    }

    if (jumlah_user == 0) {
        printf("Belum ada akun yang terdaftar di data/user.txt.\n");
    } else if (login(daftar_user, jumlah_user, username_login, role_login)) {
        if (strcmp(role_login, "admin") == 0) {
            printf("\nLogin sebagai Admin berhasil.\n");
            menu_admin();
        } else if (strcmp(role_login, "user") == 0) {
            printf("\nLogin sebagai User berhasil.\n");
            menu_user(username_login);
        } else {
            printf("Role akun tidak dikenal.\n");
        }
    }

    bebaskan_memori(&daftar_buku, &jumlah_buku,
                    &daftar_user, &jumlah_user,
                    &daftar_pinjam, &jumlah_pinjam);
    return 0;
}