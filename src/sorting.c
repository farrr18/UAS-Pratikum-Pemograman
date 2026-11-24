#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sorting.h"

static int bandingkan_teks_tanpa_huruf_besar(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        unsigned char ca = (unsigned char)*a;
        unsigned char cb = (unsigned char)*b;
        int la = tolower(ca);
        int lb = tolower(cb);
        if (la != lb) return la - lb;
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

static int perlu_tukar(int hasil_banding, int ascending) {
    return ascending ? hasil_banding > 0 : hasil_banding < 0;
}

void insertion_sort_ocaid(Buku data[], int jumlah, int ascending) {
    int i;
    if (data == NULL || jumlah <= 1) return;

    for (i = 1; i < jumlah; i++) {
        Buku kunci = data[i];
        int j = i - 1;
        while (j >= 0 && perlu_tukar(strcmp(data[j].ocaid, kunci.ocaid), ascending)) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = kunci;
    }
}

void insertion_sort_judul(Buku data[], int jumlah, int ascending) {
    int i;
    if (data == NULL || jumlah <= 1) return;

    for (i = 1; i < jumlah; i++) {
        Buku kunci = data[i];
        int j = i - 1;
        while (j >= 0 && perlu_tukar(bandingkan_teks_tanpa_huruf_besar(data[j].judul, kunci.judul), ascending)) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = kunci;
    }
}

void bubble_sort_tahun(Buku data[], int jumlah, int ascending) {
    int i, j;
    if (data == NULL || jumlah <= 1) return;

    for (i = 0; i < jumlah - 1; i++) {
        int ada_tukar = 0;
        for (j = 0; j < jumlah - i - 1; j++) {
            int harus_tukar = ascending
                ? data[j].tahun > data[j + 1].tahun
                : data[j].tahun < data[j + 1].tahun;
            if (harus_tukar) {
                Buku sementara = data[j];
                data[j] = data[j + 1];
                data[j + 1] = sementara;
                ada_tukar = 1;
            }
        }
        if (!ada_tukar) break;
    }
}

static void selection_sort_angka(Buku data[], int jumlah, int ascending, int berdasarkan_halaman) {
    int i, j;
    if (data == NULL || jumlah <= 1) return;

    for (i = 0; i < jumlah - 1; i++) {
        int indeks = i;
        for (j = i + 1; j < jumlah; j++) {
            int nilai_j = berdasarkan_halaman ? data[j].halaman : data[j].stok;
            int nilai_indeks = berdasarkan_halaman ? data[indeks].halaman : data[indeks].stok;
            if (ascending ? nilai_j < nilai_indeks : nilai_j > nilai_indeks) indeks = j;
        }
        if (indeks != i) {
            Buku sementara = data[i];
            data[i] = data[indeks];
            data[indeks] = sementara;
        }
    }
}

void selection_sort_halaman(Buku data[], int jumlah, int ascending) {
    selection_sort_angka(data, jumlah, ascending, 1);
}

void selection_sort_stok(Buku data[], int jumlah, int ascending) {
    selection_sort_angka(data, jumlah, ascending, 0);
}

static int baca_pilihan(const char *prompt, int minimum, int maksimum, int *hasil) {
    char baris[80];
    char *akhir;
    long nilai;

    printf("%s", prompt);
    if (fgets(baris, sizeof(baris), stdin) == NULL) return -1;

    nilai = strtol(baris, &akhir, 10);
    if (baris == akhir) return 0;
    while (*akhir != '\0' && isspace((unsigned char)*akhir)) akhir++;
    if (*akhir != '\0' || nilai < minimum || nilai > maksimum) return 0;

    *hasil = (int)nilai;
    return 1;
}

static void tampilkan_hasil(const Buku data[], int jumlah) {
    int i;
    printf("\n=== HASIL PENGURUTAN KATALOG ===\n");
    for (i = 0; i < jumlah; i++) {
        printf("\nOCAID       : %s\n", data[i].ocaid);
        printf("Judul       : %s\n", data[i].judul);
        printf("Penulis     : %s\n", data[i].penulis);
        printf("Penerbit    : %s\n", data[i].penerbit);
        printf("Tahun       : %d\n", data[i].tahun);
        printf("Halaman     : %d\n", data[i].halaman);
        printf("Stok        : %d\n", data[i].stok);
    }
}

void menu_sorting_katalog(Buku data[], int jumlah) {
    int pilihan;
    if (data == NULL || jumlah <= 0) {
        printf("Katalog kosong atau tidak tersedia.\n");
        return;
    }

    for (;;) {
        int status;
        int arah;
        int ascending;

        printf("\n=== MENU SORTING KATALOG ===\n");
        printf("1. Insertion Sort - OCAID (Ascending/Descending)\n");
        printf("2. Insertion Sort - Judul (A-Z/Z-A)\n");
        printf("3. Bubble Sort - Tahun (Terlama/Terbaru)\n");
        printf("4. Selection Sort - Jumlah Halaman (Sedikit/Banyak)\n");
        printf("5. Selection Sort - Stok (Sedikit/Banyak)\n");
        printf("0. Kembali\n");

        status = baca_pilihan("Pilih metode sorting: ", 0, 5, &pilihan);
        if (status == -1) return;
        if (status == 0) {
            printf("Input tidak valid. Masukkan angka 0-5.\n");
            continue;
        }
        if (pilihan == 0) return;

        status = baca_pilihan("Urutan (1 = naik/A-Z, 2 = turun/Z-A): ", 1, 2, &arah);
        if (status == -1) return;
        if (status == 0) {
            printf("Pilihan arah tidak valid.\n");
            continue;
        }
        ascending = (arah == 1);

        switch (pilihan) {
            case 1:
                insertion_sort_ocaid(data, jumlah, ascending);
                break;
            case 2:
                insertion_sort_judul(data, jumlah, ascending);
                break;
            case 3:
                bubble_sort_tahun(data, jumlah, ascending);
                break;
            case 4:
                selection_sort_halaman(data, jumlah, ascending);
                break;
            case 5:
                selection_sort_stok(data, jumlah, ascending);
                break;
        }

        tampilkan_hasil(data, jumlah);
    }
}
