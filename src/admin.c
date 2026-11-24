#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "admin.h"
#include "penyimpanan.h"
#include "sorting.h"

static int baca_baris_input(const char *prompt, char *buffer, size_t ukuran) {
    size_t panjang;
    printf("%s", prompt);
    if (fgets(buffer, (int)ukuran, stdin) == NULL) return 0;

    panjang = strcspn(buffer, "\r\n");
    if (buffer[panjang] == '\0' && panjang == ukuran - 1) {
        int karakter;
        while ((karakter = getchar()) != '\n' && karakter != EOF) {}
        printf("Input terlalu panjang.\n");
        return 0;
    }
    buffer[panjang] = '\0';
    return 1;
}

static int baca_integer(const char *prompt, int minimum, int maksimum, int *hasil) {
    char buffer[100];
    char *akhir;
    long nilai;

    if (!baca_baris_input(prompt, buffer, sizeof(buffer))) return 0;
    nilai = strtol(buffer, &akhir, 10);
    if (buffer == akhir) return 0;
    while (*akhir != '\0' && isspace((unsigned char)*akhir)) akhir++;
    if (*akhir != '\0' || nilai < minimum || nilai > maksimum) return 0;
    *hasil = (int)nilai;
    return 1;
}


static int baca_teks_opsional(const char *prompt, char *teks, size_t ukuran) {
    char sementara[MAX_FIELD];
    if (!baca_baris_input(prompt, sementara, sizeof(sementara))) return 0;
    if (sementara[0] == '\0') return 1;
    if (strlen(sementara) >= ukuran) return 0;
    strcpy(teks, sementara);
    return 1;
}

static int baca_integer_opsional(const char *prompt, int nilai_lama, int minimum, int maksimum, int *hasil) {
    char buffer[100];
    char *akhir;
    long nilai;

    if (!baca_baris_input(prompt, buffer, sizeof(buffer))) return 0;
    if (buffer[0] == '\0') {
        *hasil = nilai_lama;
        return 1;
    }
    nilai = strtol(buffer, &akhir, 10);
    if (buffer == akhir) return 0;
    while (*akhir != '\0' && isspace((unsigned char)*akhir)) akhir++;
    if (*akhir != '\0' || nilai < minimum || nilai > maksimum) return 0;
    *hasil = (int)nilai;
    return 1;
}

static int muat_katalog(Buku **buku, int *jumlah) {
    if (!baca_buku(PATH_KATALOG_BUKU, buku, jumlah)) {
        printf("Gagal membaca katalog buku.\n");
        free(*buku);
        *buku = NULL;
        *jumlah = 0;
        return 0;
    }
    return 1;
}

static int cari_indeks_ocaid(const Buku buku[], int jumlah, const char *ocaid) {
    int i;
    for (i = 0; i < jumlah; i++) {
        if (strcmp(buku[i].ocaid, ocaid) == 0) return i;
    }
    return -1;
}

static void tampilkan_detail_buku(const Buku *buku) {
    printf("OCAID       : %s\n", buku->ocaid);
    printf("Judul       : %s\n", buku->judul);
    printf("Penulis     : %s\n", buku->penulis);
    printf("Penerbit    : %s\n", buku->penerbit);
    printf("Tahun       : %d\n", buku->tahun);
    printf("Halaman     : %d\n", buku->halaman);
    printf("Stok        : %d\n", buku->stok);
}

