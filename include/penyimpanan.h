#ifndef PENYIMPANAN_H
#define PENYIMPANAN_H

#include <stdio.h>

#define MAX_FIELD 2048
#define PATH_DATASET_BUKU "data/Books.txt"
#define PATH_KATALOG_BUKU "data/katalog.txt"
#define PATH_AKUN_USER "data/user.txt"
#define PATH_TRANSAKSI "data/peminjaman.txt"

typedef struct {
    char ocaid[64];
    char judul[MAX_FIELD];
    char penulis[1024];
    char penerbit[256];
    int tahun;
    int halaman;
    int stok;
} Buku;

typedef struct {
    char username[32];
    char password[32];
    char role[12];
} User;

typedef struct {
    unsigned int id_transaksi;
    char username[32];
    char ocaid[64];
    char judul_buku[MAX_FIELD];
    char tanggal_pinjam[20];
    char tanggal_kembali[20];
    char status[20];
} Peminjaman;

int pecah_baris(const char *baris, char kolom[][MAX_FIELD], int max_kolom);
int baca_buku(const char *nama_file, Buku **daftar_buku, int *jumlah_buku);
int baca_user(const char *nama_file, User **daftar_user, int *jumlah_user);
int baca_peminjaman(const char *nama_file, Peminjaman **daftar_peminjaman, int *jumlah_peminjaman);
int simpan_buku(const char *nama_file, const Buku *daftar_buku, int jumlah_buku);
int simpan_peminjaman(const char *nama_file, const Peminjaman *daftar_pinjaman, int jumlah_pinjaman);
int buat_katalog_awal(const char *file_asli, const char *file_katalog);
int muat_data(Buku **daftar_buku, int *jumlah_buku,
              User **daftar_user, int *jumlah_user,
              Peminjaman **daftar_pinjam, int *jumlah_pinjam);
void bebaskan_memori(Buku **daftar_buku, int *jumlah_buku,
                     User **daftar_user, int *jumlah_user,
                     Peminjaman **daftar_pinjam, int *jumlah_pinjam);

#endif