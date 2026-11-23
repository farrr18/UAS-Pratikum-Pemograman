#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "penyimpanan.h"
#include "user.h"

void menu_user(const char *username){
    int pilihan;

    do{
        printf("\n=== MENU PENGGUNA ===\n");
        printf("Selamat datang, %s !\n", username);
        printf("1. Tampilkan Katalog Buku\n");
        printf("2. Cari Buku\n");
        printf("3. Pinjam Buku\n");
        printf("4. Kembalikan Buku\n");
        printf("5. Riwayat Peminjaman\n");
        printf("0. Logout\n");

        printf("Pilih menu: ");
        scanf("%d", &pilihan);

        switch (pilihan){
            case 1:
                tampilkan_katalog_user();
                break;

            case 2:
                cari_buku();
                break;

            case 3:
                pinjam_buku(username);
                break;

            case 4:
                printf("Menu pegembalian buku\n");
                break;

            case 5:
                printf("Menu riwayat peminjaman\n");
                break;

            case 0:
                printf("Berhasil logout!\n");
                break;

            default:
                printf("Pilihan tidak valid!\n");
        }
    } while (pilihan != 0);
}

void tampilkan_katalog_user(void){
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;

    printf("\n=== DAFTAR KATALOG BUKU ===\n");

    if(!baca_buku("katalog.txt", &daftar_buku, &jumlah_buku)){
        printf("Gagal membaca katalog buku!\n");
        free(daftar_buku);
        return;
    }

    if (jumlah_buku == 0){
        printf("Belum ada buku dalam katalog.\n");
        free(daftar_buku);
        return;
    }

    for(int i = 0; i <jumlah_buku; i++){
        printf("\nBuku ke-%d\n", i + 1);
        printf("ID Buku        : %d\n", daftar_buku[i].id);
        printf("Judul Buku     : %s\n", daftar_buku[i].judul);
        printf("Penulis        : %s\n", daftar_buku[i].penulis);
        printf("Tahun Terbit   : %d\n", daftar_buku[i].tahun);
        printf("Jumlah Halaman : %d\n", daftar_buku[i].halaman);
        printf("Stok Buku      : %d\n", daftar_buku[i].stok);
    }
    free(daftar_buku);
}

void cari_buku(void){
    Buku *daftar_buku = NULL;
    int jumlah_buku = 0;
    int pilihan;
    int id_cari;
    char kata_kunci[256];
    int ditemukan = 0;

    printf("\n=== CARI BUKU ===\n");
    printf("1. Cari bedasarkan ID\n");
    printf("2. Cari berdsarkan Judul\n");
    printf("3. Cari berdasarkan Penulis\n");
    printf("Pilihan motide pencarian: ");
    scanf("%d", &pilihan);
    getchar();

    if (!baca_buku("katalog.txt", &daftar_buku, &jumlah_buku)){
        printf("Gagal membaca katalog buku!\n");
        free(daftar_buku);
        return;
    }

    if (jumlah_buku == 0){
        printf("Katalog buku masih kosong.\n");
        free(daftar_buku);
        return;
    }

    if (pilihan == 1){
        printf("Masukan ID buku: ");
        scanf("%d", &id_cari);
    }
    else if (pilihan == 2){
        printf("Masukkan judul buku: ");
        fgets(kata_kunci, sizeof(kata_kunci), stdin);
        kata_kunci[strcspn(kata_kunci, "\n")] = '\0';
    }
    else if (pilihan == 3){
        printf("Masukan nama penulis: ");
        fgets(kata_kunci, sizeof(kata_kunci), stdin);
        kata_kunci[strcspn(kata_kunci, "\n")] = '\0';
    }
    else {
        printf("Pilihan pencarian tidak valid!\n");
        free(daftar_buku);
        return;
    }

    for (int i = 0; i < jumlah_buku; i++){
        int cocok = 0;

        if(pilihan == 1 && daftar_buku[i].id == id_cari){
            cocok = 1;
        }
        else if(pilihan == 2 && strstr(daftar_buku[i].judul, kata_kunci) != NULL){
            cocok = 1;
        }
        else if (pilihan == 3 && strstr(daftar_buku[i].penulis, kata_kunci) != NULL){
            cocok = 1;
        }

        if (cocok){
            ditemukan = 1;

            printf("\nBuku ditemukan!\n");
            printf("ID Buku         : %d\n", daftar_buku[i].id);
            printf("Judul buku      : %s\n", daftar_buku[i].judul);
            printf("Penulis         : %s\n", daftar_buku[i].penulis);
            printf("Tahun terbit    : %d\n", daftar_buku[i].tahun);
            printf("Jumlah Halaman  : %d\n", daftar_buku[i].halaman);
            printf("Stok Buku       : %d\n", daftar_buku[i].stok);
        }
    }

    if(!ditemukan){
        printf("\nBuku yang dicari tidak ditemukan.\n");
    }

    free(daftar_buku); 
}

