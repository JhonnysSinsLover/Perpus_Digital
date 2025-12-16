#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCoreApplication>
#include <QDir>

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
    close();
}

bool DatabaseManager::initialize(const QString& dbPath)
{
    // Use provided path or default to app directory
    m_dbPath = dbPath.isEmpty() ? 
        QCoreApplication::applicationDirPath() + "/perpustakaan.db" : dbPath;
    
    qDebug() << "Initializing database at:" << m_dbPath;
    
    // Create database connection
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_dbPath);
    
    if (!m_database.open()) {
        qWarning() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    
    qDebug() << "Database opened successfully";
    
    // Create tables if they don't exist
    if (!createTables()) {
        qWarning() << "Failed to create tables";
        return false;
    }
    
    // Sync BookManager with database
    syncBookManager();
    
    qDebug() << "Database initialized successfully";
    return true;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query;
    
    // Create books table
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY,
            judul TEXT NOT NULL,
            penulis TEXT NOT NULL,
            genre TEXT NOT NULL,
            tahun INTEGER NOT NULL,
            rating REAL NOT NULL,
            imagePath TEXT
        )
    )";
    
    if (!query.exec(createTableSQL)) {
        qWarning() << "Failed to create books table:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Books table created successfully";
    return true;
}

bool DatabaseManager::isOpen() const
{
    return m_database.isOpen();
}

void DatabaseManager::close()
{
    if (m_database.isOpen()) {
        m_database.close();
        qDebug() << "Database closed";
    }
}

bool DatabaseManager::addBook(const Book& book)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO books (id, judul, penulis, genre, tahun, rating, imagePath)
        VALUES (:id, :judul, :penulis, :genre, :tahun, :rating, :imagePath)
    )");
    
    query.bindValue(":id", book.getId());
    query.bindValue(":judul", book.getJudul());
    query.bindValue(":penulis", book.getPenulis());
    query.bindValue(":genre", book.getGenre().join(", ")); // Store as comma-separated
    query.bindValue(":tahun", book.getTahun());
    query.bindValue(":rating", book.getRating());
    query.bindValue(":imagePath", book.getImagePath());
    
    if (!query.exec()) {
        qWarning() << "Failed to add book:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Book added successfully:" << book.getJudul();
    return true;
}

bool DatabaseManager::updateBook(const Book& book)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE books 
        SET judul = :judul, penulis = :penulis, genre = :genre, 
            tahun = :tahun, rating = :rating, imagePath = :imagePath
        WHERE id = :id
    )");
    
    query.bindValue(":id", book.getId());
    query.bindValue(":judul", book.getJudul());
    query.bindValue(":penulis", book.getPenulis());
    query.bindValue(":genre", book.getGenre().join(", "));
    query.bindValue(":tahun", book.getTahun());
    query.bindValue(":rating", book.getRating());
    query.bindValue(":imagePath", book.getImagePath());
    
    if (!query.exec()) {
        qWarning() << "Failed to update book:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Book updated successfully:" << book.getJudul();
    return true;
}

bool DatabaseManager::deleteBook(int id)
{
    // IMPORTANT: Sync BookManager first to ensure it has the latest data
    syncBookManager();
    
    // Save to undo stack in BookManager before deleting from database
    // This allows the undo functionality to work properly (Stack - LIFO)
    if (m_bookManager.removeBook(id)) {
        qDebug() << "Book saved to undo stack (Stack - LIFO)";
    }
    
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE id = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qWarning() << "Failed to delete book:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "Book deleted successfully from database, ID:" << id;
    return true;
}

Book DatabaseManager::getBookById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE id = :id");
    query.bindValue(":id", id);
    
    if (query.exec() && query.next()) {
        Book book;
        book.setId(query.value("id").toInt());
        book.setJudul(query.value("judul").toString());
        book.setPenulis(query.value("penulis").toString());
        
        // Parse genre from comma-separated string
        QString genreStr = query.value("genre").toString();
        QStringList genres = genreStr.split(",", Qt::SkipEmptyParts);
        for (QString& g : genres) {
            g = g.trimmed();
        }
        book.setGenre(genres);
        
        book.setTahun(query.value("tahun").toInt());
        book.setRating(query.value("rating").toDouble());
        book.setImagePath(query.value("imagePath").toString());
        
        return book;
    }
    
    return Book(); // Return empty book if not found
}

