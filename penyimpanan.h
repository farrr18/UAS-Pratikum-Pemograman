#ifndef PENYIMPANAN_H
#define PENYIMPANAN_H
#include <stdio.h>

typedef struct{
int id, tahun, halaman, stok;
char judul[256], penulis[150];
} Buku;

typedef struct{
    char username[50], password[50], role[20];
} User;

typedef struct{
    int id_transaksi, id_buku;
    char username[50], judul_buku[256], tanggal_pinjam[20], tanggal_kembali[20], status[20];
} Peminjaman;

int pecah_baris(const char *baris, char kolom[][256], int max_kolom);
int baca_buku(const char *nama_file, Buku **daftar_buku, int *jumlah_buku);
int baca_user(const char *nama_file, User **daftar_user, int *jumlah_user);
int baca_peminjaman(const char *nama_file, Peminjaman **daftar_pinjaman, int *jumlah_pinjaman);
int simpan_buku(const char *nama_file, Buku *daftar_buku, int jumlah_buku);
int simpan_peminjaman(const char *nama_file, Peminjaman *daftar_pinjaman, int jumlah_pinjaman);
int muat_data(Buku **daftar_buku, int *jumlah_buku, User **daftar_user, int *jumlah_user, Peminjaman **daftar_pinjam, int *jumlah_pinjam);
void bebaskan_memori(Buku **daftar_buku, int *jumlah_buku, User **daftar_user, int *jumlah_user, Peminjaman **daftar_pinjam, int *jumlah_pinjam);
int buat_katalog_awal(const char *file_asli, const char *file_katalog);

#endif