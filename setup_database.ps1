# Script untuk populate database dengan sample books
# Perpustakaan Digital - Database Population Script

Write-Host "========================================"
Write-Host "  Perpustakaan Digital"
Write-Host "  Database Population Script"
Write-Host "========================================"
Write-Host ""

# Path ke database
$dbPath = ".\data\perpustakaan.db"
$dataDir = ".\data"

# Create data directory if not exists
if (!(Test-Path $dataDir)) {
    New-Item -ItemType Directory -Path $dataDir | Out-Null
    Write-Host "[OK] Created data directory"
}

# Check if database exists
if (Test-Path $dbPath) {
    Write-Host "[!] Database already exists: $dbPath"
    $response = Read-Host "Do you want to reset the database? (y/N)"
    if ($response -eq 'y' -or $response -eq 'Y') {
        Remove-Item $dbPath -Force
        Write-Host "[OK] Database reset"
    } else {
        Write-Host "[!] Keeping existing database"
        Write-Host ""
        Write-Host "To add data, the application will use existing database."
        pause
        exit
    }
}

Write-Host ""
Write-Host "Creating database and tables..." -ForegroundColor Cyan

# Sample books data
$books = @(
    @{
        id = 1
        judul = "Laskar Pelangi"
        penulis = "Andrea Hirata"
        genre = "Fiksi,Inspiratif,Drama"
        tahun = 2005
        rating = 4.8
        imagePath = ""
    },
    @{
        id = 2
        judul = "Bumi Manusia"
        penulis = "Pramoedya Ananta Toer"
        genre = "Sejarah,Fiksi,Drama"
        tahun = 1980
        rating = 4.9
        imagePath = ""
    },
    @{
        id = 3
        judul = "Ayat-Ayat Cinta"
        penulis = "Habiburrahman El Shirazy"
        genre = "Romansa,Religi,Drama"
        tahun = 2004
        rating = 4.6
        imagePath = ""
    },
    @{
        id = 4
        judul = "Negeri 5 Menara"
        penulis = "Ahmad Fuadi"
        genre = "Inspiratif,Biografi,Drama"
        tahun = 2009
        rating = 4.7
        imagePath = ""
    },
    @{
        id = 5
        judul = "Perahu Kertas"
        penulis = "Dee Lestari"
        genre = "Romansa,Drama,Fiksi"
        tahun = 2009
        rating = 4.5
        imagePath = ""
    },
    @{
        id = 6
        judul = "Sang Pemimpi"
        penulis = "Andrea Hirata"
        genre = "Inspiratif,Drama,Fiksi"
        tahun = 2006
        rating = 4.7
        imagePath = ""
    },
    @{
        id = 7
        judul = "Dilan 1990"
        penulis = "Pidi Baiq"
        genre = "Romansa,Drama,Remaja"
        tahun = 2014
        rating = 4.4
        imagePath = ""
    },
    @{
        id = 8
        judul = "5 cm"
        penulis = "Donny Dhirgantoro"
        genre = "Petualangan,Inspiratif,Drama"
        tahun = 2005
        rating = 4.6
        imagePath = ""
    },
    @{
        id = 9
        judul = "Ronggeng Dukuh Paruk"
        penulis = "Ahmad Tohari"
        genre = "Sejarah,Budaya,Fiksi"
        tahun = 1982
        rating = 4.8
        imagePath = ""
    },
    @{
        id = 10
        judul = "Sepatu Dahlan"
        penulis = "Khrisna Pabichara"
        genre = "Inspiratif,Biografi,Sejarah"
        tahun = 2012
        rating = 4.5
        imagePath = ""
    },
    @{
        id = 11
        judul = "Edensor"
        penulis = "Andrea Hirata"
        genre = "Petualangan,Drama,Inspiratif"
        tahun = 2007
        rating = 4.6
        imagePath = ""
    },
    @{
        id = 12
        judul = "Gadis Kretek"
        penulis = "Ratih Kumala"
        genre = "Sejarah,Romansa,Drama"
        tahun = 2012
        rating = 4.7
        imagePath = ""
    },
    @{
        id = 13
        judul = "Hujan"
        penulis = "Tere Liye"
        genre = "Fantasi,Petualangan,Fiksi"
        tahun = 2016
        rating = 4.8
        imagePath = ""
    },
    @{
        id = 14
        judul = "Pulang"
        penulis = "Tere Liye"
        genre = "Petualangan,Drama,Inspiratif"
        tahun = 2015
        rating = 4.7
        imagePath = ""
    },
    @{
        id = 15
        judul = "Cantik Itu Luka"
        penulis = "Eka Kurniawan"
        genre = "Fantasi,Sejarah,Drama"
        tahun = 2002
        rating = 4.6
        imagePath = ""
    },
    @{
        id = 16
        judul = "Laut Bercerita"
        penulis = "Leila S. Chudori"
        genre = "Sejarah,Politik,Drama"
        tahun = 2017
        rating = 4.8
        imagePath = ""
    },
    @{
        id = 17
        judul = "The Naked Traveler"
        penulis = "Trinity"
        genre = "Petualangan,Travel,Non-Fiksi"
        tahun = 2011
        rating = 4.5
        imagePath = ""
    },
    @{
        id = 18
        judul = "Bumi"
        penulis = "Tere Liye"
        genre = "Fantasi,Petualangan,Remaja"
        tahun = 2014
        rating = 4.9
        imagePath = ""
    },
    @{
        id = 19
        judul = "Orang-Orang Biasa"
        penulis = "Andrea Hirata"
        genre = "Drama,Fiksi,Inspiratif"
        tahun = 2019
        rating = 4.4
        imagePath = ""
    },
    @{
        id = 20
        judul = "Selamat Tinggal"
        penulis = "Tere Liye"
        genre = "Romansa,Drama,Fiksi"
        tahun = 2020
        rating = 4.7
        imagePath = ""
    }
)

Write-Host "[OK] Prepared $($books.Count) books for insertion" -ForegroundColor Green
Write-Host ""
Write-Host "Please run the application now." -ForegroundColor Cyan
Write-Host "The application will automatically create the database" -ForegroundColor Cyan
Write-Host "and insert these sample books on first launch." -ForegroundColor Cyan
Write-Host ""
Write-Host "Sample Books to be Added:" -ForegroundColor Yellow
Write-Host "=========================" -ForegroundColor Yellow
foreach ($book in $books) {
    Write-Host "  [$($book.id)] $($book.judul) - $($book.penulis) ($($book.tahun)) ⭐$($book.rating)" -ForegroundColor White
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Setup Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Export to JSON for application to import
$jsonPath = ".\data\books.json"
$booksJson = @{
    books = $books
}

$booksJson | ConvertTo-Json -Depth 10 | Out-File -FilePath $jsonPath -Encoding UTF8

Write-Host "[OK] Sample data exported to: $jsonPath" -ForegroundColor Green
Write-Host "[OK] The application will import this on first launch" -ForegroundColor Green
Write-Host ""
Write-Host "Press any key to exit..." -ForegroundColor Gray
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