void tambah_buku(void) {
    Buku *daftar_buku = NULL;
    Buku buku_baru;
    int jumlah_buku = 0;
    Buku *data_baru;

    if (!muat_katalog(&daftar_buku, &jumlah_buku)) return;

    printf("\n=== TAMBAH BUKU ===\n");
    if (!baca_baris_input("OCAID buku: ", buku_baru.ocaid, sizeof(buku_baru.ocaid)) ||
        buku_baru.ocaid[0] == '\0') {
        printf("OCAID tidak boleh kosong.\n");
        free(daftar_buku);
        return;
    }
    if (cari_indeks_ocaid(daftar_buku, jumlah_buku, buku_baru.ocaid) >= 0) {
        printf("OCAID sudah terdaftar. Gunakan OCAID yang unik.\n");
        free(daftar_buku);
        return;
    }
    if (!baca_baris_input("Judul buku: ", buku_baru.judul, sizeof(buku_baru.judul)) || buku_baru.judul[0] == '\0' ||
        !baca_baris_input("Penulis: ", buku_baru.penulis, sizeof(buku_baru.penulis)) || buku_baru.penulis[0] == '\0' ||
        !baca_baris_input("Penerbit: ", buku_baru.penerbit, sizeof(buku_baru.penerbit)) || buku_baru.penerbit[0] == '\0' ||
        !baca_integer("Tahun terbit: ", 0, INT_MAX, &buku_baru.tahun) ||
        !baca_integer("Jumlah halaman: ", 0, INT_MAX, &buku_baru.halaman) ||
        !baca_integer("Stok awal: ", 0, INT_MAX, &buku_baru.stok)) {
        printf("Data buku tidak valid atau input dibatalkan.\n");
        free(daftar_buku);
        return;
    }

    data_baru = (Buku *)realloc(daftar_buku, (size_t)(jumlah_buku + 1) * sizeof(Buku));
    if (data_baru == NULL) {
        printf("Memori tidak cukup untuk menambahkan buku.\n");
        free(daftar_buku);
        return;
    }

    daftar_buku = data_baru;
    daftar_buku[jumlah_buku] = buku_baru;
    jumlah_buku++;

    if (simpan_buku(PATH_KATALOG_BUKU, daftar_buku, jumlah_buku))
        printf("Buku berhasil ditambahkan.\n");
    else
        printf("Gagal menyimpan buku.\n");

    free(daftar_buku);
}

void tampilkan_semua_buku(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int i;

    printf("\n=== DAFTAR SEMUA BUKU ===\n");
    if (!muat_katalog(&daftar_buku, &jumlah_buku)) return;
    if (jumlah_buku == 0) printf("Katalog belum memiliki buku.\n");

    for (i = 0; i < jumlah_buku; i++) {
        printf("\nBuku ke-%d\n", i + 1);
        tampilkan_detail_buku(&daftar_buku[i]);
    }
    free(daftar_buku);
}

void edit_buku(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int indeks;
    int tahun, halaman, stok;
    char ocaid[64];

    if (!muat_katalog(&daftar_buku, &jumlah_buku)) return;
    if (!baca_baris_input("Masukkan OCAID buku yang ingin diedit: ", ocaid, sizeof(ocaid))) {
        free(daftar_buku);
        return;
    }

    indeks = cari_indeks_ocaid(daftar_buku, jumlah_buku, ocaid);
    if (indeks < 0) {
        printf("Buku dengan OCAID tersebut tidak ditemukan.\n");
        free(daftar_buku);
        return;
    }

    printf("\nData buku saat ini:\n");
    tampilkan_detail_buku(&daftar_buku[indeks]);
    printf("\nTekan Enter jika ingin mempertahankan nilai lama.\n");

    if (!baca_teks_opsional("Judul baru (kosong = tetap): ", daftar_buku[indeks].judul, sizeof(daftar_buku[indeks].judul)) ||
        !baca_teks_opsional("Penulis baru (kosong = tetap): ", daftar_buku[indeks].penulis, sizeof(daftar_buku[indeks].penulis)) ||
        !baca_teks_opsional("Penerbit baru (kosong = tetap): ", daftar_buku[indeks].penerbit, sizeof(daftar_buku[indeks].penerbit)) ||
        !baca_integer_opsional("Tahun baru (kosong = tetap): ", daftar_buku[indeks].tahun, 0, INT_MAX, &tahun) ||
        !baca_integer_opsional("Jumlah halaman baru (kosong = tetap): ", daftar_buku[indeks].halaman, 0, INT_MAX, &halaman) ||
        !baca_integer_opsional("Stok baru (kosong = tetap): ", daftar_buku[indeks].stok, 0, INT_MAX, &stok)) {
        printf("Input tidak valid. Perubahan dibatalkan.\n");
        free(daftar_buku);
        return;
    }

    /* Input teks kosong berarti pertahankan nilai sebelumnya. */
    if (daftar_buku[indeks].judul[0] == '\0' || daftar_buku[indeks].penulis[0] == '\0' ||
        daftar_buku[indeks].penerbit[0] == '\0') {
        printf("Judul, penulis, dan penerbit tidak boleh kosong.\n");
        free(daftar_buku);
        return;
    }

    daftar_buku[indeks].tahun = tahun;
    daftar_buku[indeks].halaman = halaman;
    daftar_buku[indeks].stok = stok;

    if (simpan_buku(PATH_KATALOG_BUKU, daftar_buku, jumlah_buku))
        printf("Data buku berhasil diperbarui.\n");
    else
        printf("Gagal menyimpan perubahan buku.\n");

    free(daftar_buku);
}

