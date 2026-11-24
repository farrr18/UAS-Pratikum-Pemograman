#include "penyimpanan.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 16384

static void trim_spasi(char *teks) {
    size_t awal = 0;
    size_t akhir;
    size_t panjang;

    if (teks == NULL) return;

    panjang = strlen(teks);
    while (awal < panjang && isspace((unsigned char)teks[awal])) awal++;
    akhir = panjang;
    while (akhir > awal && isspace((unsigned char)teks[akhir - 1])) akhir--;

    if (awal > 0) memmove(teks, teks + awal, akhir - awal);
    teks[akhir - awal] = '\0';
}

static void hapus_kurung_siku(char *teks) {
    size_t baca, tulis = 0;
    for (baca = 0; teks[baca] != '\0'; baca++) {
        if (teks[baca] != '[' && teks[baca] != ']') {
            teks[tulis++] = teks[baca];
        }
    }
    teks[tulis] = '\0';
    trim_spasi(teks);
}

static int tulis_teks_berkutip(FILE *fp, const char *teks) {
    size_t i;
    if (fputc('"', fp) == EOF) return 0;

    for (i = 0; teks[i] != '\0'; i++) {
        if (teks[i] == '"' && fputc('"', fp) == EOF) return 0;
        if (fputc((unsigned char)teks[i], fp) == EOF) return 0;
    }

    return fputc('"', fp) != EOF;
}

static int parse_int_aman(const char *teks, int minimum, int maksimum, int *hasil) {
    char *akhir;
    long nilai;

    errno = 0;
    nilai = strtol(teks, &akhir, 10);
    if (teks == akhir || *akhir != '\0' || errno == ERANGE ||
        nilai < minimum || nilai > maksimum) {
        return 0;
    }

    *hasil = (int)nilai;
    return 1;
}

static int salin_teks(char *tujuan, size_t ukuran, const char *sumber) {
    size_t panjang = strlen(sumber);
    if (panjang >= ukuran) return 0;
    memcpy(tujuan, sumber, panjang + 1);
    return 1;
}

int pecah_baris(const char *baris, char kolom[][MAX_FIELD], int max_kolom) {
    int i = 0;
    int jumlah = 0;

    if (baris == NULL || kolom == NULL || max_kolom <= 0) return -1;

    while (baris[i] != '\0' && jumlah < max_kolom) {
        int j = 0;
        int terlalu_panjang = 0;

        while (baris[i] != '\0' && isspace((unsigned char)baris[i])) i++;
        if (baris[i] == '\0') break;

        if (baris[i] == '"') {
            int tertutup = 0;
            i++;

            while (baris[i] != '\0') {
                if (baris[i] == '"') {
                    if (baris[i + 1] == '"') {
                        if (j < MAX_FIELD - 1) kolom[jumlah][j++] = '"';
                        else terlalu_panjang = 1;
                        i += 2;
                    } else {
                        i++;
                        tertutup = 1;
                        break;
                    }
                } else {
                    if (j < MAX_FIELD - 1) kolom[jumlah][j++] = baris[i];
                    else terlalu_panjang = 1;
                    i++;
                }
            }

            if (!tertutup || terlalu_panjang) return -1;
            if (baris[i] != '\0' && !isspace((unsigned char)baris[i])) return -1;
        } else {
            while (baris[i] != '\0' && !isspace((unsigned char)baris[i])) {
                if (j < MAX_FIELD - 1) kolom[jumlah][j++] = baris[i];
                else terlalu_panjang = 1;
                i++;
            }
            if (terlalu_panjang) return -1;
        }

        kolom[jumlah][j] = '\0';
        trim_spasi(kolom[jumlah]);
        jumlah++;
    }

    while (baris[i] != '\0' && isspace((unsigned char)baris[i])) i++;
    if (baris[i] != '\0') return -1;

    return jumlah;
}

