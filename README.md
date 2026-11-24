# Sistem Manajemen Perpustakaan Berbasis CLI

Project UAS Praktikum Pemrograman Kelas A — Program Studi Informatika, FMIPA Universitas Syiah Kuala (2026).

Aplikasi Command Line Interface (CLI) menggunakan bahasa C untuk mengelola katalog buku dan sirkulasi peminjaman. Aplikasi memiliki dua peran, yaitu Admin (Pustakawan) dan User (Anggota/Mahasiswa). Data disimpan secara persisten dalam berkas teks `.txt`.

## Anggota Kelompok

| No | Nama Lengkap | NPM |
|---|---|---|
| 1 | Afirah El Abidah | 260810701100002 |
| 2 | Muhammad Azfar Syukri | 260810701100035 |
| 3 | Cut Riska Dewi | 260810701100057 |

## Fitur Aplikasi

### Admin
- Menambahkan buku ke katalog (OCAID, judul, penulis, penerbit, tahun, halaman, dan stok).
- Menampilkan seluruh koleksi beserta atribut dan stok.
- Memperbarui judul, penulis, penerbit, tahun terbit, halaman, dan stok.
- Menghapus buku; buku yang masih memiliki peminjaman aktif tidak dapat dihapus.
- Melihat laporan transaksi seluruh pengguna.
- Mengurutkan katalog menggunakan menu sorting yang sama dengan User.

### User
- Melihat katalog buku.
- Mencari buku berdasarkan OCAID atau judul (pencarian penulis juga tersedia sebagai fitur tambahan).
- Meminjam buku jika stok lebih dari nol; stok berkurang satu dan transaksi dicatat.
- Mengembalikan buku; stok bertambah satu dan status transaksi berubah.
- Melihat daftar peminjaman yang masih aktif.
- Mengurutkan katalog.

## Algoritma Sorting

Ketiga algoritma diimplementasikan secara manual tanpa `qsort()`.

| Algoritma | Atribut | Urutan |
|---|---|---|
| Insertion Sort | OCAID | Ascending / Descending |
| Insertion Sort | Judul | A–Z / Z–A |
| Bubble Sort | Tahun terbit | Terlama–Terbaru / Terbaru–Terlama |
| Selection Sort | Jumlah halaman | Sedikit–Banyak / Banyak–Sedikit |
| Selection Sort | Stok | Sedikit–Banyak / Banyak–Sedikit |

Menu sorting yang sama digunakan oleh Admin dan User. Pengurutan dilakukan pada array di memori dan tidak mengubah urutan permanen katalog.

## Struktur Direktori

```text
UAS-Pratikum-Pemograman/
├── src/
│   ├── main.c
│   ├── admin.c
│   ├── penyimpanan.c
│   ├── user.c
│   └── sorting.c
├── include/
│   ├── admin.h
│   ├── penyimpanan.h
│   ├── user.h
│   └── sorting.h
├── data/
│   ├── Books.txt          # Dataset master asli, tidak diubah aplikasi
│   ├── katalog.txt        # Dibuat otomatis saat pertama kali dijalankan
│   ├── user.txt           # Data akun
│   └── peminjaman.txt     # Data transaksi
├── build/                 # Hasil build lokal, tidak diunggah
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Pengelolaan Data

### Dataset master: `data/Books.txt`

Dataset Open Library merupakan sumber data awal. Format sumber yang digunakan adalah:

```text
<ocaid> "<title>" "<author>" <year> <page_count> <edition_id> <work_id> <url>
```

Aplikasi tidak mengubah file master tersebut.

### Katalog operasional: `data/katalog.txt`

Saat program pertama kali dijalankan, fungsi `buat_katalog_awal()` membaca `Books.txt` dan membuat `katalog.txt` jika file katalog belum ada atau masih kosong. Konversi mempertahankan OCAID asli, mengambil judul, penulis, tahun, dan jumlah halaman, mengisi penerbit awal dengan `Tidak diketahui`, serta memberikan stok awal 5.

Baris pertama katalog berisi penanda `#KATALOG_PERPUSTAKAAN` agar program dapat mengenali file operasional. Baris data berikutnya menggunakan format:

```text
<ocaid> "<judul>" "<penulis>" "<penerbit>" <tahun> <halaman> <stok>
```

Semua fitur administrasi, katalog, pencarian, sorting, peminjaman, dan pengembalian menggunakan `katalog.txt`. File ini tidak dibuat ulang setiap kali program berjalan agar perubahan stok dan data admin tetap tersimpan. Jika ingin mengimpor ulang dataset master, cadangkan katalog dan transaksi terlebih dahulu.

**Migrasi dari format katalog lama:** versi sebelumnya memakai ID numerik dan belum memiliki kolom penerbit. Sebelum menjalankan versi kode ini untuk pertama kali, cadangkan lalu pindahkan/hapus `data/katalog.txt` lama agar dibuat ulang dengan OCAID asli. Jika `data/peminjaman.txt` lokal sudah berisi transaksi dari versi lama yang menyimpan ID numerik, jangan menghapusnya sembarangan: cadangkan dan migrasikan referensi ID buku lama ke OCAID sesuai urutan dataset terlebih dahulu. File `data/peminjaman.txt` di repository contoh saat ini kosong.

### Data akun: `data/user.txt`

Format:

```text
<username> <password> <role>
```

Role yang digunakan adalah `admin` atau `user`. Contoh akun awal (sesuaikan dengan isi `data/user.txt` di repository):

```text
admin admin123 admin
budi budi12 user
siti siti12 user
```

### Data transaksi: `data/peminjaman.txt`

Format:

```text
<id_transaksi> <username> <ocaid> "<judul_buku>" <tanggal_pinjam> <tanggal_kembali> <status>
```

- Tanggal mengikuti format `DD-MM-YYYY`.
- Status peminjaman: `DIPINJAM`.
- Status setelah pengembalian: `KEMBALI`.
- Jika belum dikembalikan, `tanggal_kembali` berisi `-`.

## Persyaratan

- GCC atau Clang yang mendukung C11.
- CMake versi 3.16 atau lebih baru.

## Kompilasi dan Menjalankan

**Penting:** jalankan program dari direktori utama repository karena semua path data menggunakan awalan `data/`.

### Windows (MinGW)

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\perpustakaan.exe
```

### Linux / macOS

```bash
cmake -S . -B build
cmake --build build
./build/perpustakaan
```

### Kompilasi langsung menggunakan GCC

Dari direktori utama repository:

```bash
gcc -std=c11 -Wall -Wextra -Iinclude \
  src/main.c src/admin.c src/user.c src/penyimpanan.c src/sorting.c \
  -o perpustakaan
```

Jalankan pada Windows dengan `./perpustakaan.exe` atau Linux/macOS dengan `./perpustakaan`.

## Catatan Penting

1. Jangan mengubah `Books.txt` melalui aplikasi.
2. Jangan menghapus `katalog.txt` sembarangan karena file tersebut menyimpan perubahan operasional dan stok.
3. Jika katalog perlu dibuat ulang dari master dataset, buat cadangan `katalog.txt` dan `peminjaman.txt` terlebih dahulu.
4. Pastikan `Books.txt`, `user.txt`, dan `peminjaman.txt` berada di dalam folder `data/`.
5. Password pada `user.txt` merupakan data demonstrasi proyek, bukan penyimpanan password aman untuk aplikasi produksi.

## Kontribusi Individu

Kontribusi masing-masing anggota dapat dilihat melalui riwayat commit dan pull request pada repository GitHub.

---

UAS Praktikum Pemrograman Kelas A — Informatika, FMIPA, Universitas Syiah Kuala (2026).