void pinjam_buku(const char *username) {
    Buku *daftar_buku = NULL;
    Peminjaman *daftar_pinjam = NULL;

    int jumlah_buku = 0;
    int jumlah_pinjam = 0;
    int id_cari;
    int indeks_buku = -1;
    int id_transaksi = 1;

    printf("\n=== PEMINJAMAN BUKU ===\n");
    if (!baca_buku("katalog.txt", &daftar_buku, &jumlah_buku)) {
        printf("Gagal membaca katalog buku!\n");
        free(daftar_buku);
        return;
    }

    if (jumlah_buku == 0){
        printf("Katalog buku masih kosong.\n");
        free(daftar_buku);
        return;
    }

    printf("Masukkan ID buku yang ingin dipinjam: ");
    if (scanf("%d", &id_cari) != 1){
        printf("Input ID tidak valid!\n");
        int karakter;
        while ((karakter = getchar()) != '\n' && karakter != EOF);
        free(daftar_buku);
        return;
    }

    for (int i = 0; i < jumlah_buku; i++){
        if (daftar_buku[i].id == id_cari){
            indeks_buku = i;
            break;
        }
    }

    if (indeks_buku == -1){
        printf("Buku dengan ID tersebut tidak ditemukan!\n");
        free(daftar_buku);
        return;
    }

    if (daftar_buku[indeks_buku].stok <= 0){
        printf("Maaf, stok buku sedang habis!\n");
        free(daftar_buku);
        return;
    }

    FILE *file = fopen("peminjaman.txt", "a");

    if (file == NULL){
        printf("Gagal membuka file transaksi!\n");
        free(daftar_buku);
        return;
    }

    fclose(file);

    if (!baca_peminjaman("peminjaman.txt",&daftar_pinjam, &jumlah_pinjam)){
        printf("Gagal membaca data transaksi!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    for (int i = 0; i < jumlah_pinjam; i++){
        if (daftar_pinjam[i].id_transaksi >= id_transaksi){
            id_transaksi = daftar_pinjam[i].id_transaksi + 1;
        }
    }

    Peminjaman *data_baru = realloc(daftar_pinjam,(jumlah_pinjam + 1) * sizeof(Peminjaman));

    if (data_baru == NULL) {
        printf("Gagal menambahkan transaksi!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    daftar_pinjam = data_baru;

    Peminjaman transaksi_baru;

    transaksi_baru.id_transaksi = id_transaksi;
    transaksi_baru.id_buku = daftar_buku[indeks_buku].id;

    snprintf(transaksi_baru.username,sizeof(transaksi_baru.username), "%s", username);

    snprintf(transaksi_baru.judul_buku,sizeof(transaksi_baru.judul_buku), "%s",daftar_buku[indeks_buku].judul);

    time_t sekarang = time(NULL);
    struct tm *waktu = localtime(&sekarang);

    if (waktu == NULL){
        printf("Gagal mendapatkan tanggal!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    strftime(transaksi_baru.tanggal_pinjam,sizeof(transaksi_baru.tanggal_pinjam),"%Y-%m-%d", waktu);

    strcpy(transaksi_baru.tanggal_kembali, "-");
    strcpy(transaksi_baru.status, "dipinjam");

    daftar_pinjam[jumlah_pinjam] = transaksi_baru;
    jumlah_pinjam++;

    daftar_buku[indeks_buku].stok--;

    if (!simpan_buku("katalog.txt", daftar_buku, jumlah_buku)){
        printf("Gagal menyimpan perubahan stok!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    if (!simpan_peminjaman("peminjaman.txt",daftar_pinjam, jumlah_pinjam)){

        daftar_buku[indeks_buku].stok++;
        simpan_buku("katalog.txt", daftar_buku, jumlah_buku);

        printf("Gagal menyimpan transaksi peminjaman!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    printf("\nPeminjaman berhasil!\n");
    printf("ID Transaksi   : %d\n", id_transaksi);
    printf("Nama Pengguna  : %s\n", username);
    printf("Judul Buku     : %s\n", daftar_buku[indeks_buku].judul);
    printf("Tanggal Pinjam : %s\n", transaksi_baru.tanggal_pinjam);
    printf("Sisa Stok      : %d\n", daftar_buku[indeks_buku].stok);

    free(daftar_buku);
    free(daftar_pinjam);
}


void kembalikan_buku(const char *username) {
    Buku *daftar_buku = NULL;
    Peminjaman *daftar_pinjam = NULL;

    int jumlah_buku = 0;
    int jumlah_pinjam = 0;
    int id_transaksi;
    int indeks_transaksi = -1;
    int indeks_buku = -1;

    printf("\n=== PENGEMBALIAN BUKU ===\n");

    if (!baca_peminjaman("peminjaman.txt",&daftar_pinjam, &jumlah_pinjam)){
        printf("Gagal membaca data peminjaman!\n");
        free(daftar_pinjam);
        return;
    }

    printf("\n=== DAFTAR BUKU YANG BELUM DIKEMBALIKAN ===\n");

    int ada_pinjaman = 0;

    for (int i = 0; i < jumlah_pinjam; i++) {
        if (strcmp(daftar_pinjam[i].username, username) == 0 &&
            strcmp(daftar_pinjam[i].status, "dipinjam") == 0) {

            ada_pinjaman = 1;

            printf("\nID Transaksi  : %d\n", daftar_pinjam[i].id_transaksi);
            printf("ID Buku         : %d\n", daftar_pinjam[i].id_buku);
            printf("Judul Buku      : %s\n", daftar_pinjam[i].judul_buku);
            printf("Tanggal Pinjam  : %s\n", daftar_pinjam[i].tanggal_pinjam);
            printf("Status          : %s\n", daftar_pinjam[i].status);
        }
    }

    if (!ada_pinjaman) {
        printf("Tidak ada buku yang perlu dikembalikan.\n");
        free(daftar_pinjam);
        return;
    }

    printf("\nMasukkan ID transaksi yang ingin dikembalikan: ");

    if (scanf("%d", &id_transaksi) != 1) {
        printf("Input ID transaksi tidak valid!\n");
        int karakter;
        while ((karakter = getchar()) != '\n' && karakter != EOF);
        free(daftar_pinjam);
        return;
    }

    for (int i = 0; i < jumlah_pinjam; i++) {
        if (daftar_pinjam[i].id_transaksi == id_transaksi && strcmp(daftar_pinjam[i].username, username) == 0) {
            indeks_transaksi = i;
            break;
        }
    }

    if (indeks_transaksi == -1) {
        printf("Transaksi tidak ditemukan atau bukan milik Anda!\n");
        free(daftar_pinjam);
        return;
    }

    if (strcmp(daftar_pinjam[indeks_transaksi].status, "dipinjam") != 0) {
        printf("Buku pada transaksi ini sudah dikembalikan!\n");
        free(daftar_pinjam);
        return;
    }

    if (!baca_buku("katalog.txt", &daftar_buku, &jumlah_buku)) {
        printf("Gagal membaca katalog buku!\n");
        free(daftar_pinjam);
        free(daftar_buku);
        return;
    }

    for (int i = 0; i < jumlah_buku; i++) {
        if (daftar_buku[i].id == daftar_pinjam[indeks_transaksi].id_buku) {
            indeks_buku = i;
            break;
        }
    }

    if (indeks_buku == -1) {
        printf("Data buku tidak ditemukan dalam katalog!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    time_t sekarang = time(NULL);
    struct tm *waktu = localtime(&sekarang);

    if (waktu == NULL) {
        printf("Gagal mendapatkan tanggal!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    char tanggal_kembali[20];

    strftime(tanggal_kembali, sizeof(tanggal_kembali),"%Y-%m-%d", waktu);
    strcpy(daftar_pinjam[indeks_transaksi].tanggal_kembali,tanggal_kembali);
    strcpy(daftar_pinjam[indeks_transaksi].status,"dikembalikan");

    daftar_buku[indeks_buku].stok++;

    if (!simpan_buku("katalog.txt", daftar_buku, jumlah_buku)) {
        printf("Gagal menyimpan perubahan stok buku!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    if (!simpan_peminjaman("peminjaman.txt",daftar_pinjam, jumlah_pinjam)) {

        daftar_buku[indeks_buku].stok--;
        simpan_buku("katalog.txt", daftar_buku, jumlah_buku);

        printf("Gagal menyimpan data pengembalian!\n");
        free(daftar_buku);
        free(daftar_pinjam);
        return;
    }

    printf("\nPengembalian buku berhasil!\n");
    printf("ID Transaksi       : %d\n", id_transaksi);
    printf("Judul Buku         : %s\n",
           daftar_pinjam[indeks_transaksi].judul_buku);
    printf("Tanggal Pengembalian: %s\n", tanggal_kembali);
    printf("Stok Buku Sekarang : %d\n",
           daftar_buku[indeks_buku].stok);

    free(daftar_buku);
    free(daftar_pinjam);
}
