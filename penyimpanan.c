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