int baca_buku(const char *nama_file, Buku **daftar_buku, int *jumlah_buku) {
    FILE *fp;
    Buku *data;
    int kapasitas = 128;
    int jumlah = 0;
    char baris[MAX_LINE];
    char kolom[10][MAX_FIELD];

    if (daftar_buku == NULL || jumlah_buku == NULL) return 0;
    *daftar_buku = NULL;
    *jumlah_buku = 0;

    fp = fopen(nama_file, "r");
    if (fp == NULL) return 0;

    data = (Buku *)malloc((size_t)kapasitas * sizeof(Buku));
    if (data == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        int n;
        int tahun, halaman, stok;
        Buku buku;

        if (baris[0] == '#') continue;
        n = pecah_baris(baris, kolom, 10);
        if (n < 7) continue;
        if (strcmp(kolom[0], "ocaid") == 0 || strcmp(kolom[0], "OCAID") == 0) continue;
        if (strlen(kolom[0]) >= sizeof(buku.ocaid)) continue;
        if (!parse_int_aman(kolom[4], 0, INT_MAX, &tahun)) continue;
        if (!parse_int_aman(kolom[5], 0, INT_MAX, &halaman)) continue;
        if (!parse_int_aman(kolom[6], 0, INT_MAX, &stok)) continue;
        if (!salin_teks(buku.ocaid, sizeof(buku.ocaid), kolom[0]) ||
            !salin_teks(buku.judul, sizeof(buku.judul), kolom[1]) ||
            !salin_teks(buku.penulis, sizeof(buku.penulis), kolom[2]) ||
            !salin_teks(buku.penerbit, sizeof(buku.penerbit), kolom[3])) {
            continue;
        }

        buku.tahun = tahun;
        buku.halaman = halaman;
        buku.stok = stok;

        if (jumlah == kapasitas) {
            Buku *baru;
            if (kapasitas > INT_MAX / 2) break;
            kapasitas *= 2;
            baru = (Buku *)realloc(data, (size_t)kapasitas * sizeof(Buku));
            if (baru == NULL) {
                free(data);
                fclose(fp);
                return 0;
            }
            data = baru;
        }

        data[jumlah++] = buku;
    }

    if (ferror(fp)) {
        free(data);
        fclose(fp);
        return 0;
    }
    fclose(fp);

    *daftar_buku = data;
    *jumlah_buku = jumlah;
    return 1;
}

int buat_katalog_awal(const char *file_asli, const char *file_katalog) {
    FILE *cek = fopen(file_katalog, "r");
    FILE *sumber;
    FILE *tujuan;
    char baris[MAX_LINE];
    char kolom[10][MAX_FIELD];
    int jumlah_buku = 0;

    if (cek != NULL) {
        char penanda[64] = {0};
        if (fgets(penanda, sizeof(penanda), cek) != NULL) {
            penanda[strcspn(penanda, "\r\n")] = '\0';
        }
        if (penanda[0] != '\0' && strcmp(penanda, "#KATALOG_PERPUSTAKAAN") == 0) {
            fclose(cek);
            return 1;
        }
        fclose(cek);
    }

    sumber = fopen(file_asli, "r");
    if (sumber == NULL) {
        printf("Gagal membuka dataset asli: %s\n", file_asli);
        return 0;
    }

    tujuan = fopen(file_katalog, "w");
    if (tujuan == NULL) {
        printf("Gagal membuat katalog: %s\n", file_katalog);
        fclose(sumber);
        return 0;
    }

    if (fprintf(tujuan, "#KATALOG_PERPUSTAKAAN\n") < 0) {
        fclose(sumber);
        fclose(tujuan);
        remove(file_katalog);
        return 0;
    }

    while (fgets(baris, sizeof(baris), sumber) != NULL) {
        int n = pecah_baris(baris, kolom, 10);
        int tahun, halaman;
        const char *penerbit_awal = "Tidak diketahui";
        char judul[MAX_FIELD];
        char penulis[1024];

        if (n < 5) continue;
        if (strcmp(kolom[0], "ocaid") == 0 || strcmp(kolom[0], "OCAID") == 0) continue;
        if (strlen(kolom[0]) >= 64) continue;
        if (!parse_int_aman(kolom[3], 0, INT_MAX, &tahun)) continue;
        if (!parse_int_aman(kolom[4], 0, INT_MAX, &halaman)) continue;
        if (!salin_teks(judul, sizeof(judul), kolom[1])) continue;
        if (!salin_teks(penulis, sizeof(penulis), kolom[2])) continue;

        hapus_kurung_siku(judul);
        hapus_kurung_siku(penulis);

        if (fprintf(tujuan, "%s ", kolom[0]) < 0 ||
            !tulis_teks_berkutip(tujuan, judul) || fputc(' ', tujuan) == EOF ||
            !tulis_teks_berkutip(tujuan, penulis) || fputc(' ', tujuan) == EOF ||
            !tulis_teks_berkutip(tujuan, penerbit_awal) ||
            fprintf(tujuan, " %d %d 5\n", tahun, halaman) < 0) {
            fclose(sumber);
            fclose(tujuan);
            remove(file_katalog);
            return 0;
        }

        jumlah_buku++;
    }

    if (ferror(sumber) || ferror(tujuan)) {
        fclose(sumber);
        fclose(tujuan);
        remove(file_katalog);
        return 0;
    }

    fclose(sumber);
    if (fclose(tujuan) != 0) {
        remove(file_katalog);
        return 0;
    }

    if (jumlah_buku == 0) {
        remove(file_katalog);
        printf("Tidak ada baris buku valid dalam dataset!\n");
        return 0;
    }

    printf("Katalog berhasil dibuat. Jumlah buku: %d\n", jumlah_buku);
    return 1;
}

