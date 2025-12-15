#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QStringList>
#include <QJsonObject>

/**
 * @brief Class representing a book in the digital library
 * 
 * This class encapsulates all the properties of a book including
 * identification, metadata, and rating information.
 */
class Book
{
public:
    /**
     * @brief Default constructor
     */
    Book();

    /**
     * @brief Parameterized constructor
     * @param id Unique identifier for the book
     * @param judul Title of the book
     * @param penulis Author of the book
     * @param genre List of genres
     * @param tahun Publication year
     * @param rating Book rating (0.0 - 5.0)
     */
    Book(int id, const QString& judul, const QString& penulis,
         const QStringList& genre, int tahun, double rating);

    /**
     * @brief Parameterized constructor with image path
     * @param id Unique identifier for the book
     * @param judul Title of the book
     * @param penulis Author of the book
     * @param genre List of genres
     * @param tahun Publication year
     * @param rating Book rating (0.0 - 5.0)
     * @param imagePath Path to book cover image
     */
    Book(int id, const QString& judul, const QString& penulis,
         const QStringList& genre, int tahun, double rating, const QString& imagePath);

    /**
     * @brief Construct from JSON object
     * @param json JSON object containing book data
     */
    static Book fromJson(const QJsonObject& json);

    /**
     * @brief Convert book to JSON object
     * @return QJsonObject representation of the book
     */
    QJsonObject toJson() const;

    // Getters
    int getId() const { return m_id; }
    QString getJudul() const { return m_judul; }
    QString getPenulis() const { return m_penulis; }
    QStringList getGenre() const { return m_genre; }
    int getTahun() const { return m_tahun; }
    double getRating() const { return m_rating; }
    QString getImagePath() const { return m_imagePath; }

    // Setters (optional, for future modifications)
    void setId(int id) { m_id = id; }
    void setJudul(const QString& judul) { m_judul = judul; }
    void setPenulis(const QString& penulis) { m_penulis = penulis; }
    void setGenre(const QStringList& genre) { m_genre = genre; }
    void setTahun(int tahun) { m_tahun = tahun; }
    void setRating(double rating) { m_rating = rating; }
    void setImagePath(const QString& path) { m_imagePath = path; }

    /**
     * @brief Check if book has a specific genre
     * @param genre Genre to check
     * @return true if book has the genre, false otherwise
     */
    bool hasGenre(const QString& genre) const;

    /**
     * @brief Get a formatted string representation of the book
     * @return QString formatted book information
     */
    QString toString() const;

    /**
     * @brief Compare books by ID
     */
    bool operator==(const Book& other) const { return m_id == other.m_id; }
    bool operator!=(const Book& other) const { return m_id != other.m_id; }

private:
    int m_id;                    ///< Unique book identifier
    QString m_judul;             ///< Book title
    QString m_penulis;           ///< Book author
    QStringList m_genre;         ///< List of genres
    int m_tahun;                 ///< Publication year
    double m_rating;             ///< Book rating (0.0 - 5.0)
    QString m_imagePath;         ///< Path to book cover image
};

// Inline implementations for better performance

inline Book::Book()
    : m_id(0), m_judul(""), m_penulis(""), m_genre(), m_tahun(0), m_rating(0.0), m_imagePath("")
{
}

inline Book::Book(int id, const QString& judul, const QString& penulis,
                  const QStringList& genre, int tahun, double rating)
    : m_id(id), m_judul(judul), m_penulis(penulis),
      m_genre(genre), m_tahun(tahun), m_rating(rating), m_imagePath("")
{
}

inline Book::Book(int id, const QString& judul, const QString& penulis,
                  const QStringList& genre, int tahun, double rating, const QString& imagePath)
    : m_id(id), m_judul(judul), m_penulis(penulis),
      m_genre(genre), m_tahun(tahun), m_rating(rating), m_imagePath(imagePath)
{
}

inline bool Book::hasGenre(const QString& genre) const
{
    return m_genre.contains(genre, Qt::CaseInsensitive);
}

inline QString Book::toString() const
{
    return QString("[%1] %2 by %3 (%4) - %5★")
        .arg(m_id)
        .arg(m_judul)
        .arg(m_penulis)
        .arg(m_tahun)
        .arg(m_rating, 0, 'f', 1);
}

#endif // BOOK_H
