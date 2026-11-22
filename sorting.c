#include <stdio.h>
#include <string.h>
#include "sorting.h"

void insertion_sort_judul(Buku daftar_buku[], int jumlah_buku){
    for(int i = 1; i < jumlah_buku; i++){
        Buku kunci = daftar_buku[i];
        int j = i-1;

        while(j >= 0 && strcmp(daftar_buku[j].judul, kunci.judul) > 0){
            daftar_buku[j+1] = daftar_buku[j];
            j--;
        }

        daftar_buku[j+1] = kunci;
    }
}

void buble_sort_penulisan(Buku daftar_buku[], int jumlah_buku){
    for(int i =0; i < jumlah_buku -1; i++){
        for(int j = 0; j < jumlah_buku -i -1; j++){

            if (strcmp(daftar_buku[j].penulis, daftar_buku[j+1].penulis) > 0){
                Buku sementara = daftar_buku[j];
                daftar_buku[j] = daftar_buku[j+1];
                daftar_buku[j+1] = sementara;

            }
        }
    }
}

