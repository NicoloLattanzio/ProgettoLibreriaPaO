QT += core gui sql multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

SOURCES += \
    ADDItemWidget.cpp \
    edititemwidget.cpp \
    listitemvisitor.cpp \
    main.cpp \
    mainwindow.cpp \
    guivisitor.cpp \
    persistence/DatabaseManager.cpp \
    persistence/DatabaseSaveVisitor.cpp \
    model/Book.cpp \
    model/CD.cpp \
    model/Item.cpp \
    model/Movie.cpp

HEADERS += \
    ADDItemWidget.h \
    edititemwidget.h \
    listitemvisitor.h \
    mainwindow.h \
    guivisitor.h \
    persistence/DatabaseManager.h \
    persistence/DatabaseSaveVisitor.h \
    model/Book.h \
    model/CD.h \
    model/Item.h \
    model/Movie.h \
    model/ConstVisitor.h \
    model/Visitor.h \

HEADERS += $${persistence_HEADERS}
SOURCES += $${persistence_SOURCES}


HEADERS += $${view_HEADERS}
SOURCES += $${view_SOURCES}
FORMS += $${view_FORMS}

# Deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += icons.qrc \
    icons.qrc