std::vector<Book> DatabaseManager::getAllBooks()
{
    std::vector<Book> books;
    
    QSqlQuery query("SELECT * FROM books ORDER BY id");
    
    while (query.next()) {
        Book book;
        book.setId(query.value("id").toInt());
        book.setJudul(query.value("judul").toString());
        book.setPenulis(query.value("penulis").toString());
        
        // Parse genre
        QString genreStr = query.value("genre").toString();
        QStringList genres = genreStr.split(",", Qt::SkipEmptyParts);
        for (QString& g : genres) {
            g = g.trimmed();
        }
        book.setGenre(genres);
        
        book.setTahun(query.value("tahun").toInt());
        book.setRating(query.value("rating").toDouble());
        book.setImagePath(query.value("imagePath").toString());
        
        books.push_back(book);
    }
    
    qDebug() << "Retrieved" << books.size() << "books from database";
    return books;
}

int DatabaseManager::getBookCount()
{
    QSqlQuery query("SELECT COUNT(*) FROM books");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

std::vector<Book> DatabaseManager::searchByTitle(const QString& title)
{
    std::vector<Book> books;
    
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE judul LIKE :title");
    query.bindValue(":title", "%" + title + "%");
    
    if (query.exec()) {
        while (query.next()) {
            Book book;
            book.setId(query.value("id").toInt());
            book.setJudul(query.value("judul").toString());
            book.setPenulis(query.value("penulis").toString());
            
            QString genreStr = query.value("genre").toString();
            QStringList genres = genreStr.split(",", Qt::SkipEmptyParts);
            for (QString& g : genres) {
                g = g.trimmed();
            }
            book.setGenre(genres);
            
            book.setTahun(query.value("tahun").toInt());
            book.setRating(query.value("rating").toDouble());
            book.setImagePath(query.value("imagePath").toString());
            
            books.push_back(book);
        }
    }
    
    return books;
}

std::vector<Book> DatabaseManager::searchByAuthor(const QString& author)
{
    std::vector<Book> books;
    
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE penulis LIKE :author");
    query.bindValue(":author", "%" + author + "%");
    
    if (query.exec()) {
        while (query.next()) {
            Book book;
            book.setId(query.value("id").toInt());
            book.setJudul(query.value("judul").toString());
            book.setPenulis(query.value("penulis").toString());
            
            QString genreStr = query.value("genre").toString();
            QStringList genres = genreStr.split(",", Qt::SkipEmptyParts);
            for (QString& g : genres) {
                g = g.trimmed();
            }
            book.setGenre(genres);
            
            book.setTahun(query.value("tahun").toInt());
            book.setRating(query.value("rating").toDouble());
            book.setImagePath(query.value("imagePath").toString());
            
            books.push_back(book);
        }
    }
    
    return books;
}

std::vector<Book> DatabaseManager::searchByGenre(const QString& genre)
{
    std::vector<Book> books;
    
    QSqlQuery query;
    query.prepare("SELECT * FROM books WHERE genre LIKE :genre");
    query.bindValue(":genre", "%" + genre + "%");
    
    if (query.exec()) {
        while (query.next()) {
            Book book;
            book.setId(query.value("id").toInt());
            book.setJudul(query.value("judul").toString());
            book.setPenulis(query.value("penulis").toString());
            
            QString genreStr = query.value("genre").toString();
            QStringList genres = genreStr.split(",", Qt::SkipEmptyParts);
            for (QString& g : genres) {
                g = g.trimmed();
            }
            book.setGenre(genres);
            
            book.setTahun(query.value("tahun").toInt());
            book.setRating(query.value("rating").toDouble());
            book.setImagePath(query.value("imagePath").toString());
            
            books.push_back(book);
        }
    }
    
    return books;
}

bool DatabaseManager::clearAllBooks()
{
    QSqlQuery query("DELETE FROM books");
    if (!query.exec()) {
        qWarning() << "Failed to clear books:" << query.lastError().text();
        return false;
    }
    
    qDebug() << "All books cleared from database";
    return true;
}

bool DatabaseManager::importFromJson(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open JSON file for import:" << filePath;
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return false;
    }
    
    QJsonObject root = doc.object();
    QJsonArray booksArray = root["books"].toArray();
    
    int imported = 0;
    for (const QJsonValue& value : booksArray) {
        if (value.isObject()) {
            Book book = Book::fromJson(value.toObject());
            if (addBook(book)) {
                imported++;
            }
        }
    }
    
    qDebug() << "Imported" << imported << "books from JSON";
    return imported > 0;
}

