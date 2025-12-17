cmake_minimum_required(VERSION 3.16)

project(Perpustakaan_Digital_2 VERSION 1.0 LANGUAGES CXX)

# Qt build settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql Charts)

# Additional modules for JSON support
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Charts)
else()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Charts)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/backend
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# Set AUTOUIC search path for .ui files
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

# Backend sources
set(BACKEND_SOURCES
    backend/Book.cpp
    backend/BookManager.cpp
    backend/DatabaseManager.cpp
)

set(BACKEND_HEADERS
    backend/Book.h
    backend/BookManager.h
    backend/DatabaseManager.h
    backend/Sorting.h
    backend/Searching.h
    backend/Graph.h
)

# UI sources
set(UI_SOURCES
    ui/mainwindow.cpp
    ui/AddBookDialog.cpp
    ui/DashboardPage.cpp
    ui/BooksCollectionPage.cpp
    ui/AddBookPage.cpp
    ui/StatisticsPage.cpp
    ui/RecommendationPage.cpp
    ui/BorrowQueuePage.cpp
    ui/PopularBooksPage.cpp
    ui/BookCardWidget.cpp
    ui/BookPreviewDialog.cpp
)

set(UI_HEADERS
    ui/mainwindow.h
    ui/AddBookDialog.h
    ui/DashboardPage.h
    ui/BooksCollectionPage.h
    ui/AddBookPage.h
    ui/StatisticsPage.h
    ui/RecommendationPage.h
    ui/BorrowQueuePage.h
    ui/PopularBooksPage.h
    ui/BookCardWidget.h
    ui/BookPreviewDialog.h
)

set(UI_FORMS
    ui/mainwindow.ui
)

# Main sources
set(PROJECT_SOURCES
    main.cpp
    ${BACKEND_SOURCES}
    ${BACKEND_HEADERS}
    ${UI_SOURCES}
    ${UI_HEADERS}
    ${UI_FORMS}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Perpustakaan_Digital_2
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Perpustakaan_Digital_2 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Perpustakaan_Digital_2 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Perpustakaan_Digital_2
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Perpustakaan_Digital_2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Charts)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Perpustakaan_Digital_2)
endif()
set_target_properties(Perpustakaan_Digital_2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Perpustakaan_Digital_2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Perpustakaan_Digital_2)
endif()
