/**
 * ============================================================================
 * PRIORITY QUEUE & HEAP IMPLEMENTATION
 * ============================================================================
 * 
 * Implementasi Priority Queue (Max Heap) untuk mendapatkan buku populer
 * 
 * Struktur Data: Priority Queue (Max Heap)
 * Kegunaan: Mendapatkan buku dengan rating tertinggi secara efisien
 * Prinsip: Element dengan prioritas tertinggi keluar duluan
 * 
 * Heap Type: Max Heap (parent >= children)
 * - Root = elemen dengan nilai terbesar (rating tertinggi)
 * - Diimplementasikan dengan complete binary tree dalam array
 * 
 * Kompleksitas:
 * - Insert: O(log n) - heapify up
 * - Extract Max: O(log n) - heapify down
 * - Get Max: O(1) - elemen teratas
 * - Build Heap: O(n)
 */

#include "BookManager.h"
#include <queue>
#include <algorithm>

// ============================================================================
// GET TOP RATED BOOKS - Mengambil N buku dengan rating tertinggi
// Menggunakan Priority Queue (Max Heap)
// ============================================================================

std::vector<Book> BookManager::getTopRatedBooks(int topN)
{
    if (m_books.empty()) {
        return std::vector<Book>();
    }
    
    // CREATE MAX HEAP - Priority Queue berdasarkan rating
    // Comparator: a < b untuk MAX HEAP (elemen terbesar di top)
    auto compareRating = [](const Book& a, const Book& b) {
        return a.getRating() < b.getRating(); // Max heap: parent >= children
    };
    
    // std::priority_queue<Type, Container, Comparator>
    std::priority_queue<Book, std::vector<Book>, decltype(compareRating)> 
        maxHeap(compareRating);
    
    // BUILD HEAP - Add all books to priority queue
    // Kompleksitas: O(n log n) untuk n insertions
    for (const Book& book : m_books) {
        maxHeap.push(book);  // Heapify up setiap insert
    }
    
    // EXTRACT TOP N - Ambil N elemen teratas (rating tertinggi)
    // Kompleksitas: O(n log n) untuk n extractions
    std::vector<Book> topBooks;
    for (int i = 0; i < topN && !maxHeap.empty(); i++) {
        topBooks.push_back(maxHeap.top());  // O(1) - lihat top
        maxHeap.pop();                       // O(log n) - heapify down
    }
    
    return topBooks;
}

// ============================================================================
// GET POPULAR BOOKS - Filter buku dengan rating >= threshold
// ============================================================================

std::vector<Book> BookManager::getPopularBooks(double minRating)
{
    std::vector<Book> popularBooks;
    
    // Filter buku berdasarkan rating minimum
    for (const Book& book : m_books) {
        if (book.getRating() >= minRating) {
            popularBooks.push_back(book);
        }
    }
    
    // Sort by rating (descending) menggunakan std::sort
    // Note: Bisa juga pakai priority queue seperti di atas
    std::sort(popularBooks.begin(), popularBooks.end(),
              [](const Book& a, const Book& b) {
                  return a.getRating() > b.getRating();
              });
    
    return popularBooks;
}

// ============================================================================
// VISUALISASI MAX HEAP:
// ============================================================================
/*
Contoh: Books dengan rating [4.8, 4.5, 4.7, 3.9, 4.2, 4.1, 3.8]

Max Heap Structure (Complete Binary Tree):

                    4.8 (root = max)
                   /    \
                4.5      4.7
               /  \      /  \
             3.9  4.2  4.1  3.8

Properti Max Heap:
1. Complete Binary Tree (penuh dari kiri)
2. Parent >= Children di setiap level
3. Root = element terbesar
4. Diimplementasi dalam array:

Array: [4.8, 4.5, 4.7, 3.9, 4.2, 4.1, 3.8]
Index:  [0,   1,   2,   3,   4,   5,   6]

Relasi Parent-Child:
- Parent of i    = (i-1)/2
- Left child of i  = 2*i + 1
- Right child of i = 2*i + 2
*/

// ============================================================================
// OPERASI HEAP:
// ============================================================================
/*
1. INSERT (Heapify Up):
   - Tambahkan elemen di akhir array (maintain complete tree)
   - Bubble up: compare dengan parent, swap jika perlu
   - Repeat sampai properti heap terpenuhi
   
   Example: Insert 4.9 ke heap di atas
   [4.8, 4.5, 4.7, 3.9, 4.2, 4.1, 3.8, 4.9]
                                           ↑ tambah di sini
   
   Bubble up:
   4.9 > 3.9 (parent) → swap
   4.9 > 4.5 (parent) → swap  
   4.9 > 4.8 (parent) → swap
   
   Result: 4.9 menjadi root (max element)

2. EXTRACT MAX (Heapify Down):
   - Ambil root (max element)
   - Replace root dengan elemen terakhir
   - Bubble down: compare dengan children, swap dengan yang lebih besar
   - Repeat sampai properti heap terpenuhi
   
   Example: Extract Max (4.8) dari heap
   [4.8, 4.5, 4.7, 3.9, 4.2, 4.1, 3.8]
   
   Step 1: Remove 4.8, replace dengan 3.8 (last element)
   [3.8, 4.5, 4.7, 3.9, 4.2, 4.1]
   
   Step 2: Bubble down 3.8
   3.8 < max(4.5, 4.7) → swap dengan 4.7
   3.8 < max(4.1, -) → swap dengan 4.1
   
   Result: [4.7, 4.5, 4.1, 3.9, 4.2, 3.8]
*/

