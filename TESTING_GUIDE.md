# 🧪 Panduan Testing - Halaman Koleksi Buku

## 📋 Daftar Isi

1. [Persiapan Testing](#persiapan-testing)
2. [Test Scenario](#test-scenario)
3. [Expected Results](#expected-results)
4. [Troubleshooting](#troubleshooting)

---

## 🔧 Persiapan Testing

### 1. Build & Run Application

```bash
# Dari Qt Creator
- Klik tombol Build (Hammer icon)
- Klik tombol Run (Play icon)

# Atau dari terminal
cd "build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug"
# Run executable yang dihasilkan
```

### 2. Verifikasi Database

- Pastikan database terbuka dengan data sample (15 buku)
- Jika tidak ada data, aplikasi akan auto-import dari `data/books.json`
- Atau akan insert 15 sample books otomatis

### 3. Buka Halaman Koleksi Buku

- Klik menu "Koleksi Buku" di sidebar kiri
- Pastikan tabel/grid menampilkan buku

---

## 🧪 Test Scenario

### Test 1: SEARCH BY TITLE (Linear Search)

**Steps:**

1. Fokus ke input field "Cari Judul"
2. Ketik partial title, contoh: "pelangi"
3. Observe hasil real-time filtering

**Expected Result:**

- ✅ Tabel/grid update otomatis saat mengetik
- ✅ Menampilkan buku dengan judul yang mengandung "pelangi" (case-insensitive)
- ✅ Contoh: "Laskar Pelangi" muncul
- ✅ Algoritma: Linear Search O(n)

**Validation:**

- Coba ketik huruf acak → hasilnya filtered
- Clear input → semua buku muncul kembali

---

### Test 2: SEARCH BY AUTHOR (Linear Search)

**Steps:**

1. Fokus ke input field "Cari Penulis"
2. Ketik nama penulis, contoh: "Andrea"
3. Tekan Enter atau klik tombol Cari

**Expected Result:**

- ✅ Dialog/Message jika tidak ditemukan
- ✅ Tabel/grid update dengan buku dari penulis tersebut
- ✅ Contoh: "Andrea Hirata" → muncul "Laskar Pelangi"
- ✅ Algoritma: Linear Search O(n) via BookManager

**Validation:**

- Coba nama yang tidak ada → muncul dialog "Tidak ada buku"
- Coba nama yang ada → buku ter-filter

---

### Test 3: FILTER BY GENRE (Linear Search)

**Steps:**

1. Klik dropdown "Filter Genre"
2. Pilih genre, contoh: "Fiksi"
3. Klik tombol "Cari"

**Expected Result:**

- ✅ Tabel/grid hanya menampilkan buku dengan genre "Fiksi"
- ✅ Multiple genre support (buku bisa punya >1 genre)
- ✅ Algoritma: Linear Search O(n) via BookManager

**Validation:**

- Pilih "-- Semua Genre --" → semua buku muncul
- Pilih genre spesifik → hanya buku dengan genre tersebut

---

### Test 4: RESET FILTER

**Steps:**

1. Lakukan filter/search (title/author/genre)
2. Klik tombol "Reset"

**Expected Result:**

- ✅ Semua input field ter-clear
- ✅ Dropdown genre kembali ke "-- Semua Genre --"
- ✅ Tabel/grid menampilkan semua buku
- ✅ State UI reset sempurna

---

### Test 5: SORT BY TITLE (QuickSort)

**Steps:**

1. Klik tombol "Judul"
2. Observe urutan buku di tabel/grid
3. Klik lagi tombol "Judul"

**Expected Result:**

- ✅ Klik pertama: Sort ascending (A-Z)
- ✅ Klik kedua: Sort descending (Z-A)
- ✅ Klik ketiga: Sort ascending lagi (toggle)
- ✅ Algoritma: QuickSort O(n log n) dari BookManager
- ✅ Bukan std::sort, tapi custom implementation

**Validation:**

- Periksa urutan judul: alphabetical
- Toggle bekerja dengan benar

---

### Test 6: SORT BY YEAR (QuickSort)

**Steps:**

1. Klik tombol "Tahun"
2. Observe urutan tahun

**Expected Result:**

- ✅ Klik pertama: Sort ascending (tahun lama → baru)
- ✅ Klik kedua: Sort descending (tahun baru → lama)
- ✅ Algoritma: QuickSort O(n log n)

**Validation:**

- Periksa kolom "Tahun": numerical order

---

### Test 7: SORT BY RATING (QuickSort)

**Steps:**

1. Klik tombol "Rating"
2. Observe urutan rating

**Expected Result:**

- ✅ Klik pertama: Sort ascending (rating rendah → tinggi)
- ✅ Klik kedua: Sort descending (rating tinggi → rendah)
- ✅ Algoritma: QuickSort O(n log n)

**Validation:**

- Periksa kolom "Rating": numerical order dengan ⭐

---

### Test 8: SORT BY AUTHOR (QuickSort)

**Steps:**

1. Klik tombol "Penulis"
2. Observe urutan penulis

**Expected Result:**

- ✅ Klik pertama: Sort ascending (A-Z)
- ✅ Klik kedua: Sort descending (Z-A)
- ✅ Algoritma: QuickSort O(n log n)

**Validation:**

- Periksa kolom "Penulis": alphabetical order

---

### Test 9: BUILD BST (Binary Search Tree)

**Steps:**

1. Scroll ke section "🌳 Binary Search Tree"
2. Klik tombol "Build BST"

**Expected Result:**

- ✅ Muncul dialog konfirmasi: "BST berhasil dibangun"
- ✅ Dialog menampilkan jumlah buku (contoh: 15 buku)
- ✅ Dialog menjelaskan kompleksitas: O(log n)
- ✅ BST tersimpan di memory (BookManager)

**Validation:**

- Build multiple times → tetap success
- BST diurutkan berdasarkan judul (alphabetical)

---

### Test 10: SEARCH BST (Binary Search Tree)

**SCENARIO A: BST Belum Dibangun**

**Steps:**

1. Restart aplikasi (atau clear BST)
2. Ketik judul di input "Cari judul buku dengan BST..."
3. Klik "🔍 Search BST"

**Expected Result:**

- ✅ Muncul dialog: "BST Belum Dibangun"
- ✅ Tanya: "Apakah Anda ingin membangun BST sekarang?"
- ✅ Jika klik Yes → auto build BST
- ✅ Jika klik No → cancel search

**SCENARIO B: BST Sudah Dibangun - Buku Ditemukan**

**Steps:**

1. Build BST terlebih dahulu
2. Ketik judul EXACT, contoh: "Laskar Pelangi"
3. Klik "🔍 Search BST" atau tekan Enter

**Expected Result:**

- ✅ Dialog: "✅ Buku Ditemukan (BST)"
- ✅ Menampilkan detail buku:
  - 📖 Judul: Laskar Pelangi
  - ✍️ Penulis: Andrea Hirata
  - 📅 Tahun: 2005
  - ⭐ Rating: 4.8
- ✅ Tabel/grid update hanya menampilkan buku tersebut
- ✅ Algoritma: BST Search O(log n)

**SCENARIO C: BST Sudah Dibangun - Buku Tidak Ditemukan**

**Steps:**

1. Ketik judul yang tidak ada, contoh: "Buku Ajaib"
2. Klik "🔍 Search BST"

**Expected Result:**

- ✅ Dialog: "❌ Tidak Ditemukan"
- ✅ Pesan: "Buku dengan judul 'Buku Ajaib' tidak ditemukan di BST"
- ✅ Tips pencarian: exact match, case-insensitive
- ✅ Tabel/grid tidak berubah

**Validation:**

- BST search adalah EXACT MATCH (tidak partial)
- Case-insensitive: "laskar pelangi" = "Laskar Pelangi"
- Kompleksitas O(log n)

---

### Test 11: DELETE BOOK (Persiapan Stack)

**Steps:**

1. Pilih buku di tabel (klik row)
2. Klik tombol "🗑️ Hapus Buku"
3. Konfirmasi delete

**Expected Result:**

- ✅ Buku hilang dari tabel
- ✅ Buku di-push ke stack (internal)
- ✅ Tombol "↩️ Undo Hapus" aktif/enabled
- ✅ Database di-update

**Note:** Ini persiapan untuk test Stack/Undo

---

### Test 12: UNDO DELETE (Stack LIFO)

**SCENARIO A: Ada Buku yang Bisa Di-Undo**

**Steps:**

1. Setelah delete beberapa buku (test 11)
2. Klik tombol "↩️ Undo Hapus"

**Expected Result:**

- ✅ Dialog: "✅ Undo Berhasil"
- ✅ Menampilkan: "Buku berhasil dikembalikan!"
- ✅ Menampilkan: "Sisa undo tersedia: X"
- ✅ Buku terakhir yang dihapus muncul kembali di tabel
- ✅ Algoritma: Stack Pop O(1) - LIFO

**Validation:**

- Delete 3 buku: A, B, C
- Undo 1x → C muncul (Last In First Out)
- Undo 2x → B muncul
- Undo 3x → A muncul
- Undo 4x → Dialog "Stack undo kosong"

**SCENARIO B: Tidak Ada Buku yang Bisa Di-Undo**

**Steps:**

1. Buka aplikasi fresh (belum ada delete)
2. Klik tombol "↩️ Undo Hapus"

**Expected Result:**

- ✅ Dialog: "⚠️ Tidak Ada Undo"
- ✅ Pesan: "Stack undo kosong!"
- ✅ Pesan: "Tidak ada buku yang bisa dikembalikan."

---

### Test 13: TOGGLE VIEW (Card ↔ Table)

**Steps:**

1. Klik tombol "🗃️ Switch View" (atau "Card View"/"Table View")
2. Observe perubahan tampilan

**Expected Result:**

- ✅ Tampilan berubah dari Table → Card Grid (4 kolom)
- ✅ Tampilan berubah dari Card Grid → Table
- ✅ Data tetap sama, hanya layout yang berubah
- ✅ Text tombol berubah sesuai mode:
  - Di Table mode: "🗃️ Card View"
  - Di Card mode: "🗃️ Table View"

**Validation:**

- Filter & sort tetap bekerja di kedua mode
- Card menampilkan cover + info ringkas
- Table menampilkan data detail dalam row

---

## 📊 Expected Performance

| Operasi       | Algoritma  | Kompleksitas | Waktu (15 buku) |
| ------------- | ---------- | ------------ | --------------- |
| Sort          | QuickSort  | O(n log n)   | < 1 ms          |
| Linear Search | Linear     | O(n)         | < 1 ms          |
| BST Build     | BST Insert | O(n log n)   | < 1 ms          |
| BST Search    | BST Search | O(log n)     | < 1 ms          |
| Undo Delete   | Stack Pop  | O(1)         | < 1 ms          |

**Note:** Untuk dataset besar (1000+ buku), BST search jauh lebih cepat dari linear search.

---

## 🔍 Struktur Data Validation

### Checklist Algoritma yang Digunakan

- [x] **QuickSort** (bukan Bubble Sort atau std::sort)

  - Implementation: `BookManager::quickSort()`
  - Digunakan di: Sort by Title, Year, Rating, Author

- [x] **Linear Search** (bukan Binary Search untuk partial match)

  - Implementation: Loop `for (const Book& book : m_books)`
  - Digunakan di: Search by Title, Author, Genre

- [x] **Binary Search Tree**

  - Implementation: `BookManager::BSTNode` dengan `insertBST()` dan `searchBST()`
  - Digunakan di: Build BST, Search BST

- [x] **Stack (LIFO)**
  - Implementation: `std::stack<Book> m_deletedBooks`
  - Digunakan di: Undo Delete

---

## ❌ Troubleshooting

### Problem 1: BST Search Tidak Menemukan Buku (Padahal Ada)

**Possible Causes:**

- BST belum dibangun → Klik "Build BST" dulu
- Typo dalam judul → BST search adalah exact match
- Case sensitivity → Seharusnya case-insensitive, check implementation

**Solution:**

- Build BST terlebih dahulu
- Pastikan judul exact (copy paste dari tabel)

---

### Problem 2: Undo Tidak Ada Efek

**Possible Causes:**

- Tidak ada buku yang di-delete sebelumnya
- Stack sudah kosong (semua undo sudah digunakan)
- Database tidak ter-sync

**Solution:**

- Delete buku terlebih dahulu
- Check implementasi `saveBookManagerToDatabase()`

---

### Problem 3: Sorting Tidak Stabil

**Possible Causes:**

- Multiple klik terlalu cepat → toggle ascending/descending
- Data tidak ter-sort dari BookManager

**Solution:**

- Wait setelah klik sorting
- Check `refreshTable()` dipanggil setelah sort

---

### Problem 4: Filter/Search Tidak Real-time

**Possible Causes:**

- Signal tidak terhubung dengan benar
- `textChanged` signal tidak connect ke `onSearchTextChanged`

**Solution:**

- Check connection di `createControlSection()`:
  ```cpp
  connect(m_searchBox, &QLineEdit::textChanged,
          this, &BooksCollectionPage::onSearchTextChanged);
  ```

---

## ✅ Success Criteria

Semua test dianggap SUCCESS jika:

1. ✅ **Search** (Title, Author, Genre) mengembalikan hasil yang tepat
2. ✅ **Sort** (4 tombol) menggunakan QuickSort dan toggle ascending/descending
3. ✅ **BST Build** sukses membangun tree dari semua buku
4. ✅ **BST Search** menemukan buku dengan O(log n) complexity
5. ✅ **Undo Delete** mengembalikan buku dengan urutan LIFO (Stack)
6. ✅ **Reset** menghapus semua filter dan menampilkan semua buku
7. ✅ **Toggle View** bekerja tanpa hilangkan data
8. ✅ **Tidak ada error** di console atau crash

---

## 📝 Test Report Template

```markdown
# Test Report - Koleksi Buku

**Tanggal**: ******\_\_\_******
**Tester**: ******\_\_\_******

## Test Results

| Test ID | Test Name        | Status        | Notes |
| ------- | ---------------- | ------------- | ----- |
| TEST-01 | Search by Title  | ☐ PASS ☐ FAIL |       |
| TEST-02 | Search by Author | ☐ PASS ☐ FAIL |       |
| TEST-03 | Filter by Genre  | ☐ PASS ☐ FAIL |       |
| TEST-04 | Reset Filter     | ☐ PASS ☐ FAIL |       |
| TEST-05 | Sort by Title    | ☐ PASS ☐ FAIL |       |
| TEST-06 | Sort by Year     | ☐ PASS ☐ FAIL |       |
| TEST-07 | Sort by Rating   | ☐ PASS ☐ FAIL |       |
| TEST-08 | Sort by Author   | ☐ PASS ☐ FAIL |       |
| TEST-09 | Build BST        | ☐ PASS ☐ FAIL |       |
| TEST-10 | Search BST       | ☐ PASS ☐ FAIL |       |
| TEST-11 | Delete Book      | ☐ PASS ☐ FAIL |       |
| TEST-12 | Undo Delete      | ☐ PASS ☐ FAIL |       |
| TEST-13 | Toggle View      | ☐ PASS ☐ FAIL |       |

## Overall Result

☐ ALL TESTS PASSED
☐ SOME TESTS FAILED (See notes above)

## Signatures

Tester: ********\_******** Date: ********\_********
```

---

**Prepared by**: GitHub Copilot  
**Date**: December 15, 2025  
**Version**: 1.0
