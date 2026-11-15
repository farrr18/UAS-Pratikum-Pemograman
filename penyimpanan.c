#include "penyimpanan.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024

static void trim_spasi(char *str){
    int awal = 0, akhir = (int)strlen(str)-1;

    while (str[awal] != '\0' && isspace((unsigned char)str[awal])) {awal++;}
    while (akhir >= awal && isspace((unsigned char)str[akhir])) {akhir--;}
    
    int j = 0;
    for (int i = awal; i <= akhir; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

int pecah_baris(const char *baris, char kolom[][256], int max_kolom){
    int i = 0, jumlah_kolom = 0;

    while (baris[i] != '\0' && baris[i] != '\n' && baris[i] != '\r' && jumlah_kolom < max_kolom){
        while (baris[i] == ' ' || baris[i] == '\t') {i++;}
        if (baris[i] == '\0' || baris[i] == '\n' || baris[i] == '\r') {break;}

        int j = 0;
        if (baris[i] == '"'){
            i++;
            while (baris[i] != '\0' && baris[i] != '"' && baris[i] != '\n' && baris[i] != '\r'){
                if (j < 255) {kolom[jumlah_kolom][j++] = baris[i];}
                i++;
            }
            if (baris[i] == '"') {i++;}
        } else {
            while (baris[i] != '\0' && baris[i] != ' ' && baris[i] != '\t' && baris[i] != '\n' && baris[i] != '\r'){
                if (j < 255) {kolom[jumlah_kolom][j++] = baris[i];}
                i++;
            }
        }

        kolom[jumlah_kolom][j] = '\0';
        trim_spasi(kolom[jumlah_kolom]);
        jumlah_kolom++;
    }

   return jumlah_kolom;
}

int baca_buku(const char *nama_file, Buku **daftar_buku, int *jumlah_buku){
    FILE *fp = fopen(nama_file, "r");
    if (fp == NULL) {
        *daftar_buku = NULL;
        *jumlah_buku = 0;
        return 0;
    }
    int kapasitas = 128, count = 0;
    Buku *temp_list = (Buku *)malloc(kapasitas * sizeof(Buku));
    if (temp_list == NULL) {
        fclose(fp);
        return 0;
    }
    char baris[MAX_LINE];
    char kolom[10][256];

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        int n_kolom = pecah_baris(baris, kolom, 10);
        if (n_kolom < 5) continue;

        if (strcmp(kolom[0], "ID") == 0 || strcmp(kolom[0], "id") == 0) {continue;}
        if (count >= kapasitas) {
            kapasitas *= 2;
            Buku *baru = (Buku *)realloc(temp_list, kapasitas * sizeof(Buku));
            if (baru == NULL) {
                free(temp_list);
                fclose(fp);
                return 0;
            }
            temp_list = baru;
        }

        temp_list[count].id = atoi(kolom[0]);
        strncpy(temp_list[count].judul, kolom[1], sizeof(temp_list[count].judul) - 1);
        temp_list[count].judul[sizeof(temp_list[count].judul) - 1] = '\0';

        strncpy(temp_list[count].penulis, kolom[2], sizeof(temp_list[count].penulis) - 1);
        temp_list[count].penulis[sizeof(temp_list[count].penulis) - 1] = '\0';

        temp_list[count].tahun = atoi(kolom[3]);
        temp_list[count].halaman = atoi(kolom[4]);
        temp_list[count].stok = (n_kolom >= 6) ? atoi(kolom[5]) : 5;
        count++;
    }
    fclose(fp);
    *daftar_buku = temp_list;
    *jumlah_buku = count;
    return 1;

}