# Perpustakaan Digital - Struktur Proyek

## 🚀 Quick Start untuk Developer

### Prerequisites
- Qt 6.x atau Qt 5.x (download dari [qt.io](https://www.qt.io/download))
- CMake 3.16+ 
- C++17 compiler (MinGW/MSVC/GCC)

### Setup Project

1. **Clone repository**
```bash
git clone <repository-url>
cd Perpustakaan_Digital_2
```

2. **Setup database** (PowerShell di Windows)
```powershell
.\setup_database.ps1
.\populate_db.ps1
```

3. **Build dengan CMake**
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

4. **Atau buka di Qt Creator**
- File → Open File or Project
- Pilih `CMakeLists.txt`
- Pilih Kit (MinGW 64-bit atau MSVC)
- Configure → Build → Run

### ⚠️ PENTING: Jangan Commit Build Artifacts!
File-file ini **OTOMATIS** di-generate dan **TIDAK BOLEH** di-commit:
- `build/` folder
- `.qtcreator/` folder
- `*.user` files
- `CMakeCache.txt`
- `*_autogen/` folders
- Binary files (`.exe`, `.dll`)

Lihat [PROJECT_STRUCTURE.md](PROJECT_STRUCTURE.md) untuk detail lengkap.

---

## Struktur Direktori

```
/PerpustakaanDigital
│
├── data/
│   └── books.json          (Database buku dalam format JSON)
│
├── backend/
│   ├── Book.h              (Class Book - SELESAI ✓)
│   ├── Book.cpp            (Implementasi Book - SELESAI ✓)
│   ├── BookManager.h       (Manajemen koleksi buku - SELESAI ✓)
│   ├── BookManager.cpp     (Implementasi BookManager - SELESAI ✓)
│   ├── Sorting.h           (Algoritma sorting - SELESAI ✓)
│   ├── Searching.h         (Algoritma searching - SELESAI ✓)
│   ├── Graph.h             (Graph rekomendasi - SELESAI ✓)
│   └── example_usage.cpp   (Contoh penggunaan - SELESAI ✓)
│
├── ui/
│   ├── mainwindow.h        (Header UI utama)
│   ├── mainwindow.cpp      (Implementasi UI)
│   └── mainwindow.ui       (Qt Designer file)
│
├── CMakeLists.txt          (Build configuration)
└── main.cpp                (Entry point aplikasi)
```

## Class Book

### Fitur:

- **Atribut**: id, judul, penulis, genre (list), tahun, rating
- **Constructor**: Default dan parameterized
- **JSON Support**: `fromJson()` dan `toJson()` untuk parsing data
- **Getters/Setters**: Akses penuh ke semua atribut
- **Utility**: `hasGenre()`, `toString()`, operator perbandingan

### Contoh Penggunaan:

```cpp
#include "Book.h"
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

// 1. Membuat book manual
Book book1(1, "Laskar Pelangi", "Andrea Hirata",
           QStringList{"Fiksi", "Drama", "Inspiratif"}, 2005, 4.8);

qDebug() << book1.toString();
// Output: [1] Laskar Pelangi by Andrea Hirata (2005) - 4.8★

// 2. Load dari JSON file
QFile file("data/books.json");
if (file.open(QIODevice::ReadOnly)) {
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    QJsonArray booksArray = root["books"].toArray();

    QList<Book> books;
    for (const QJsonValue& value : booksArray) {
        Book book = Book::fromJson(value.toObject());
        books.append(book);
    }

    file.close();
}

// 3. Filter by genre
if (book1.hasGenre("Drama")) {
    qDebug() << "Book ini mengandung genre Drama";
}

// 4. Convert to JSON
QJsonObject jsonBook = book1.toJson();
qDebug() << QJsonDocument(jsonBook).toJson(QJsonDocument::Indented);
```

## Clean Code Principles:

✓ Single Responsibility: Class Book hanya menangani data buku
✓ Encapsulation: Private attributes dengan public getters/setters
✓ Documentation: Doxygen comments untuk semua method
✓ Qt Integration: Menggunakan QString, QStringList, QJsonObject
✓ Const Correctness: Getter methods marked as const
✓ Inline Optimization: Simple methods diimplementasi inline

## BookManager Class

### Fitur Utama:

- **Data Storage**: `vector<Book>` untuk menyimpan koleksi buku
- **JSON I/O**: Load dan save dari/ke file JSON
- **CRUD Operations**: Add, get, update, remove books
- **Sorting Algorithms**:
  - QuickSort by title, year, rating, author
  - Complexity: O(n log n) average case
- **Searching Algorithms**:
  - Binary Search by title (requires sorted data) - O(log n)
  - Linear Search by genre, author - O(n)
  - Year range search - O(n)

### Contoh Penggunaan:

```cpp
BookManager manager;

// Load dari JSON
manager.loadFromJson("data/books.json");

// Sorting
manager.quickSortByTitle(true);     // Ascending
manager.quickSortByYear(false);     // Descending
manager.quickSortByRating(false);   // Top rated first

// Searching
Book* book = manager.binarySearchByTitle("Laskar Pelangi");
std::vector<Book> fantasyBooks = manager.searchByGenre("Fantasi");
std::vector<Book> books2010s = manager.searchByYearRange(2010, 2020);

// CRUD
manager.addBook(newBook);
manager.updateBook(modifiedBook);
manager.removeBook(bookId);

// Save
manager.saveToJson("data/books_updated.json");
```

## Graph Class (Adjacency List)

### Struktur Data:

- **Adjacency List**: `map<QString, set<QString>>`
- **Undirected Graph**: Genre connections are bidirectional
- **Node**: Genre names
- **Edge**: Relationship between genres (books that share genres)

### Fitur Utama:

- **buildGraph()**: Automatically create connections from book data
- **getRecommendation()**: BFS-based recommendation system
  - Depth parameter to control exploration distance
  - Scoring system based on genre proximity and rating
- **findPath()**: Find shortest path between genres using BFS
- **Graph Analytics**: Get node degrees, connected genres, etc.

### Algoritma Rekomendasi:

1. **BFS Traversal**: Explore connected genres up to maxDepth
2. **Genre Depth Mapping**: Track distance from target genre
3. **Relevance Scoring**:
   - Score = 10/(depth+1) for each matching genre
   - Bonus: +0.5 × book rating
4. **Sorting**: Return books sorted by relevance score

### Contoh Penggunaan:

```cpp
Graph graph;
BookManager manager;
manager.loadFromJson("data/books.json");

// Build graph from book data
graph.buildGraph(manager.getAllBooks());

// Get recommendations for "Fantasi" genre
std::vector<Book> recs = graph.getRecommendation("Fantasi",
                                                  manager.getAllBooks(),
                                                  2); // depth=2

// Get connected genres
std::set<QString> connected = graph.getConnectedGenres("Drama");

// Find path between genres
std::vector<QString> path = graph.findPath("Fantasi", "Sejarah");
// Output: Fantasi → Petualangan → Drama → Sejarah

// Graph statistics
qDebug() << "Genres:" << graph.getNodeCount();
qDebug() << "Connections:" << graph.getEdgeCount();
qDebug() << "Drama degree:" << graph.getGenreDegree("Drama");
```

## Sorting & Searching Headers

### Sorting.h - Generic Sorting Algorithms:

- **QuickSort**: O(n log n) average, O(n²) worst case
- **MergeSort**: O(n log n) guaranteed, stable sort
- **InsertionSort**: O(n²), good for small arrays
- **BubbleSort**: O(n²), educational purposes

All support custom comparison functions using `std::function`.

### Searching.h - Generic Search Algorithms:

- **Binary Search**: O(log n), requires sorted data
- **Linear Search**: O(n), works on unsorted data
- **Interpolation Search**: O(log log n) for uniform distribution
- **Jump Search**: O(√n), good for sorted arrays
- **findAll()**: Find multiple matches

## Performance Characteristics

| Operation           | Time Complexity | Space Complexity |
| ------------------- | --------------- | ---------------- |
| Load JSON           | O(n)            | O(n)             |
| QuickSort           | O(n log n)      | O(log n)         |
| Binary Search       | O(log n)        | O(1)             |
| BFS (Graph)         | O(V + E)        | O(V)             |
| Get Recommendations | O(V + E + n)    | O(V + n)         |

Where:

- n = number of books
- V = number of genres (vertices)
- E = number of genre connections (edges)

## Testing

Lihat [backend/example_usage.cpp](backend/example_usage.cpp) untuk demonstrasi lengkap semua fitur.

## Next Steps:

1. ✅ Implementasi BookManager untuk CRUD operations
2. ✅ Tambahkan algoritma sorting (Quick Sort, Merge Sort)
3. ✅ Tambahkan algoritma searching (Binary Search, Linear Search)
4. ✅ Implementasi Graph untuk sistem rekomendasi berbasis genre
5. 🔄 Integrasikan dengan UI Qt (in progress)
