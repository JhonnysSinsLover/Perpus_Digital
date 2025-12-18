/**
 * ============================================================================
 * QUEUE IMPLEMENTATION - FIFO (First In First Out)
 * ============================================================================
 * 
 * Implementasi Queue untuk sistem antrian peminjaman buku
 * 
 * Struktur Data: Queue (Antrian)
 * Kegunaan: Mengelola antrian peminjaman buku
 * Prinsip: FIFO - First In First Out (yang pertama masuk, keluar duluan)
 * 
 * Kompleksitas:
 * - Enqueue (push): O(1) - menambah ke belakang antrian
 * - Dequeue (pop): O(1) - mengambil dari depan antrian
 * - Front: O(1) - melihat elemen terdepan
 * - Empty: O(1) - cek apakah queue kosong
 */

#include "BookManager.h"
#include <QDebug>
#include <QDateTime>

// ============================================================================
// Borrow Request Structure (dari BookManager.h)
// ============================================================================
/*
struct BorrowRequest {
    QString borrowerName;
    int bookId;
    QString bookTitle;
    QString requestTime;
    
    BorrowRequest() : bookId(-1) {}
    BorrowRequest(QString name, int id, QString title, QString time)
        : borrowerName(name), bookId(id), bookTitle(title), requestTime(time) {}
};
*/

// ============================================================================
// Queue Declaration (dari BookManager.h)
// ============================================================================
/*
private:
    std::queue<BorrowRequest> m_borrowQueue;  // Queue untuk borrow request
*/

// ============================================================================
// ADD TO BORROW QUEUE - Menambah request ke belakang antrian
// Enqueue = FIFO (First In First Out)
// ============================================================================

bool BookManager::addToBorrowQueue(const QString& borrowerName, int bookId)
{
    // Cari buku berdasarkan ID
    Book* book = getBookById(bookId);
    if (!book) {
        qDebug() << "Book with ID" << bookId << "not found";
        return false;
    }
    
    // Buat borrow request dengan timestamp
    QString currentTime = QDateTime::currentDateTime()
                              .toString("yyyy-MM-dd HH:mm:ss");
    BorrowRequest request(borrowerName, bookId, book->getJudul(), currentTime);
    
    // PUSH/ENQUEUE - Tambahkan ke belakang antrian (FIFO)
    // Request baru selalu ditambahkan di belakang
    m_borrowQueue.push(request);
    
    qDebug() << "Added borrow request:" << borrowerName 
             << "for" << book->getJudul();
    return true;
}

// ============================================================================
// PROCESS NEXT BORROW - Proses request pertama di antrian
// Dequeue = FIFO (First In First Out)
// ============================================================================

BookManager::BorrowRequest BookManager::processNextBorrow()
{
    // CHECK IF QUEUE IS EMPTY
    if (m_borrowQueue.empty()) {
        qDebug() << "Borrow queue is empty";
        return BorrowRequest();  // Return empty request
    }
    
    // FRONT - Lihat request yang paling depan (yang pertama masuk)
    // POP/DEQUEUE - Ambil dan hapus dari depan antrian (FIFO)
    BorrowRequest request = m_borrowQueue.front();  // .front() = lihat elemen terdepan
    m_borrowQueue.pop();                             // .pop() = hapus elemen terdepan
    
    qDebug() << "Processed borrow request:" << request.borrowerName 
             << "for" << request.bookTitle;
    return request;
}

// ============================================================================
// GET ALL BORROW REQUESTS - Lihat semua request tanpa mengubah queue
// ============================================================================

std::vector<BookManager::BorrowRequest> BookManager::getAllBorrowRequests() const
{
    std::vector<BorrowRequest> requests;
    
    // Copy queue untuk iterasi (tidak mengubah original queue)
    std::queue<BorrowRequest> tempQueue = m_borrowQueue;
    
    // Ambil semua elemen dari copy queue
    while (!tempQueue.empty()) {
        requests.push_back(tempQueue.front());
        tempQueue.pop();
    }
    
    return requests;
}

// ============================================================================
// CONTOH PENGGUNAAN QUEUE:
// ============================================================================
/*
// 3 orang mengajukan peminjaman buku secara berurutan:
addToBorrowQueue("Alice", 1);   // Alice mengajukan -> PUSH ke queue
addToBorrowQueue("Bob", 2);     // Bob mengajukan -> PUSH ke queue
addToBorrowQueue("Charlie", 3); // Charlie mengajukan -> PUSH ke queue

// Queue sekarang (dari depan ke belakang):
// FRONT -> [Alice] [Bob] [Charlie] <- BACK
//           ↑                ↑
//        Yang pertama      Yang terakhir
//        masuk akan        masuk menunggu
//        dilayani dulu     di belakang

// Proses peminjaman (dequeue):
processNextBorrow();  // Process Alice (front of queue) -> POP
processNextBorrow();  // Process Bob (now front) -> POP
processNextBorrow();  // Process Charlie (now front) -> POP

// Queue sekarang kosong
*/

