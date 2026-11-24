#ifndef USER_H
#define USER_H

void menu_user(const char *username);

void tampilkan_katalog_user(void);
void cari_buku(void);
void pinjam_buku(const char *username);
void kembalikan_buku(const char *username);
void riwayat_peminjaman(const char *username);

#endif