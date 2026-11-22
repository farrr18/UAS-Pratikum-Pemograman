#include <stdio.h>
#include <string.h>
#include "sorting.h"

void insertion_sort_judul(buku daftar_buku[], int jumlah_buku){
    for(int i = 1; i < jumlah_buku; i++){
        buku kunci = daftar_buku[i];
        int j = i-1;

        while(j >= 0 && strcmp(daftar_buku[j].judul, kunci.judul) > 0){
            daftar_buku[j+1] = daftar_buku[j];
            j--;
        }

        daftar_buku[j+1] = kunci;
    }
}