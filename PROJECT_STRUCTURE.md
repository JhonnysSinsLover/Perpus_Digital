# Struktur Folder Project Perpustakaan Digital

## 📁 Struktur Folder Standar

```
Perpustakaan_Digital_2/
├── backend/              # Backend logic dan business layer
│   ├── Book.h/.cpp       # Class untuk entitas Book
│   ├── BookManager.h/.cpp # Manager untuk operasi book
│   ├── DatabaseManager.h/.cpp # Database connection & queries
│   ├── Sorting.h         # Algoritma sorting
│   ├── Searching.h       # Algoritma searching
│   └── Graph.h           # Implementasi graph untuk rekomendasi
│
├── ui/                   # Frontend Qt UI components
│   ├── mainwindow.h/.cpp/.ui # Main window aplikasi
│   ├── *Page.h/.cpp      # Halaman-halaman aplikasi
│   ├── *Dialog.h/.cpp    # Dialog boxes
│   └── *Widget.h/.cpp    # Custom widgets
│
├── data/                 # Data files (JSON, etc)
│   └── books.json        # Initial book data
│
├── CMakeLists.txt        # CMake configuration
├── main.cpp              # Entry point aplikasi
├── README.md             # Dokumentasi utama
├── setup_database.ps1    # Script setup database
└── populate_db.ps1       # Script populate data

## 🚫 Folder yang DI-IGNORE (tidak perlu di-commit)

### Build Artifacts (JANGAN DI-COMMIT!)
```
build/                    # CMake build output
build-*/                  # Qt Creator build variants
*_autogen/               # Qt MOC/UIC/RCC generated files
*.exe, *.dll             # Compiled binaries
```

### IDE Configuration (JANGAN DI-COMMIT!)
```
.qtcreator/              # Qt Creator settings
.vscode/                 # VS Code settings
*.pro.user               # Qt Creator user files
CMakeCache.txt           # CMake cache (machine-specific)
```

### Dependencies (JANGAN DI-COMMIT!)
```
node_modules/            # If using Node.js tools
```

## ⚙️ Setup untuk Developer Lain

### Langkah 1: Clone Repository
```bash
git clone <repository-url>
cd Perpustakaan_Digital_2
```

### Langkah 2: Setup Database
```powershell
# Di PowerShell
.\setup_database.ps1
.\populate_db.ps1
```

### Langkah 3: Build dengan CMake
```bash
# Buat folder build (otomatis di-ignore)
mkdir build
cd build

# Generate build files
cmake ..

# Compile
cmake --build .
```

### Langkah 4: Buka di Qt Creator (Optional)
1. Buka Qt Creator
2. File → Open File or Project
3. Pilih `CMakeLists.txt`
4. Pilih kit yang sesuai (MinGW atau MSVC)
5. Configure project

## 🔧 Requirements

- Qt 6.x atau Qt 5.x
- CMake 3.16+
- C++ 17 compiler (MinGW/MSVC/GCC)
- SQLite (sudah include di Qt)

## ⚠️ Common Issues

### "Cannot open CMakeLists.txt.user"
**Penyebab**: File `.qtcreator/*.user` ter-commit (machine-specific)
**Solusi**: File ini sudah dihapus dari repository. Setiap developer akan generate sendiri.

### "CMakeCache.txt error"
**Penyebab**: CMakeCache.txt dari machine lain ter-commit
**Solusi**: Hapus folder `build/` dan buat ulang dengan `cmake ..`

### "Binary not found"
**Penyebab**: Belum compile atau binary di folder yang salah
**Solusi**: Pastikan sudah run `cmake --build .` di folder build

## 📝 Catatan Penting

1. **JANGAN** commit folder `build/`
2. **JANGAN** commit file `.qtcreator/` atau `*.user`
3. **JANGAN** commit binary executables
4. **LAKUKAN** commit semua source code (.h, .cpp)
5. **LAKUKAN** commit CMakeLists.txt dan data files
6. **LAKUKAN** commit dokumentasi dan scripts

## 🤝 Untuk Kontributor

Sebelum push ke repository:
```bash
# Check status
git status

# Pastikan tidak ada file build artifacts
git status | grep -E "(build/|\.user|CMakeCache|\.exe)"

# Jika ada, pastikan sudah di .gitignore
```
