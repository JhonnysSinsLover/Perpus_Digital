/**
 * ============================================================================
 * STACK IMPLEMENTATION - LIFO (Last In First Out)
 * ============================================================================
 * 
 * Implementasi Stack untuk fitur Undo Delete di Perpustakaan Digital
 * 
 * Struktur Data: Stack (Tumpukan)
 * Kegunaan: Menyimpan buku yang dihapus untuk fitur undo
 * Prinsip: LIFO - Last In First Out (yang terakhir masuk, keluar duluan)
 * 
 * Kompleksitas:
 * - Push: O(1) - menambah ke atas stack
 * - Pop: O(1) - mengambil dari atas stack
 * - Top: O(1) - melihat elemen teratas
 * - Empty: O(1) - cek apakah stack kosong
 */

#include "BookManager.h"
#include <QDebug>

// ============================================================================
// Stack Declaration (dari BookManager.h)
// ============================================================================
/*
private:
    std::stack<Book> m_deletedBooks;  // Stack untuk undo delete feature
*/

// ============================================================================
// REMOVE BOOK - Hapus buku dan simpan ke stack untuk undo
// Push ke stack = LIFO (Last In First Out)
// ============================================================================

bool BookManager::removeBook(int id)
{
    // Find the book to save it to undo stack before removing
    Book* bookToDelete = getBookById(id);
    if (bookToDelete) {
        // PUSH TO STACK (LIFO - Last In First Out)
        // Buku yang dihapus terakhir akan ada di atas stack
        m_deletedBooks.push(*bookToDelete);
        qDebug() << "Added to undo stack:" << bookToDelete->getJudul();
    }
    
    // Remove book from collection
    auto it = std::remove_if(m_books.begin(), m_books.end(),
                             [id](const Book& book) { return book.getId() == id; });
    
    if (it != m_books.end()) {
        m_books.erase(it, m_books.end());
        return true;
    }
    return false;
}

// ============================================================================
// UNDO DELETE - Restore buku yang terakhir dihapus
// Pop dari stack = LIFO (Last In First Out)
// ============================================================================

bool BookManager::undoDelete()
{
    // CHECK IF STACK IS EMPTY
    if (m_deletedBooks.empty()) {
        qDebug() << "Undo stack is empty";
        return false;
    }
    
    // POP FROM STACK (LIFO - Last In First Out)
    // Mengambil buku yang paling terakhir dihapus (top of stack)
    Book restoredBook = m_deletedBooks.top();  // .top() = melihat elemen teratas
    m_deletedBooks.pop();                       // .pop() = menghapus elemen teratas
    
    // Add book back to collection
    m_books.push_back(restoredBook);
    
    qDebug() << "Restored book:" << restoredBook.getJudul();
    return true;
}

// ============================================================================
// CONTOH PENGGUNAAN STACK:
// ============================================================================
/*
// User menghapus 3 buku secara berurutan:
removeBook(1);  // Hapus "Buku A" -> PUSH ke stack
removeBook(2);  // Hapus "Buku B" -> PUSH ke stack  
removeBook(3);  // Hapus "Buku C" -> PUSH ke stack

// Stack sekarang (dari atas ke bawah):
// TOP -> [Buku C] <- yang terakhir dihapus
//        [Buku B]
//        [Buku A] <- yang pertama dihapus
// BOTTOM

// User melakukan undo:
undoDelete();   // Restore "Buku C" (top of stack) -> POP
undoDelete();   // Restore "Buku B" (top of stack) -> POP
undoDelete();   // Restore "Buku A" (top of stack) -> POP

// Stack sekarang kosong
*/

// ============================================================================
// VISUALISASI STACK - LIFO PRINCIPLE:
// ============================================================================
/*
PUSH (menambah):                    POP (mengambil):

    |     |                             |     |
    |     |                             |     |
    |  C  |  <- Push C                  |     |  <- Pop C (yang terakhir masuk, keluar duluan)
    |  B  |  <- Push B                  |  B  |
    |  A  |  <- Push A                  |  A  |
    +-----+                             +-----+
    
Stack seperti tumpukan piring:
- Piring yang terakhir ditaruh di atas (PUSH)
- Piring yang diambil adalah yang paling atas (POP)
- Tidak bisa ambil piring dari tengah atau bawah
*/

// ============================================================================
// KEUNTUNGAN STACK UNTUK UNDO FEATURE:
// ============================================================================
// 1. Sederhana dan efisien: O(1) untuk push dan pop
// 2. LIFO cocok untuk undo: yang terakhir dihapus, undo duluan
// 3. Natural behavior: user biasanya undo aksi terakhir
// 4. Memory efficient: hanya simpan yang perlu di-undo
//
// OPERASI STACK YANG DIGUNAKAN:
// 1. push() - menambahkan elemen ke atas stack
// 2. pop() - menghapus elemen dari atas stack
// 3. top() - melihat elemen teratas tanpa menghapus
// 4. empty() - mengecek apakah stack kosong
// 5. size() - mendapatkan jumlah elemen di stack (optional)
// ============================================================================

// ============================================================================
// IMPLEMENTASI INTERNAL std::stack:
// ============================================================================
/*
std::stack menggunakan container adapter (biasanya std::deque atau std::vector)
dengan interface yang dibatasi:

template<typename T, typename Container = std::deque<T>>
class stack {
    Container c;  // underlying container
    
public:
    void push(const T& value) { c.push_back(value); }
    void pop() { c.pop_back(); }
    T& top() { return c.back(); }
    bool empty() const { return c.empty(); }
    size_t size() const { return c.size(); }
};

Hanya operasi LIFO yang diekspos ke user!
*/
// ============================================================================