bool DatabaseManager::exportToJson(const QString& filePath)
{
    std::vector<Book> books = getAllBooks();
    
    QJsonArray booksArray;
    for (const Book& book : books) {
        booksArray.append(book.toJson());
    }
    
    QJsonObject root;
    root["books"] = booksArray;
    
    QJsonDocument doc(root);
    
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for export:" << filePath;
        return false;
    }
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Exported" << books.size() << "books to JSON";
    return true;
}

bool DatabaseManager::insertSampleBooks()
{
    // Check if database already has books
    if (getBookCount() > 0) {
        qDebug() << "Database already contains books, skipping sample data insertion";
        return false;
    }
    
    qDebug() << "Inserting 15 sample books...";
    
    // Sample books with diverse genres, authors, years, and ratings
    std::vector<Book> sampleBooks = {
        Book(1, "Laskar Pelangi", "Andrea Hirata", {"Fiksi", "Inspiratif"}, 2005, 4.8, ""),
        Book(2, "Bumi Manusia", "Pramoedya Ananta Toer", {"Sejarah", "Fiksi"}, 1980, 4.9, ""),
        Book(3, "Perahu Kertas", "Dee Lestari", {"Romansa", "Fiksi"}, 2009, 4.6, ""),
        Book(4, "Ayat-Ayat Cinta", "Habiburrahman El Shirazy", {"Romansa", "Inspiratif"}, 2004, 4.7, ""),
        Book(5, "Negeri 5 Menara", "Ahmad Fuadi", {"Inspiratif", "Fiksi"}, 2009, 4.8, ""),
        Book(6, "The Lord of the Rings", "J.R.R. Tolkien", {"Fantasi", "Petualangan"}, 1954, 4.9, ""),
        Book(7, "Harry Potter dan Batu Bertuah", "J.K. Rowling", {"Fantasi", "Petualangan"}, 1997, 4.8, ""),
        Book(8, "1984", "George Orwell", {"Fiksi", "Dystopia"}, 1949, 4.7, ""),
        Book(9, "Sapiens: A Brief History of Humankind", "Yuval Noah Harari", {"Sejarah", "Sains"}, 2011, 4.6, ""),
        Book(10, "The Alchemist", "Paulo Coelho", {"Fiksi", "Inspiratif"}, 1988, 4.5, ""),
        Book(11, "To Kill a Mockingbird", "Harper Lee", {"Fiksi", "Drama"}, 1960, 4.8, ""),
        Book(12, "Pride and Prejudice", "Jane Austen", {"Romansa", "Fiksi"}, 1813, 4.6, ""),
        Book(13, "The Hobbit", "J.R.R. Tolkien", {"Fantasi", "Petualangan"}, 1937, 4.7, ""),
        Book(14, "Cantik Itu Luka", "Eka Kurniawan", {"Fiksi", "Sejarah"}, 2002, 4.5, ""),
        Book(15, "Filosofi Kopi", "Dee Lestari", {"Fiksi", "Inspiratif"}, 2006, 4.6, "")
    };
    
    int inserted = 0;
    for (const Book& book : sampleBooks) {
        if (addBook(book)) {
            inserted++;
        }
    }
    
    qDebug() << "Successfully inserted" << inserted << "sample books";
    
    // Sync with BookManager
    syncBookManager();
    
    return inserted == sampleBooks.size();
}

// ============================================================================
// BookManager Integration
// ============================================================================

void DatabaseManager::syncBookManager()
{
    // Load all books from database to BookManager
    std::vector<Book> books = getAllBooks();
    m_bookManager.clear();
    
    for (const Book& book : books) {
        m_bookManager.addBook(book);
    }
    
    qDebug() << "Synced" << books.size() << "books to BookManager";
}

bool DatabaseManager::saveBookManagerToDatabase()
{
    // Get all books from BookManager
    std::vector<Book> books = m_bookManager.getAllBooks();
    
    // Clear database
    clearAllBooks();
    
    // Insert all books from BookManager
    int saved = 0;
    for (const Book& book : books) {
        if (addBook(book)) {
            saved++;
        }
    }
    
    qDebug() << "Saved" << saved << "books from BookManager to database";
    return saved > 0;
}
