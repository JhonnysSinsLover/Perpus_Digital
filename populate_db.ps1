# Perpustakaan Digital - Database Population Script
Write-Host "========================================"
Write-Host "  Database Population Script"
Write-Host "========================================"
Write-Host ""

$jsonPath = ".\data\books.json"
$dataDir = ".\data"

# Create data directory
if (!(Test-Path $dataDir)) {
    New-Item -ItemType Directory -Path $dataDir | Out-Null
    Write-Host "[OK] Created data directory"
}

# Create JSON content
$jsonContent = @"
[
    {
        "id": 1,
        "judul": "Laskar Pelangi",
        "penulis": "Andrea Hirata",
        "genre": ["Fiksi", "Inspiratif"],
        "tahun": 2005,
        "rating": 4.8,
        "imagePath": ""
    },
    {
        "id": 2,
        "judul": "Bumi Manusia",
        "penulis": "Pramoedya Ananta Toer",
        "genre": ["Fiksi", "Sejarah"],
        "tahun": 1980,
        "rating": 4.9,
        "imagePath": ""
    },
    {
        "id": 3,
        "judul": "Ayat-Ayat Cinta",
        "penulis": "Habiburrahman El Shirazy",
        "genre": ["Romance", "Religi"],
        "tahun": 2004,
        "rating": 4.3,
        "imagePath": ""
    },
    {
        "id": 4,
        "judul": "Ronggeng Dukuh Paruk",
        "penulis": "Ahmad Tohari",
        "genre": ["Fiksi", "Budaya"],
        "tahun": 1982,
        "rating": 4.6,
        "imagePath": ""
    },
    {
        "id": 5,
        "judul": "Perahu Kertas",
        "penulis": "Dee Lestari",
        "genre": ["Romance", "Petualangan"],
        "tahun": 2009,
        "rating": 4.4,
        "imagePath": ""
    },
    {
        "id": 6,
        "judul": "Tenggelamnya Kapal Van Der Wijck",
        "penulis": "Hamka",
        "genre": ["Romance", "Klasik"],
        "tahun": 1938,
        "rating": 4.7,
        "imagePath": ""
    },
    {
        "id": 7,
        "judul": "Cantik Itu Luka",
        "penulis": "Eka Kurniawan",
        "genre": ["Fiksi", "Fantasi"],
        "tahun": 2002,
        "rating": 4.5,
        "imagePath": ""
    },
    {
        "id": 8,
        "judul": "Negeri 5 Menara",
        "penulis": "Ahmad Fuadi",
        "genre": ["Inspirasi", "Pendidikan"],
        "tahun": 2009,
        "rating": 4.6,
        "imagePath": ""
    },
    {
        "id": 9,
        "judul": "Sang Pemimpi",
        "penulis": "Andrea Hirata",
        "genre": ["Fiksi", "Inspirasi"],
        "tahun": 2006,
        "rating": 4.4,
        "imagePath": ""
    },
    {
        "id": 10,
        "judul": "5 cm",
        "penulis": "Donny Dhirgantoro",
        "genre": ["Petualangan", "Persahabatan"],
        "tahun": 2005,
        "rating": 4.2,
        "imagePath": ""
    },
    {
        "id": 11,
        "judul": "Hujan",
        "penulis": "Tere Liye",
        "genre": ["Fiksi", "Misteri"],
        "tahun": 2016,
        "rating": 4.5,
        "imagePath": ""
    },
    {
        "id": 12,
        "judul": "Pulang",
        "penulis": "Tere Liye",
        "genre": ["Fiksi", "Drama"],
        "tahun": 2015,
        "rating": 4.7,
        "imagePath": ""
    },
    {
        "id": 13,
        "judul": "Supernova",
        "penulis": "Dee Lestari",
        "genre": ["Fiksi", "Filsafat"],
        "tahun": 2001,
        "rating": 4.4,
        "imagePath": ""
    },
    {
        "id": 14,
        "judul": "Edensor",
        "penulis": "Andrea Hirata",
        "genre": ["Fiksi", "Petualangan"],
        "tahun": 2007,
        "rating": 4.3,
        "imagePath": ""
    },
    {
        "id": 15,
        "judul": "Lelaki Harimau",
        "penulis": "Eka Kurniawan",
        "genre": ["Fiksi", "Thriller"],
        "tahun": 2004,
        "rating": 4.6,
        "imagePath": ""
    },
    {
        "id": 16,
        "judul": "Di Bawah Lindungan Kabah",
        "penulis": "Hamka",
        "genre": ["Romance", "Religi"],
        "tahun": 1936,
        "rating": 4.5,
        "imagePath": ""
    },
    {
        "id": 17,
        "judul": "Ranah 3 Warna",
        "penulis": "Ahmad Fuadi",
        "genre": ["Inspirasi", "Pendidikan"],
        "tahun": 2011,
        "rating": 4.5,
        "imagePath": ""
    },
    {
        "id": 18,
        "judul": "Bumi",
        "penulis": "Tere Liye",
        "genre": ["Fantasi", "Petualangan"],
        "tahun": 2014,
        "rating": 4.8,
        "imagePath": ""
    },
    {
        "id": 19,
        "judul": "Komet",
        "penulis": "Tere Liye",
        "genre": ["Fantasi", "Petualangan"],
        "tahun": 2018,
        "rating": 4.7,
        "imagePath": ""
    },
    {
        "id": 20,
        "judul": "Cinta di Dalam Gelas",
        "penulis": "Andrea Hirata",
        "genre": ["Romance", "Humor"],
        "tahun": 2010,
        "rating": 4.1,
        "imagePath": ""
    }
]
"@

# Write JSON file
$jsonContent | Out-File -FilePath $jsonPath -Encoding UTF8
Write-Host "[OK] Created books.json with 20 sample books"
Write-Host ""
Write-Host "File location: $jsonPath"
Write-Host ""
Write-Host "The application will automatically import this data on first run."
Write-Host "Press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
