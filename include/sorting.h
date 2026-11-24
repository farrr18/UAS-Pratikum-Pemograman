#ifndef SORTING_H
#define SORTING_H

#include "penyimpanan.h"

void insertion_sort_ocaid(Buku daftar_buku[], int jumlah_buku, int ascending);
void insertion_sort_judul(Buku daftar_buku[], int jumlah_buku, int ascending);

void bubble_sort_tahun(Buku daftar_buku[], int jumlah_buku, int ascending);

void selection_sort_halaman(Buku daftar_buku[], int jumlah_buku, int ascending);
void selection_sort_stok(Buku daftar_buku[], int jumlah_buku, int ascending);

void menu_sorting_katalog(Buku daftar_buku[], int jumlah_buku);

#endif