int baca_user(const char *nama_file, User **daftar_user, int *jumlah_user) {
    FILE *fp;
    User *data;
    int kapasitas = 32;
    int jumlah = 0;
    char baris[MAX_LINE];
    char kolom[5][MAX_FIELD];

    if (daftar_user == NULL || jumlah_user == NULL) return 0;
    *daftar_user = NULL;
    *jumlah_user = 0;

    fp = fopen(nama_file, "r");
    if (fp == NULL) return 0;

    data = (User *)malloc((size_t)kapasitas * sizeof(User));
    if (data == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        int n = pecah_baris(baris, kolom, 5);
        User akun;

        if (n < 3) continue;
        if (!salin_teks(akun.username, sizeof(akun.username), kolom[0]) ||
            !salin_teks(akun.password, sizeof(akun.password), kolom[1]) ||
            !salin_teks(akun.role, sizeof(akun.role), kolom[2])) continue;
        if (strcmp(akun.role, "admin") != 0 && strcmp(akun.role, "user") != 0) continue;

        if (jumlah == kapasitas) {
            User *baru;
            kapasitas *= 2;
            baru = (User *)realloc(data, (size_t)kapasitas * sizeof(User));
            if (baru == NULL) {
                free(data);
                fclose(fp);
                return 0;
            }
            data = baru;
        }
        data[jumlah++] = akun;
    }

    if (ferror(fp)) {
        free(data);
        fclose(fp);
        return 0;
    }
    fclose(fp);
    *daftar_user = data;
    *jumlah_user = jumlah;
    return 1;
}

int baca_peminjaman(const char *nama_file, Peminjaman **daftar_peminjaman, int *jumlah_peminjaman) {
    FILE *fp;
    Peminjaman *data;
    int kapasitas = 64;
    int jumlah = 0;
    char baris[MAX_LINE];
    char kolom[10][MAX_FIELD];

    if (daftar_peminjaman == NULL || jumlah_peminjaman == NULL) return 0;
    *daftar_peminjaman = NULL;
    *jumlah_peminjaman = 0;

    fp = fopen(nama_file, "r");
    if (fp == NULL) return 0;

    data = (Peminjaman *)malloc((size_t)kapasitas * sizeof(Peminjaman));
    if (data == NULL) {
        fclose(fp);
        return 0;
    }

    while (fgets(baris, sizeof(baris), fp) != NULL) {
        int n = pecah_baris(baris, kolom, 10);
        char *akhir;
        unsigned long id;
        Peminjaman transaksi;

        if (n < 7) continue;
        errno = 0;
        id = strtoul(kolom[0], &akhir, 10);
        if (kolom[0] == akhir || *akhir != '\0' || errno == ERANGE || id > UINT_MAX) continue;
        if (!salin_teks(transaksi.username, sizeof(transaksi.username), kolom[1]) ||
            !salin_teks(transaksi.ocaid, sizeof(transaksi.ocaid), kolom[2]) ||
            !salin_teks(transaksi.judul_buku, sizeof(transaksi.judul_buku), kolom[3]) ||
            !salin_teks(transaksi.tanggal_pinjam, sizeof(transaksi.tanggal_pinjam), kolom[4]) ||
            !salin_teks(transaksi.tanggal_kembali, sizeof(transaksi.tanggal_kembali), kolom[5]) ||
            !salin_teks(transaksi.status, sizeof(transaksi.status), kolom[6])) continue;

        if (strcmp(transaksi.status, "dipinjam") == 0) strcpy(transaksi.status, "DIPINJAM");
        if (strcmp(transaksi.status, "dikembalikan") == 0 || strcmp(transaksi.status, "KEMBALI") == 0)
            strcpy(transaksi.status, "KEMBALI");

        transaksi.id_transaksi = (unsigned int)id;

        if (jumlah == kapasitas) {
            Peminjaman *baru;
            kapasitas *= 2;
            baru = (Peminjaman *)realloc(data, (size_t)kapasitas * sizeof(Peminjaman));
            if (baru == NULL) {
                free(data);
                fclose(fp);
                return 0;
            }
            data = baru;
        }
        data[jumlah++] = transaksi;
    }

    if (ferror(fp)) {
        free(data);
        fclose(fp);
        return 0;
    }
    fclose(fp);
    *daftar_peminjaman = data;
    *jumlah_peminjaman = jumlah;
    return 1;
}

