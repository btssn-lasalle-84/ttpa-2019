QT += qml quick quickcontrols2 bluetooth sql widgets charts
CONFIG += c++11

#DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	BaseDeDonnees.cpp \
    peripheriqueTTPA.cpp \
    CommunicationBluetooth.cpp \
    main.cpp \
    Joueur.cpp \
    Joueurs.cpp \
    Parametre.cpp \
    Parametres.cpp \
    Profil.cpp \
    Profils.cpp

HEADERS += \
	BaseDeDonnees.h \
    peripheriqueTTPA.h \
    CommunicationBluetooth.h \
    Joueur.h \
    Joueurs.h \
    Parametre.h \
    Parametres.h \
    Profil.h \
    Profils.h

RESOURCES += qml.qrc \
            images.qrc \
            icons/terminal-TTPA/index.theme \
            $$files(icons/*.png, true)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

DISTFILES += \
    android-sources/AndroidManifest.xml \
    theme/qtquickcontrols2.conf

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

unix:!macx:
{
    android:
    {
        # déploie la base de données avec l'apk
        deployment.files += ttpa.sqlite
        deployment.path = /assets/db
        INSTALLS += deployment
    }
    !android:
    {
        # copie la base de données dans le dossier build
        CONFIG += file_copies
        COPIES += bd
        bd.files = ttpa.sqlite
        bd.path = $$OUT_PWD/
        bd.base = $$PWD/
    }
}
