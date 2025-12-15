#include "Book.h"
#include <QJsonArray>

Book Book::fromJson(const QJsonObject& json)
{
    int id = json["id"].toInt();
    QString judul = json["judul"].toString();
    QString penulis = json["penulis"].toString();
    int tahun = json["tahun"].toInt();
    double rating = json["rating"].toDouble();
    QString imagePath = json["imagePath"].toString(); // New field

    QStringList genre;
    QJsonArray genreArray = json["genre"].toArray();
    for (const QJsonValue& value : genreArray) {
        genre.append(value.toString());
    }

    Book book(id, judul, penulis, genre, tahun, rating);
    book.setImagePath(imagePath);
    return book;
}

QJsonObject Book::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    json["judul"] = m_judul;
    json["penulis"] = m_penulis;
    json["tahun"] = m_tahun;
    json["rating"] = m_rating;
    json["imagePath"] = m_imagePath; // New field

    QJsonArray genreArray;
    for (const QString& g : m_genre) {
        genreArray.append(g);
    }
    json["genre"] = genreArray;

    return json;
}