// ============================================================================
// CONTOH PENGGUNAAN:
// ============================================================================
/*
// Get top 5 rated books
std::vector<Book> topBooks = bookMgr.getTopRatedBooks(5);

// Hasil (terurut dari rating tertinggi):
// 1. "Book A" - 4.8 ⭐
// 2. "Book B" - 4.7 ⭐
// 3. "Book C" - 4.5 ⭐
// 4. "Book D" - 4.2 ⭐
// 5. "Book E" - 4.1 ⭐

// Process di belakang layar:
// 1. Build max heap dari semua buku: O(n)
// 2. Extract max 5 kali: O(5 log n)
// 3. Total: O(n + k log n) dimana k=5

// Bandingkan dengan sorting biasa:
// - Full sort: O(n log n)
// - Priority Queue untuk top k: O(n + k log n)
// - Lebih efisien jika k << n
*/

// ============================================================================
// PRIORITY QUEUE vs REGULAR QUEUE:
// ============================================================================
/*
REGULAR QUEUE (FIFO):              PRIORITY QUEUE (Heap):
+---+---+---+---+                  Priority:  5   4   3   2   1
| A | B | C | D |                  +-----+-----+-----+-----+-----+
+---+---+---+---+                  | Max | ... | ... | ... | Min |
FRONT         BACK                 +-----+-----+-----+-----+-----+
(keluar)    (masuk)                FRONT                     BACK
                                   (highest priority keluar duluan)

Regular Queue:                     Priority Queue:
- Keluar sesuai urutan masuk       - Keluar sesuai prioritas
- First In First Out               - Highest Priority First Out
- Sederhana                        - Lebih complex (heap)
- O(1) enqueue, dequeue           - O(log n) insert, extract

Use Case:
Queue: Borrow queue               Priority Queue: Top rated books
       (fair, first come)                       (best first)
*/

// ============================================================================
// KEUNTUNGAN PRIORITY QUEUE UNTUK TOP RATED BOOKS:
// ============================================================================
// 1. Efisien untuk top-K queries: O(n + k log n)
// 2. Tidak perlu full sort jika hanya butuh beberapa teratas
// 3. Dinamis: bisa update priority dan reheapify
// 4. Natural untuk ranking/scoring systems
//
// OPERASI PRIORITY QUEUE:
// 1. push() - insert dengan heapify up: O(log n)
// 2. pop() - extract max dengan heapify down: O(log n)
// 3. top() - lihat element tertinggi: O(1)
// 4. empty() - cek kosong: O(1)
// 5. size() - jumlah elements: O(1)
// ============================================================================

// ============================================================================
// MIN HEAP vs MAX HEAP:
// ============================================================================
/*
MAX HEAP (untuk top rated):        MIN HEAP (untuk lowest rated):
        10                                  1
       /  \                                / \
      8    9                              3   2
     / \  / \                            / \ / \
    4  5 6  7                           4  5 6  7

Max Heap: parent >= children          Min Heap: parent <= children
Root = Maximum                        Root = Minimum
Use: Top-K, highest priority         Use: Bottom-K, lowest priority

Di C++ std::priority_queue:
- Default = Max Heap (std::less<T>)
- For Min Heap = std::greater<T>

Example:
// Max Heap (default)
std::priority_queue<int> maxHeap;
maxHeap.push(3); maxHeap.push(1); maxHeap.push(5);
maxHeap.top(); // 5 (largest)

// Min Heap
std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
minHeap.push(3); minHeap.push(1); minHeap.push(5);
minHeap.top(); // 1 (smallest)
*/

// ============================================================================
// IMPLEMENTASI INTERNAL std::priority_queue:
// ============================================================================
/*
template<
    typename T,
    typename Container = std::vector<T>,
    typename Compare = std::less<T>  // Max heap by default
>
class priority_queue {
    Container c;
    Compare comp;
    
public:
    void push(const T& value) {
        c.push_back(value);
        std::push_heap(c.begin(), c.end(), comp);  // Heapify up
    }
    
    void pop() {
        std::pop_heap(c.begin(), c.end(), comp);   // Heapify down
        c.pop_back();
    }
    
    const T& top() const { return c.front(); }
    bool empty() const { return c.empty(); }
    size_t size() const { return c.size(); }
};

Heap operations:
- std::push_heap: Heapify up from last element
- std::pop_heap: Move root to end, heapify down
- std::make_heap: Build heap from unsorted range
*/

// ============================================================================
// APLIKASI PRIORITY QUEUE DI DUNIA NYATA:
// ============================================================================
/*
1. Task Scheduling:
   - OS menggunakan priority queue untuk schedule processes
   - Process dengan priority tinggi dijalankan duluan

2. Dijkstra's Algorithm:
   - Shortest path algorithm
   - Priority queue untuk ambil node dengan distance terkecil

3. Huffman Coding:
   - Compression algorithm
   - Priority queue untuk build optimal prefix tree

4. Event Simulation:
   - Discrete event simulation
   - Priority queue untuk schedule events by time

5. Recommendation System:
   - Like aplikasi kita: top rated books
   - Priority queue untuk ambil items dengan score tertinggi
*/
// ============================================================================
