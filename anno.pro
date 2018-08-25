QT += core gui uitools widgets

TARGET = anno
TEMPLATE = app

win32 {
    LIBS += Shell32.lib
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    QResultImageView/QResultImageView.cpp \
    QResultImageView/qt-image-flood-fill/qfloodfill.cpp \
    cpp-move-file-to-trash/move-file-to-trash.cpp

HEADERS  += \
    mainwindow.h \
    QResultImageView/QResultImageView.h \
    QResultImageView/qt-image-flood-fill/qfloodfill.h \
    version.h

FORMS    += \
    mainwindow.ui \
    about.ui

RC_FILE = anno.rc

RESOURCES += \
    anno.qrc

target.path = /$(DESTDIR)$$[QT_INSTALL_BINS]
INSTALLS += target