static int buku_masih_dipinjam(const char *ocaid) {
    Peminjaman *transaksi = NULL;
    int jumlah = 0;
    int i;

    if (!baca_peminjaman(PATH_TRANSAKSI, &transaksi, &jumlah)) return -1;
    for (i = 0; i < jumlah; i++) {
        if (strcmp(transaksi[i].ocaid, ocaid) == 0 && strcmp(transaksi[i].status, "DIPINJAM") == 0) {
            free(transaksi);
            return 1;
        }
    }
    free(transaksi);
    return 0;
}

void hapus_buku(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int indeks;
    int i;
    char ocaid[64];
    int status_aktif;

    if (!muat_katalog(&daftar_buku, &jumlah_buku)) return;
    if (!baca_baris_input("Masukkan OCAID buku yang ingin dihapus: ", ocaid, sizeof(ocaid))) {
        free(daftar_buku);
        return;
    }

    indeks = cari_indeks_ocaid(daftar_buku, jumlah_buku, ocaid);
    if (indeks < 0) {
        printf("Buku tidak ditemukan.\n");
        free(daftar_buku);
        return;
    }

    status_aktif = buku_masih_dipinjam(ocaid);
    if (status_aktif != 0) {
        printf(status_aktif == 1 ? "Buku masih dipinjam dan tidak dapat dihapus.\n" :
                                  "Data transaksi gagal dibaca; penghapusan dibatalkan.\n");
        free(daftar_buku);
        return;
    }

    printf("Buku yang akan dihapus: %s\n", daftar_buku[indeks].judul);
    for (i = indeks; i < jumlah_buku - 1; i++) daftar_buku[i] = daftar_buku[i + 1];
    jumlah_buku--;

    if (simpan_buku(PATH_KATALOG_BUKU, daftar_buku, jumlah_buku))
        printf("Buku berhasil dihapus.\n");
    else
        printf("Gagal menyimpan perubahan katalog.\n");

    free(daftar_buku);
}

void laporan_transaksi(void) {
    Peminjaman *daftar = NULL;
    int jumlah = 0;
    int i;

    printf("\n=== LAPORAN TRANSAKSI SELURUH PENGGUNA ===\n");
    if (!baca_peminjaman(PATH_TRANSAKSI, &daftar, &jumlah)) {
        printf("Gagal membaca data transaksi.\n");
        return;
    }
    if (jumlah == 0) printf("Belum ada transaksi.\n");

    for (i = 0; i < jumlah; i++) {
        printf("\nTransaksi ke-%d\n", i + 1);
        printf("ID Transaksi   : %u\n", daftar[i].id_transaksi);
        printf("Username       : %s\n", daftar[i].username);
        printf("OCAID          : %s\n", daftar[i].ocaid);
        printf("Judul Buku     : %s\n", daftar[i].judul_buku);
        printf("Tanggal Pinjam : %s\n", daftar[i].tanggal_pinjam);
        printf("Tanggal Kembali: %s\n", daftar[i].tanggal_kembali);
        printf("Status         : %s\n", daftar[i].status);
    }
    free(daftar);
}

void menu_sorting_admin(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;

    if (!muat_katalog(&daftar_buku, &jumlah_buku)) return;
    menu_sorting_katalog(daftar_buku, jumlah_buku);
    free(daftar_buku);
}

void menu_admin(void) {
    char baris[80];
    int pilihan;

    for (;;) {
        char *akhir;
        long nilai;

        printf("\n===== MENU ADMIN =====\n");
        printf("1. Tambah Buku\n");
        printf("2. Tampilkan Semua Buku\n");
        printf("3. Edit Buku\n");
        printf("4. Hapus Buku\n");
        printf("5. Laporan Transaksi\n");
        printf("6. Sorting Katalog\n");
        printf("0. Logout\n");
        printf("Pilih menu: ");

        if (fgets(baris, sizeof(baris), stdin) == NULL) break;
        nilai = strtol(baris, &akhir, 10);
        if (baris == akhir) {
            printf("Input harus berupa angka.\n");
            continue;
        }
        while (*akhir != '\0' && isspace((unsigned char)*akhir)) akhir++;
        if (*akhir != '\0' || nilai < 0 || nilai > 6) {
            printf("Pilihan tidak valid. Masukkan angka 0-6.\n");
            continue;
        }
        pilihan = (int)nilai;

        switch (pilihan) {
            case 1: tambah_buku(); break;
            case 2: tampilkan_semua_buku(); break;
            case 3: edit_buku(); break;
            case 4: hapus_buku(); break;
            case 5: laporan_transaksi(); break;
            case 6: menu_sorting_admin(); break;
            case 0: printf("Logout berhasil.\n"); return;
        }
    }
}
