#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QStyleFactory>
#include <QPalette>

int main(int argc, char *argv[])
{
    // High DPI Support (Qt 6+ enables this by default)
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);

    // 1. Font Standar (Agar tidak terlihat jadul)
    QFont font("Segoe UI", 10);
    a.setFont(font);

    // 2. Style Dasar (Fusion adalah yang paling stabil)
    a.setStyle(QStyleFactory::create("Fusion"));

    // 3. FORCE PALETTE (Memaksa warna terang, anti-dark mode error)
    QPalette p = a.palette();
    p.setColor(QPalette::Window, QColor(240, 242, 245));      // Background Abu Muda
    p.setColor(QPalette::WindowText, QColor(33, 37, 41));     // Teks Hitam
    p.setColor(QPalette::Base, QColor(255, 255, 255));        // Input Field Putih
    p.setColor(QPalette::AlternateBase, QColor(240, 242, 245));
    p.setColor(QPalette::Text, QColor(33, 37, 41));           // Teks Input Hitam
    p.setColor(QPalette::Button, QColor(255, 255, 255));      // Tombol Putih
    p.setColor(QPalette::ButtonText, QColor(33, 37, 41));     // Teks Tombol Hitam
    a.setPalette(p);

    // 4. STYLESHEET AMAN (Hanya memperbaiki komponen dasar)
    a.setStyleSheet(R"(
        /* Reset Dasar */
        * { outline: none; }
        
        /* Input Field (Pasti terlihat) */
        QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox {
            background-color: #ffffff;
            border: 1px solid #ced4da;
            border-radius: 6px;
            padding: 6px 10px;
            color: #212529; /* Hitam */
            selection-background-color: #3b82f6;
        }
        QLineEdit:focus { border: 2px solid #3b82f6; }

        /* ComboBox (Fix Dropdown) */
        QComboBox {
            background-color: #ffffff;
            border: 1px solid #ced4da;
            border-radius: 6px;
            padding: 6px 10px;
            color: #212529;
        }
        QComboBox::drop-down { border: 0px; width: 30px; }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            border: 1px solid #ced4da;
            color: #212529;
            selection-background-color: #3b82f6;
        }

        /* Tombol Standar */
        QPushButton {
            background-color: #ffffff;
            border: 1px solid #ced4da;
            border-radius: 6px;
            padding: 8px 16px;
            color: #212529;
            font-weight: 600;
        }
        QPushButton:hover { background-color: #f8f9fa; }
        QPushButton:pressed { background-color: #e9ecef; }

        /* Tabel (Grid Bersih) */
        QTableWidget {
            background-color: #ffffff;
            border: 1px solid #dee2e6;
            gridline-color: #f1f3f5;
            color: #212529;
        }
        QHeaderView::section {
            background-color: #f8f9fa;
            color: #495057;
            border: none;
            border-bottom: 2px solid #dee2e6;
            padding: 8px;
            font-weight: bold;
        }

        /* Dialog & Pesan (Fix Hitam) */
        QDialog, QMessageBox { background-color: #ffffff; }
        QLabel { color: #212529; }
    )");

    MainWindow w;
    w.show();
    return a.exec();
}