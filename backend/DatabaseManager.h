#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QVector>
#include "Book.h"
#include "BookManager.h"

/**
 * @brief SQLite Database Manager for Perpustakaan Digital
 * 
 * Handles all database operations including CRUD for books
 */
class DatabaseManager
{
public:
    /**
     * @brief Get singleton instance
     */
    static DatabaseManager& instance();

    /**
     * @brief Initialize database connection
     * @param dbPath Path to SQLite database file
     * @return true if successful, false otherwise
     */
    bool initialize(const QString& dbPath = "");

    /**
     * @brief Check if database is open and ready
     */
    bool isOpen() const;

    /**
     * @brief Close database connection
     */
    void close();

    // CRUD Operations
    /**
     * @brief Add a new book to database
     */
    bool addBook(const Book& book);

    /**
     * @brief Update existing book
     */
    bool updateBook(const Book& book);

    /**
     * @brief Delete book by ID
     */
    bool deleteBook(int id);

    /**
     * @brief Get book by ID
     */
    Book getBookById(int id);

    /**
     * @brief Get all books from database
     */
    std::vector<Book> getAllBooks();

    /**
     * @brief Get book count
     */
    int getBookCount();

    /**
     * @brief Search books by title
     */
    std::vector<Book> searchByTitle(const QString& title);

    /**
     * @brief Search books by author
     */
    std::vector<Book> searchByAuthor(const QString& author);

    /**
     * @brief Search books by genre
     */
    std::vector<Book> searchByGenre(const QString& genre);

    /**
     * @brief Clear all books from database
     */
    bool clearAllBooks();

    /**
     * @brief Import books from JSON file
     */
    bool importFromJson(const QString& filePath);

    /**
     * @brief Export books to JSON file
     */
    bool exportToJson(const QString& filePath);

    /**
     * @brief Insert sample books for demo purposes
     */
    bool insertSampleBooks();
    
    /**
     * @brief Get BookManager instance for advanced data structure operations
     * @return Reference to BookManager
     */
    BookManager& getBookManager() { return m_bookManager; }
    
    /**
     * @brief Sync BookManager with database (load all books)
     */
    void syncBookManager();
    
    /**
     * @brief Save BookManager books to database
     */
    bool saveBookManagerToDatabase();

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    bool createTables();
    QSqlDatabase m_database;
    QString m_dbPath;
    BookManager m_bookManager;  ///< BookManager for advanced data structures (Stack, Queue, BST, Priority Queue)
};

#endif // DATABASEMANAGER_H