int simpan_buku(const char *nama_file, const Buku *daftar_buku, int jumlah_buku) {
    FILE *fp;
    int i;

    if (jumlah_buku < 0 || (jumlah_buku > 0 && daftar_buku == NULL)) return 0;
    fp = fopen(nama_file, "w");
    if (fp == NULL) return 0;

    if (fprintf(fp, "#KATALOG_PERPUSTAKAAN\n") < 0) {
        fclose(fp);
        return 0;
    }

    for (i = 0; i < jumlah_buku; i++) {
        if (fprintf(fp, "%s ", daftar_buku[i].ocaid) < 0 ||
            !tulis_teks_berkutip(fp, daftar_buku[i].judul) || fputc(' ', fp) == EOF ||
            !tulis_teks_berkutip(fp, daftar_buku[i].penulis) || fputc(' ', fp) == EOF ||
            !tulis_teks_berkutip(fp, daftar_buku[i].penerbit) ||
            fprintf(fp, " %d %d %d\n", daftar_buku[i].tahun,
                    daftar_buku[i].halaman, daftar_buku[i].stok) < 0) {
            fclose(fp);
            return 0;
        }
    }

    if (ferror(fp)) {
        fclose(fp);
        return 0;
    }
    return fclose(fp) == 0;
}

int simpan_peminjaman(const char *nama_file, const Peminjaman *daftar_pinjaman, int jumlah_pinjaman) {
    FILE *fp;
    int i;

    if (jumlah_pinjaman < 0 || (jumlah_pinjaman > 0 && daftar_pinjaman == NULL)) return 0;
    fp = fopen(nama_file, "w");
    if (fp == NULL) return 0;

    for (i = 0; i < jumlah_pinjaman; i++) {
        if (fprintf(fp, "%u %s %s ", daftar_pinjaman[i].id_transaksi,
                    daftar_pinjaman[i].username, daftar_pinjaman[i].ocaid) < 0 ||
            !tulis_teks_berkutip(fp, daftar_pinjaman[i].judul_buku) ||
            fprintf(fp, " %s %s %s\n", daftar_pinjaman[i].tanggal_pinjam,
                    daftar_pinjaman[i].tanggal_kembali, daftar_pinjaman[i].status) < 0) {
            fclose(fp);
            return 0;
        }
    }

    if (ferror(fp)) {
        fclose(fp);
        return 0;
    }
    return fclose(fp) == 0;
}

int muat_data(Buku **daftar_buku, int *jumlah_buku,
              User **daftar_user, int *jumlah_user,
              Peminjaman **daftar_pinjam, int *jumlah_pinjam) {
    int ok_buku, ok_user, ok_pinjam;

    if (daftar_buku == NULL || jumlah_buku == NULL || daftar_user == NULL ||
        jumlah_user == NULL || daftar_pinjam == NULL || jumlah_pinjam == NULL) return 0;

    *daftar_buku = NULL; *jumlah_buku = 0;
    *daftar_user = NULL; *jumlah_user = 0;
    *daftar_pinjam = NULL; *jumlah_pinjam = 0;

    ok_buku = baca_buku(PATH_KATALOG_BUKU, daftar_buku, jumlah_buku);
    ok_user = baca_user(PATH_AKUN_USER, daftar_user, jumlah_user);
    ok_pinjam = baca_peminjaman(PATH_TRANSAKSI, daftar_pinjam, jumlah_pinjam);

    if (!ok_buku || !ok_user || !ok_pinjam) {
        bebaskan_memori(daftar_buku, jumlah_buku, daftar_user, jumlah_user,
                        daftar_pinjam, jumlah_pinjam);
        return 0;
    }
    return 1;
}

void bebaskan_memori(Buku **daftar_buku, int *jumlah_buku,
                     User **daftar_user, int *jumlah_user,
                     Peminjaman **daftar_pinjam, int *jumlah_pinjam) {
    if (daftar_buku != NULL) {
        free(*daftar_buku);
        *daftar_buku = NULL;
    }
    if (jumlah_buku != NULL) *jumlah_buku = 0;

    if (daftar_user != NULL) {
        free(*daftar_user);
        *daftar_user = NULL;
    }
    if (jumlah_user != NULL) *jumlah_user = 0;

    if (daftar_pinjam != NULL) {
        free(*daftar_pinjam);
        *daftar_pinjam = NULL;
    }
    if (jumlah_pinjam != NULL) *jumlah_pinjam = 0;
}