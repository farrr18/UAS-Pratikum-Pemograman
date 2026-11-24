#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sorting.h"
#include "penyimpanan.h"
#include "user.h"

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

static int pastikan_file_transaksi(void) {
    FILE *fp = fopen(PATH_TRANSAKSI, "a");
    if (fp == NULL) return 0;
    return fclose(fp) == 0;
}

static int baca_katalog(Buku **daftar_buku, int *jumlah_buku) {
    if (!baca_buku(PATH_KATALOG_BUKU, daftar_buku, jumlah_buku)) {
        printf("Gagal membaca katalog buku.\n");
        free(*daftar_buku);
        *daftar_buku = NULL;
        *jumlah_buku = 0;
        return 0;
    }
    if (*jumlah_buku == 0) {
        printf("Katalog buku masih kosong.\n");
        free(*daftar_buku);
        *daftar_buku = NULL;
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

static void tampilkan_buku(const Buku *buku) {
    printf("OCAID       : %s\n", buku->ocaid);
    printf("Judul       : %s\n", buku->judul);
    printf("Penulis     : %s\n", buku->penulis);
    printf("Penerbit    : %s\n", buku->penerbit);
    printf("Tahun       : %d\n", buku->tahun);
    printf("Halaman     : %d\n", buku->halaman);
    printf("Stok        : %d\n", buku->stok);
}

void menu_user(const char *username) {
    char baris[80];

    for (;;) {
        char *akhir;
        long nilai;

        printf("\n=== MENU PENGGUNA ===\n");
        printf("Selamat datang, %s!\n", username);
        printf("1. Tampilkan Katalog Buku\n");
        printf("2. Cari Buku\n");
        printf("3. Pinjam Buku\n");
        printf("4. Kembalikan Buku\n");
        printf("5. Daftar Peminjaman Aktif\n");
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

        switch ((int)nilai) {
            case 1: tampilkan_katalog_user(); break;
            case 2: cari_buku(); break;
            case 3: pinjam_buku(username); break;
            case 4: kembalikan_buku(username); break;
            case 5: riwayat_peminjaman(username); break;
            case 6: {
                Buku *buku = NULL;
                int jumlah = 0;
                if (baca_katalog(&buku, &jumlah)) {
                    menu_sorting_katalog(buku, jumlah);
                    free(buku);
                }
                break;
            }
            case 0:
                printf("Logout berhasil.\n");
                return;
        }
    }
}

void tampilkan_katalog_user(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int i;

    printf("\n=== DAFTAR KATALOG BUKU ===\n");
    if (!baca_katalog(&daftar_buku, &jumlah_buku)) return;

    for (i = 0; i < jumlah_buku; i++) {
        printf("\nBuku ke-%d\n", i + 1);
        tampilkan_buku(&daftar_buku[i]);
    }
    free(daftar_buku);
}

void cari_buku(void) {
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int pilihan, i, ditemukan = 0;
    char kata_kunci[MAX_FIELD];

    printf("\n=== CARI BUKU ===\n");
    printf("1. Cari berdasarkan OCAID\n");
    printf("2. Cari berdasarkan judul\n");
    printf("3. Cari berdasarkan penulis (tambahan)\n");
    if (!baca_integer("Pilihan pencarian: ", 1, 3, &pilihan)) {
        printf("Pilihan pencarian tidak valid.\n");
        return;
    }
    if (!baca_katalog(&daftar_buku, &jumlah_buku)) return;

    if (!baca_baris_input("Masukkan kata kunci: ", kata_kunci, sizeof(kata_kunci)) || kata_kunci[0] == '\0') {
        printf("Kata kunci tidak boleh kosong.\n");
        free(daftar_buku);
        return;
    }

    for (i = 0; i < jumlah_buku; i++) {
        int cocok = 0;
        if (pilihan == 1) cocok = strcmp(daftar_buku[i].ocaid, kata_kunci) == 0;
        else if (pilihan == 2) cocok = strstr(daftar_buku[i].judul, kata_kunci) != NULL;
        else cocok = strstr(daftar_buku[i].penulis, kata_kunci) != NULL;

        if (cocok) {
            ditemukan = 1;
            printf("\nBuku ditemukan:\n");
            tampilkan_buku(&daftar_buku[i]);
        }
    }
    if (!ditemukan) printf("Buku tidak ditemukan.\n");
    free(daftar_buku);
}

void pinjam_buku(const char *username) {
    Buku *buku = NULL;
    Peminjaman *transaksi = NULL;
    int jumlah_buku = 0, jumlah_transaksi = 0;
    int indeks_buku, i;
    unsigned int id_transaksi = 1;
    char ocaid[64];
    Peminjaman baru;
    time_t sekarang;
    struct tm *waktu;

    printf("\n=== PEMINJAMAN BUKU ===\n");
    if (!baca_katalog(&buku, &jumlah_buku)) return;
    if (!baca_baris_input("Masukkan OCAID buku yang ingin dipinjam: ", ocaid, sizeof(ocaid))) {
        free(buku);
        return;
    }

    indeks_buku = cari_indeks_ocaid(buku, jumlah_buku, ocaid);
    if (indeks_buku < 0) {
        printf("Buku dengan OCAID tersebut tidak ditemukan.\n");
        free(buku);
        return;
    }
    if (buku[indeks_buku].stok <= 0) {
        printf("Stok buku sedang habis.\n");
        free(buku);
        return;
    }
    if (!pastikan_file_transaksi() || !baca_peminjaman(PATH_TRANSAKSI, &transaksi, &jumlah_transaksi)) {
        printf("Gagal membaca atau menyiapkan file transaksi.\n");
        free(buku);
        free(transaksi);
        return;
    }

    for (i = 0; i < jumlah_transaksi; i++) {
        if (transaksi[i].id_transaksi >= id_transaksi) {
            if (transaksi[i].id_transaksi == UINT_MAX) {
                printf("Nomor transaksi sudah mencapai batas maksimum.\n");
                free(buku);
                free(transaksi);
                return;
            }
            id_transaksi = transaksi[i].id_transaksi + 1;
        }
    }

    {
        Peminjaman *baru_alokasi = (Peminjaman *)realloc(
            transaksi, (size_t)(jumlah_transaksi + 1) * sizeof(Peminjaman));
        if (baru_alokasi == NULL) {
            printf("Memori tidak cukup untuk membuat transaksi.\n");
            free(buku);
            free(transaksi);
            return;
        }
        transaksi = baru_alokasi;
    }

    memset(&baru, 0, sizeof(baru));
    baru.id_transaksi = id_transaksi;
    snprintf(baru.username, sizeof(baru.username), "%s", username);
    snprintf(baru.ocaid, sizeof(baru.ocaid), "%s", buku[indeks_buku].ocaid);
    snprintf(baru.judul_buku, sizeof(baru.judul_buku), "%s", buku[indeks_buku].judul);

    sekarang = time(NULL);
    waktu = localtime(&sekarang);
    if (waktu == NULL || strftime(baru.tanggal_pinjam, sizeof(baru.tanggal_pinjam), "%d-%m-%Y", waktu) == 0) {
        printf("Gagal mendapatkan tanggal peminjaman.\n");
        free(buku);
        free(transaksi);
        return;
    }
    strcpy(baru.tanggal_kembali, "-");
    strcpy(baru.status, "DIPINJAM");

    transaksi[jumlah_transaksi] = baru;
    jumlah_transaksi++;
    buku[indeks_buku].stok--;

    if (!simpan_buku(PATH_KATALOG_BUKU, buku, jumlah_buku)) {
        printf("Gagal menyimpan perubahan stok.\n");
        free(buku);
        free(transaksi);
        return;
    }
    if (!simpan_peminjaman(PATH_TRANSAKSI, transaksi, jumlah_transaksi)) {
        buku[indeks_buku].stok++;
        simpan_buku(PATH_KATALOG_BUKU, buku, jumlah_buku);
        printf("Gagal menyimpan transaksi. Perubahan stok dibatalkan.\n");
        free(buku);
        free(transaksi);
        return;
    }

    printf("Peminjaman berhasil. ID transaksi: %u\n", id_transaksi);
    printf("Judul: %s\nSisa stok: %d\n", buku[indeks_buku].judul, buku[indeks_buku].stok);
    free(buku);
    free(transaksi);
}

void kembalikan_buku(const char *username) {
    Buku *buku = NULL;
    Peminjaman *transaksi = NULL;
    int jumlah_buku = 0, jumlah_transaksi = 0;
    int indeks_transaksi = -1, indeks_buku = -1;
    int i, id_input;
    time_t sekarang;
    struct tm *waktu;
    char tanggal_kembali[20];

    printf("\n=== PENGEMBALIAN BUKU ===\n");
    if (!pastikan_file_transaksi() || !baca_peminjaman(PATH_TRANSAKSI, &transaksi, &jumlah_transaksi)) {
        printf("Gagal membaca data transaksi.\n");
        free(transaksi);
        return;
    }

    printf("\n=== BUKU YANG MASIH DIPINJAM ===\n");
    for (i = 0; i < jumlah_transaksi; i++) {
        if (strcmp(transaksi[i].username, username) == 0 && strcmp(transaksi[i].status, "DIPINJAM") == 0) {
            printf("ID Transaksi: %u | OCAID: %s | Judul: %s | Tanggal: %s\n",
                   transaksi[i].id_transaksi, transaksi[i].ocaid,
                   transaksi[i].judul_buku, transaksi[i].tanggal_pinjam);
        }
    }

    if (!baca_integer("Masukkan ID transaksi yang ingin dikembalikan: ", 1, INT_MAX, &id_input)) {
        printf("ID transaksi tidak valid.\n");
        free(transaksi);
        return;
    }

    for (i = 0; i < jumlah_transaksi; i++) {
        if (transaksi[i].id_transaksi == (unsigned int)id_input &&
            strcmp(transaksi[i].username, username) == 0) {
            indeks_transaksi = i;
            break;
        }
    }
    if (indeks_transaksi < 0 || strcmp(transaksi[indeks_transaksi].status, "DIPINJAM") != 0) {
        printf("Transaksi tidak ditemukan, bukan milik Anda, atau sudah dikembalikan.\n");
        free(transaksi);
        return;
    }

    if (!baca_katalog(&buku, &jumlah_buku)) {
        free(transaksi);
        return;
    }
    indeks_buku = cari_indeks_ocaid(buku, jumlah_buku, transaksi[indeks_transaksi].ocaid);
    if (indeks_buku < 0) {
        printf("Buku tidak ditemukan pada katalog. Pengembalian dibatalkan.\n");
        free(buku);
        free(transaksi);
        return;
    }

    sekarang = time(NULL);
    waktu = localtime(&sekarang);
    if (waktu == NULL || strftime(tanggal_kembali, sizeof(tanggal_kembali), "%d-%m-%Y", waktu) == 0) {
        printf("Gagal mendapatkan tanggal pengembalian.\n");
        free(buku);
        free(transaksi);
        return;
    }

    buku[indeks_buku].stok++;
    strcpy(transaksi[indeks_transaksi].tanggal_kembali, tanggal_kembali);
    strcpy(transaksi[indeks_transaksi].status, "KEMBALI");

    if (!simpan_buku(PATH_KATALOG_BUKU, buku, jumlah_buku)) {
        printf("Gagal menyimpan stok buku.\n");
        free(buku);
        free(transaksi);
        return;
    }
    if (!simpan_peminjaman(PATH_TRANSAKSI, transaksi, jumlah_transaksi)) {
        buku[indeks_buku].stok--;
        strcpy(transaksi[indeks_transaksi].tanggal_kembali, "-");
        strcpy(transaksi[indeks_transaksi].status, "DIPINJAM");
        simpan_buku(PATH_KATALOG_BUKU, buku, jumlah_buku);
        printf("Gagal menyimpan transaksi pengembalian. Perubahan dibatalkan.\n");
        free(buku);
        free(transaksi);
        return;
    }

    printf("Pengembalian berhasil. Judul: %s | Stok sekarang: %d\n",
           transaksi[indeks_transaksi].judul_buku, buku[indeks_buku].stok);
    free(buku);
    free(transaksi);
}

void riwayat_peminjaman(const char *username) {
    Peminjaman *transaksi = NULL;
    int jumlah = 0, i, ditemukan = 0;

    if (!pastikan_file_transaksi() || !baca_peminjaman(PATH_TRANSAKSI, &transaksi, &jumlah)) {
        printf("Gagal membaca data transaksi.\n");
        free(transaksi);
        return;
    }

    printf("\n=== DAFTAR PEMINJAMAN AKTIF ===\n");
    for (i = 0; i < jumlah; i++) {
        if (strcmp(transaksi[i].username, username) == 0 && strcmp(transaksi[i].status, "DIPINJAM") == 0) {
            ditemukan = 1;
            printf("\nID Transaksi   : %u\n", transaksi[i].id_transaksi);
            printf("OCAID          : %s\n", transaksi[i].ocaid);
            printf("Judul Buku     : %s\n", transaksi[i].judul_buku);
            printf("Tanggal Pinjam : %s\n", transaksi[i].tanggal_pinjam);
            printf("Status         : %s\n", transaksi[i].status);
        }
    }
    if (!ditemukan) printf("Tidak ada buku yang sedang Anda pinjam.\n");
    free(transaksi);
}
