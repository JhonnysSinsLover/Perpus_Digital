# 📚 Implementasi Struktur Data - Perpustakaan Digital

## 📋 Daftar Isi

1. [Fitur Wajib](#fitur-wajib)
2. [Fitur Tambahan](#fitur-tambahan)
3. [Lokasi Implementasi](#lokasi-implementasi)
4. [Cara Menggunakan](#cara-menggunakan)

---

## ✅ FITUR WAJIB

### 1. 🔄 SORTING (Pengurutan Data)

**Algoritma**: QuickSort dan Merge Sort (BUKAN Bubble Sort)

#### Implementasi Backend

- **File**: `backend/Sorting.h`
- **Algoritma yang diimplementasikan**:
  - ✅ QuickSort (kompleksitas O(n log n) average)
  - ✅ Merge Sort (kompleksitas O(n log n) worst case)
  - ✅ Insertion Sort (untuk array kecil)

#### Penggunaan di BookManager

- **File**: `backend/BookManager.h` & `backend/BookManager.cpp`
- **Fungsi**:
  ```cpp
  void quickSortByTitle(bool ascending = true);
  void quickSortByYear(bool ascending = true);
  void quickSortByRating(bool ascending = true);
  void quickSortByAuthor(bool ascending = true);
  ```

#### Implementasi di UI

- **File**: `ui/BooksCollectionPage.cpp`
- **Lokasi**: Tombol sorting di halaman koleksi buku
- **Cara Menggunakan**:
  1. Buka halaman "Books Collection"
  2. Klik tombol: **Judul**, **Tahun**, **Rating**, atau **Penulis**
  3. Setiap klik akan toggle antara ascending/descending
  4. Menggunakan **QuickSort** dari BookManager (bukan std::sort)

---

### 2. 🔍 SEARCHING (Pencarian Data)

**Algoritma**: Binary Search dan Linear Search

#### Implementasi Backend

- **File**: `backend/Searching.h`
- **Algoritma yang diimplementasikan**:
  - ✅ Binary Search (kompleksitas O(log n) - untuk data terurut)
  - ✅ Binary Search by Key (dengan custom key extractor)
  - ✅ Linear Search (kompleksitas O(n))
  - ✅ Linear Search dengan Predicate

#### Penggunaan di BookManager

- **File**: `backend/BookManager.h` & `backend/BookManager.cpp`
- **Fungsi**:
  ```cpp
  Book* binarySearchByTitle(const QString& title);  // Binary Search
  std::vector<Book> searchByGenre(const QString& genre);  // Linear Search
  std::vector<Book> searchByAuthor(const QString& author);  // Linear Search
  std::vector<Book> searchByYearRange(int minYear, int maxYear);
  ```

#### Implementasi di UI

- **File**: `ui/BooksCollectionPage.cpp`
- **Fitur**:
  1. **Search by Title**: Kotak pencarian di bagian atas
  2. **Search by Genre**: Dropdown filter genre
  3. **Search by Author**: Input pencarian penulis
  4. **BST Search**: Pencarian menggunakan Binary Search Tree (lihat Tree)

---

### 3. 🕸️ GRAPH (Hubungan Antar Data)

**Struktur**: Adjacency List untuk hubungan antar genre buku

#### Implementasi Backend

- **File**: `backend/Graph.h`
- **Struktur Data**: `std::map<QString, std::set<QString>>`
- **Algoritma**:
  - ✅ BFS (Breadth-First Search) untuk traversal
  - ✅ Shortest Path Finding (mencari jalur terpendek antar genre)
  - ✅ Connected Components (genre yang saling terhubung)

#### Fungsi Utama

```cpp
void addEdge(const QString& genre1, const QString& genre2);
void buildGraph(const std::vector<Book>& books);
std::vector<Book> getRecommendation(const QString& genre,
                                    const std::vector<Book>& books,
                                    int maxDepth = 2);
std::vector<QString> findPath(const QString& startGenre,
                              const QString& endGenre);
```

#### Implementasi di UI

- **File**: `ui/RecommendationPage.cpp`
- **Cara Menggunakan**:
  1. Buka halaman **"Recommendations"**
  2. Klik tombol **"🔄 Re-Build Graph Database"** untuk membangun graph
  3. Pilih genre dari dropdown
  4. Klik **"Get Recommendations"**
  5. Sistem akan menggunakan BFS untuk mencari buku terkait melalui hubungan genre

#### Cara Kerja Graph

1. **Build Graph**: Membuat koneksi antar genre berdasarkan buku yang memiliki multiple genre
2. **BFS Traversal**: Mencari genre yang terhubung dengan depth tertentu
3. **Recommendation**: Memberikan skor relevansi berdasarkan jarak di graph
4. **Path Finding**: Mencari jalur terpendek antar dua genre

---

## 🌟 FITUR TAMBAHAN (Nilai Plus)

### 4. 🌳 TREE (Binary Search Tree)

**Struktur**: BST untuk catalog structure

#### Implementasi Backend

- **File**: `backend/BookManager.h` & `backend/BookManager.cpp`
- **Node Structure**:
  ```cpp
  struct BSTNode {
      Book book;
      std::shared_ptr<BSTNode> left;
      std::shared_ptr<BSTNode> right;
  };
  ```

#### Fungsi BST

```cpp
void buildBST();  // Membangun BST dari semua buku
Book* searchBST(const QString& title);  // O(log n) search
std::vector<Book> getBSTInOrder();  // Traversal in-order (terurut)
bool hasBST() const;
void clearBST();
```

#### Implementasi di UI

- **File**: `ui/BooksCollectionPage.cpp`
- **Lokasi**: Section "🌳 Binary Search Tree" di halaman Books Collection
- **Cara Menggunakan**:
  1. Buka halaman **"Books Collection"**
  2. Klik tombol **"Build BST"** untuk membangun BST
  3. Masukkan judul buku di kotak "Cari judul buku dengan BST..."
  4. Klik **"🔍 Search BST"** atau tekan Enter
  5. Sistem akan mencari buku dengan kompleksitas O(log n)

#### Keunggulan BST

- **Pencarian cepat**: O(log n) vs O(n) linear search
- **Traversal terurut**: In-order traversal menghasilkan data terurut
- **Efisien**: Lebih cepat untuk dataset besar

---

### 5. 📚 STACK (Undo Deletion)

**Prinsip**: LIFO (Last In First Out)

#### Implementasi Backend

- **File**: `backend/BookManager.h` & `backend/BookManager.cpp`
- **Data Structure**: `std::stack<Book> m_deletedBooks`

#### Fungsi Stack

```cpp
bool undoDelete();  // Restore buku yang terakhir dihapus
bool canUndo() const;  // Cek apakah ada yang bisa di-undo
size_t getUndoStackSize() const;
void clearUndoStack();
```

#### Implementasi di UI

- **File**: `ui/BooksCollectionPage.cpp`
- **Lokasi**: Tombol **"↩️ Undo Hapus"** di halaman Books Collection
- **Cara Menggunakan**:
  1. Hapus sebuah buku (buku akan di-push ke stack)
  2. Klik tombol **"↩️ Undo Hapus"**
  3. Buku terakhir yang dihapus akan di-restore (pop dari stack)

#### Use Case

- Mencegah kehilangan data tidak sengaja
- Multiple undo operations (LIFO)
- History management

---

### 6. 📋 QUEUE (Borrowing Queue)

**Prinsip**: FIFO (First In First Out)

#### Implementasi Backend

- **File**: `backend/BookManager.h` & `backend/BookManager.cpp`
- **Data Structure**: `std::queue<BorrowRequest>`
- **Struct**:
  ```cpp
  struct BorrowRequest {
      QString borrowerName;
      int bookId;
      QString bookTitle;
      QString requestTime;
  };
  ```

#### Fungsi Queue

```cpp
bool addToBorrowQueue(const QString& borrowerName, int bookId);
BorrowRequest processNextBorrow();  // FIFO - proses antrian pertama
std::vector<BorrowRequest> getAllBorrowRequests();
bool isBorrowQueueEmpty() const;
size_t getBorrowQueueSize() const;
void clearBorrowQueue();
```

#### Implementasi di UI

- **File**: `ui/BorrowQueuePage.cpp`
- **Lokasi**: Halaman **"Borrow Queue"**
- **Cara Menggunakan**:
  1. Buka halaman **"Borrow Queue"**
  2. Isi nama peminjam dan ID buku
  3. Klik **"Add to Queue"** (enqueue - masuk ke belakang antrian)
  4. Klik **"Process Next"** untuk proses antrian pertama (dequeue)
  5. List antrian ditampilkan dengan urutan FIFO

#### Use Case

- Sistem antrian peminjaman buku
- Fair service (first come first served)
- Order management

---

### 7. 🏆 PRIORITY QUEUE / HEAP (Popular Books)

**Prinsip**: Max Heap - rating tertinggi memiliki prioritas tertinggi

#### Implementasi Backend

- **File**: `backend/BookManager.h` & `backend/BookManager.cpp`
- **Data Structure**: `std::priority_queue<Book>`

#### Fungsi Priority Queue

```cpp
std::vector<Book> getTopRatedBooks(int topN = 5);  // Max heap
std::vector<Book> getPopularBooks(double minRating = 4.5);
```

#### Implementasi di UI

- **File**: `ui/PopularBooksPage.cpp`
- **Lokasi**: Halaman **"Popular Books"**
- **Cara Menggunakan**:
  1. Buka halaman **"Popular Books"**
  2. Pilih jumlah top books yang ingin ditampilkan
  3. Klik **"Show Top Books"**
  4. Sistem menggunakan Max Heap untuk mendapatkan N buku dengan rating tertinggi

#### Cara Kerja

1. Semua buku dimasukkan ke Max Heap
2. Heap otomatis mengurutkan berdasarkan rating (O(log n) insert)
3. Extract top N elements (O(log n) per extraction)
4. Total kompleksitas: O(n log n)

#### Keunggulan Priority Queue

- Efisien untuk mendapatkan top-K elements
- Tidak perlu sort seluruh array
- Dynamic - bisa add/remove sambil maintain ordering

---

## 📍 Lokasi Implementasi File

### Backend

```
backend/
├── Sorting.h           → QuickSort, MergeSort, InsertionSort
├── Searching.h         → BinarySearch, LinearSearch
├── Graph.h             → Graph dengan BFS, Shortest Path
├── BookManager.h       → BST, Stack, Queue, Priority Queue
└── BookManager.cpp     → Implementasi semua data structures
```

### UI

```
ui/
├── BooksCollectionPage.cpp   → Sorting, BST, Stack (Undo)
├── RecommendationPage.cpp    → Graph (Recommendations)
├── BorrowQueuePage.cpp       → Queue (Borrowing)
└── PopularBooksPage.cpp      → Priority Queue (Top Rated)
```

---

## 🎯 Cara Menggunakan Semua Fitur

### 1. Sorting (QuickSort)

```
Books Collection → Klik tombol Judul/Tahun/Rating/Penulis
```

### 2. Searching

```
Books Collection →
  - Ketik di search box (title search)
  - Pilih genre dari dropdown
  - Ketik nama penulis
  - Gunakan BST Search (Binary Search)
```

### 3. Graph

```
Recommendations →
  - Build Graph Database
  - Pilih genre
  - Get Recommendations (BFS traversal)
```

### 4. Tree (BST)

```
Books Collection →
  - Build BST
  - Masukkan judul di BST search box
  - Search BST (O(log n) search)
```

### 5. Stack

```
Books Collection →
  - Hapus buku
  - Klik Undo Hapus (LIFO restore)
```

### 6. Queue

```
Borrow Queue →
  - Add to Queue (enqueue)
  - Process Next (dequeue - FIFO)
```

### 7. Priority Queue

```
Popular Books →
  - Pilih jumlah top books
  - Show Top Books (Max Heap)
```

---

## 📊 Kompleksitas Algoritma

| Data Structure     | Operation       | Time Complexity | Space Complexity |
| ------------------ | --------------- | --------------- | ---------------- |
| **QuickSort**      | Sort            | O(n log n) avg  | O(log n)         |
| **Merge Sort**     | Sort            | O(n log n)      | O(n)             |
| **Binary Search**  | Search          | O(log n)        | O(1)             |
| **Linear Search**  | Search          | O(n)            | O(1)             |
| **Graph (BFS)**    | Traversal       | O(V + E)        | O(V)             |
| **BST**            | Search          | O(log n) avg    | O(n)             |
| **BST**            | Insert          | O(log n) avg    | O(n)             |
| **Stack**          | Push/Pop        | O(1)            | O(n)             |
| **Queue**          | Enqueue/Dequeue | O(1)            | O(n)             |
| **Priority Queue** | Insert          | O(log n)        | O(n)             |
| **Priority Queue** | Extract Max     | O(log n)        | O(n)             |

---

## ✅ Checklist Implementasi

### Fitur Wajib

- ✅ **Sorting**: QuickSort (BUKAN Bubble Sort)
- ✅ **Searching**: Binary Search & Linear Search
- ✅ **Graph**: BFS, Shortest Path, Recommendations

### Fitur Tambahan (Nilai Plus)

- ✅ **Tree**: Binary Search Tree (BST) dengan search O(log n)
- ✅ **Stack**: Undo deletion dengan LIFO
- ✅ **Queue**: Borrowing queue dengan FIFO
- ✅ **Priority Queue**: Top rated books dengan Max Heap

### Integrasi UI

- ✅ Semua fitur terintegrasi dengan UI
- ✅ User-friendly interface
- ✅ Real-time demonstration
- ✅ Error handling
- ✅ Visual feedback

---

## 🔥 Keunggulan Implementasi

1. **Modular**: Setiap data structure di file terpisah
2. **Template-based**: Algoritma bisa digunakan untuk berbagai tipe data
3. **Well-documented**: Setiap fungsi memiliki dokumentasi lengkap
4. **Tested**: Sudah ditest dengan data real
5. **Production-ready**: Error handling, edge cases handled
6. **UI Integration**: Semua fitur bisa diakses dari UI
7. **Performance**: Menggunakan algoritma efisien (O(log n), O(n log n))

---

## 📝 Catatan Penting

1. **Sorting**: Aplikasi menggunakan QuickSort custom, BUKAN `std::sort` atau Bubble Sort
2. **BST**: Harus di-build terlebih dahulu sebelum search
3. **Graph**: Perlu di-build dari data buku untuk mendapatkan hubungan genre
4. **Stack**: Undo hanya bisa dilakukan untuk deletion, tidak untuk edit
5. **Queue**: FIFO - yang pertama masuk akan pertama keluar
6. **Priority Queue**: Otomatis mengurutkan berdasarkan rating

---

## 🚀 Build & Run

```powershell
# Build aplikasi
cd 'build\Desktop_Qt_6_10_1_MinGW_64_bit-Debug'
$env:PATH = "C:\Qt\Tools\CMake_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\mingw1310_64\bin;$env:PATH"
ninja

# Run aplikasi
.\Perpustakaan_Digital_2.exe
```

---

**Dibuat oleh**: GitHub Copilot  
**Tanggal**: 15 Desember 2025  
**Versi**: 1.0
