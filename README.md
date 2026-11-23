# Sistem Manajemen Perpustakaan Berbasis CLI

Project UAS Praktikum Pemrograman Kelas A — Program Studi Informatika,
FMIPA Universitas Syiah Kuala (2026).

Aplikasi CLI berbahasa C untuk mengelola katalog buku dan sirkulasi peminjaman,
dengan dua peran: **Admin (Pustakawan)** dan **User (Anggota/Mahasiswa)**.
Seluruh data disimpan persisten dalam berkas `.txt`.

## Anggota Kelompok

| No | Nama Lengkap | NPM | 
|----|--------------|-----|
| 1  | _(Afirah El Abidah)_ | _(260810701100002)_ |
| 2  | _(Azfar Syukri)_ | _(260810701100035)_ |
| 3  | _(Cut Riska Dewi)_ | _(260810701100057)_ |

## Fitur

**Admin**
- Create: tambah buku baru ke katalog
- Read: tampilkan seluruh koleksi beserta atribut dan stok
- Update: ubah judul, penulis, tahun, halaman, dan stok buku
- Delete: hapus buku dari katalog
- Laporan transaksi: riwayat peminjaman seluruh user
- Sorting katalog

**User**
- Lihat katalog buku yang tersedia
- Cari buku berdasarkan ID atau judul
- Pinjam buku (stok > 0, stok otomatis berkurang 1)
- Kembalikan buku (stok otomatis bertambah 1, status diperbarui)
- Lihat daftar peminjaman pribadi
- Sorting katalog

**Sorting** (implementasi manual, tanpa `qsort`)
- Insertion Sort — berdasarkan judul buku
- Bubble Sort — berdasarkan penulis
- Selection Sort — berdasarkan tahun terbit

## Arsitektur Aplikasi

Aplikasi dibagi menjadi modul-modul terpisah:

```
main.c  ──►  login (baca user.txt)
   │
   ├── role "admin" ──►  admin.c  (menu_admin: CRUD buku, laporan)
   │
   └── role "user"  ──►  user.c   (menu_user: katalog, cari, pinjam, kembali, riwayat)

admin.c / user.c ──► penyimpanan.c (baca/simpan berkas .txt)
admin.c / user.c ──► sorting.c     (algoritma pengurutan)
```

Alur singkat:
1. Program dimulai dan memuat data akun dari `user.txt`.
2. Pengguna login dengan username dan password.
3. Program menampilkan menu sesuai role.
4. Setiap perubahan data (tambah, edit, hapus, pinjam, kembali) langsung
   disimpan ke berkas `.txt` terkait.
5. Saat keluar, memori dinamis dibebaskan (`free`).

## Struktur File

```
.
├── CMakeLists.txt     # Konfigurasi build CMake
├── README.md          # Dokumentasi
├── main.c             # Entry point, proses login, dan pemilihan menu per role
├── perpustakaan.h     # Definisi umum (struct Book, konstanta)
├── penyimpanan.h/.c   # Struct Buku/User/Peminjaman + baca & simpan berkas .txt
├── admin.h/.c         # Menu dan fitur Admin (CRUD buku, laporan transaksi)
├── user.h/.c          # Menu dan fitur User (katalog, cari, pinjam, kembali, riwayat)
├── sorting.h/.c       # Insertion, Bubble, dan Selection Sort manual
├── Books.txt          # Data katalog buku (dataset)
├── user.txt           # Data akun (username, password, role)
└── peminjaman.txt     # Data transaksi peminjaman
```

## Format Data

**`Books.txt`** — dipisahkan spasi, kolom teks diapit tanda petik ganda:
```
<id> "<judul>" "<penulis>" <tahun> <halaman> <stok>
```

**`user.txt`**
```
<username> <password> <role>      # role: admin | user
```

**`peminjaman.txt`**
```
<id_transaksi> <username> <id_buku> "<judul>" <tanggal_pinjam> <tanggal_kembali> <status>
```
Format tanggal `DD-MM-YYYY`; status `DIPINJAM` / `KEMBALI`.

## Panduan Kompilasi dan Menjalankan

### Prasyarat
- Compiler C (GCC / Clang)
- CMake versi 3.10 atau lebih baru

### Dengan CMake (disarankan)

```bash
git clone <url-repositori>
cd <nama-repositori>

cmake -S . -B build
cmake --build build
```

Jalankan program dari folder `build` (berkas `.txt` otomatis disalin ke sana):

```bash
cd build
./perpustakaan        # Linux / macOS
perpustakaan.exe      # Windows
```

### Tanpa CMake (GCC langsung)

```bash
gcc -std=c11 -Wall -Wextra main.c admin.c user.c penyimpanan.c sorting.c -o perpustakaan
./perpustakaan
```

> **Penting:** program membaca berkas `.txt` dari direktori tempat program
> dijalankan, sehingga `Katalog.txt`, `user.txt`, dan `peminjaman.txt` harus
> berada di direktori yang sama dengan executable.

### Cara Penggunaan

1. Jalankan program, lalu masukkan username dan password.
2. Pilih menu sesuai role (Admin atau User).
3. Pilih menu **Keluar** agar sesi ditutup dengan aman.

## Kontribusi Individu

Kontribusi setiap anggota dapat dilihat secara transparan melalui
*commit history* dan *pull request* pada repositori GitHub ini.