// ============================================================================
// VISUALISASI QUEUE - FIFO PRINCIPLE:
// ============================================================================
/*
ENQUEUE (menambah ke belakang):

    +-----+-----+-----+-----+
    |  A  |  B  |  C  |  D  |  <- Enqueue D (ditambah di belakang)
    +-----+-----+-----+-----+
    FRONT                 BACK
    (keluar)            (masuk)

DEQUEUE (mengambil dari depan):

    +-----+-----+-----+-----+
    |  A  |  B  |  C  |  D  |  <- Dequeue A (diambil dari depan)
    +-----+-----+-----+-----+
    FRONT                 BACK
    (keluar)            (masuk)
    
    Setelah dequeue A:
    +-----+-----+-----+
    |  B  |  C  |  D  |
    +-----+-----+-----+
    FRONT           BACK

Queue seperti antrian di kasir:
- Orang baru datang mengantri di belakang (ENQUEUE)
- Yang pertama datang dilayani duluan (DEQUEUE)
- Prinsip: Fair/adil, first come first served
*/

// ============================================================================
// KEUNTUNGAN QUEUE UNTUK BORROW SYSTEM:
// ============================================================================
// 1. Fair: First Come First Served (FCFS)
// 2. Efisien: O(1) untuk enqueue dan dequeue
// 3. Natural: seperti antrian di dunia nyata
// 4. Terorganisir: request diproses sesuai urutan kedatangan
// 5. Mudah tracking: bisa lihat siapa yang menunggu
//
// OPERASI QUEUE YANG DIGUNAKAN:
// 1. push() - menambahkan elemen ke belakang queue
// 2. pop() - menghapus elemen dari depan queue
// 3. front() - melihat elemen terdepan tanpa menghapus
// 4. back() - melihat elemen terbelakang tanpa menghapus (optional)
// 5. empty() - mengecek apakah queue kosong
// 6. size() - mendapatkan jumlah elemen di queue (optional)
// ============================================================================

// ============================================================================
// PERBEDAAN STACK vs QUEUE:
// ============================================================================
/*
STACK (LIFO):                      QUEUE (FIFO):
+-------+                          +---+---+---+---+
| [C]   | <- Push/Pop              | A | B | C | D |
| [B]   | <- Push/Pop              +---+---+---+---+
| [A]   | <- Push/Pop              ↑               ↑
+-------+                         POP            PUSH
TOP (akses)                       FRONT          BACK
                                  (keluar)       (masuk)

Stack: Terakhir masuk, keluar duluan
Queue: Pertama masuk, keluar duluan

Analogi:
Stack = Tumpukan piring         Queue = Antrian kasir
Pop dari atas                   Dequeue dari depan
Push ke atas                    Enqueue ke belakang
*/
// ============================================================================

// ============================================================================
// IMPLEMENTASI INTERNAL std::queue:
// ============================================================================
/*
std::queue menggunakan container adapter (biasanya std::deque)
dengan interface yang dibatasi:

template<typename T, typename Container = std::deque<T>>
class queue {
    Container c;  // underlying container
    
public:
    void push(const T& value) { c.push_back(value); }    // Tambah ke belakang
    void pop() { c.pop_front(); }                         // Hapus dari depan
    T& front() { return c.front(); }                      // Lihat elemen depan
    T& back() { return c.back(); }                        // Lihat elemen belakang
    bool empty() const { return c.empty(); }
    size_t size() const { return c.size(); }
};

Hanya operasi FIFO yang diekspos ke user!
*/
// ============================================================================

// ============================================================================
// USE CASE DI APLIKASI:
// ============================================================================
/*
1. Peminjaman Buku:
   - User mengajukan peminjaman -> masuk antrian
   - Admin proses sesuai urutan -> first come first served
   - Fair untuk semua user

2. Reservasi:
   - Buku sedang dipinjam
   - User lain bisa reservasi -> masuk queue
   - Ketika buku kembali, yang pertama reservasi dapat duluan

3. Notifikasi:
   - Queue bisa digunakan untuk antrian notifikasi
   - Kirim notifikasi sesuai urutan request
*/
// ============================================================================
