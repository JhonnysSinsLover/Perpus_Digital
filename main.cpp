#include "mainwindow.h"
#include <QApplication>
#include <QFont>
#include <QStyleFactory>
#include <QPalette>

int main(int argc, char *argv[])
{
    // 1. Setup High DPI Scaling
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);

    // 2. Set Font Global
    QFont font("Segoe UI", 10);
    a.setFont(font);

    // 3. Pakai Style Fusion
    a.setStyle(QStyleFactory::create("Fusion"));

    // 4. PALETTE OVERRIDE (PENTING: Mencegah Dark Mode Sistem merusak warna)
    // Kita set base palette menjadi terang agar Fusion style tidak ikut-ikutan hitam.
    QPalette p = a.palette();
    p.setColor(QPalette::Window, QColor(245, 246, 250)); // Background window abu muda
    p.setColor(QPalette::WindowText, QColor(30, 41, 59)); // Teks gelap
    p.setColor(QPalette::Base, QColor(255, 255, 255)); // Input field putih
    p.setColor(QPalette::AlternateBase, QColor(245, 246, 250));
    p.setColor(QPalette::ToolTipBase, Qt::white);
    p.setColor(QPalette::ToolTipText, Qt::white);
    p.setColor(QPalette::Text, QColor(30, 41, 59));
    p.setColor(QPalette::Button, QColor(255, 255, 255));
    p.setColor(QPalette::ButtonText, QColor(30, 41, 59));
    p.setColor(QPalette::BrightText, Qt::red);
    p.setColor(QPalette::Link, QColor(42, 130, 218));
    p.setColor(QPalette::Highlight, QColor(42, 130, 218));
    p.setColor(QPalette::HighlightedText, Qt::black);
    a.setPalette(p);

    // 5. GLOBAL STYLESHEET (Updated untuk Fix Dialog Hitam)
    a.setStyleSheet(R"(
        /* === GLOBAL RESET === */
        QWidget {
            color: #1e293b;
            font-family: "Segoe UI";
            outline: none; /* Hilangkan garis putus-putus fokus */
        }

        /* === DIALOGS & MESSAGE BOX (FIX DISINI) === */
        /* Paksa background putih untuk semua popup/dialog */
        QDialog, QMessageBox {
            background-color: #ffffff;
        }
        /* Pastikan label di dalam dialog berwarna gelap */
        QDialog QLabel, QMessageBox QLabel {
            color: #1e293b;
            background-color: transparent; /* Biar tidak ada kotak background di text */
        }

        /* === INPUT FIELDS === */
        QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox {
            background-color: #ffffff;
            border: 1px solid #cbd5e1;
            border-radius: 8px;
            padding: 8px 12px;
            font-size: 14px;
            color: #0f172a;
            selection-background-color: #3b82f6;
            selection-color: #ffffff;
        }
        
        QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #3b82f6;
            background-color: #ffffff;
        }

        /* === COMBOBOX === */
        QComboBox {
            background-color: #ffffff;
            border: 1px solid #cbd5e1;
            border-radius: 8px;
            padding: 8px 12px;
            color: #0f172a;
            font-size: 14px;
        }
        QComboBox:focus {
            border: 2px solid #3b82f6;
        }
        QComboBox::drop-down {
            border: 0px;
            width: 30px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #64748b;
            margin-right: 10px;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            border: 1px solid #cbd5e1;
            selection-background-color: #3b82f6;
            selection-color: #ffffff;
            color: #0f172a;
            outline: none;
            padding: 5px;
        }

        /* === BUTTONS === */
        QPushButton {
            background-color: #3b82f6;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 10px 20px;
            font-weight: 600;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #2563eb;
        }
        QPushButton:pressed {
            background-color: #1d4ed8;
            padding-top: 12px;
            padding-bottom: 8px;
        }
        QPushButton:disabled {
            background-color: #94a3b8;
            color: #e2e8f0;
        }

        /* Button di dalam QMessageBox biasanya perlu width minimal */
        QMessageBox QPushButton {
            min-width: 80px;
        }

        /* === TABLES === */
        QTableWidget, QTableView {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            gridline-color: #f1f5f9;
            color: #334155;
            font-size: 13px;
            selection-background-color: #e0f2fe;
            selection-color: #0f172a;
        }
        QHeaderView::section {
            background-color: #f8fafc;
            color: #64748b;
            font-weight: bold;
            border: none;
            border-bottom: 1px solid #e2e8f0;
            padding: 12px;
            text-transform: uppercase;
            font-size: 12px;
        }
        QTableWidget::item {
            padding: 5px;
        }

        /* === SCROLLBARS === */
        QScrollBar:vertical {
            border: none;
            background: #f1f5f9;
            width: 10px;
            margin: 0px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background: #cbd5e1;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical:hover {
            background: #94a3b8;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        /* === FRAMES / CARDS === */
        QFrame#formCard, QFrame#controlCard, QFrame#card {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 12px;
        }
        
        /* === GROUP BOX === */
        QGroupBox {
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            margin-top: 24px;
            background-color: #ffffff;
            font-weight: bold;
            color: #3b82f6; /* Warna judul groupbox */
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 5px;
        }
    )");

    MainWindow w;
    w.show();
    return a.exec();
}