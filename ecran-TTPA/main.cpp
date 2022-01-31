#include "ttpaihm.h"
#include <QApplication>

/**
 * @file ecran-TTPA/main.cpp
 *
 * @brief Programme principal ecran-TTPA
 *
 * @details Crée et affiche la fenêtre principale de l'application ecran-TTPA
 *
 * @author Botella-Broc Yohann
 *
 * @version 1.4
 *
 * @fn main(int argc, char *argv[])
 * @param argc
 * @param argv[]
 * @return int
 *
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TtpaIhm w; // crée la fenêtre principale de l'application
    w.show(); // affiche la fenêtre principale de l'application

    /*// Le splash
    QSplashScreen splash;
    // En avant-plan et sans décoration (titre ...)
    splash.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    // L'image de l'écran de démarrage
    splash.setPixmap(QPixmap(":logo-la-salle.png"));
    // On affiche
    splash.show();
    // On peut afficher un message sur l'image
    splash.showMessage(QString::fromUtf8("Lancement de l'application ..."), Qt::AlignHCenter | Qt::AlignTop, Qt::black);
    // On ferme le splash après un certain temps (ici 2 s)
    QTimer::singleShot(2000, &splash, SLOT(close()));
    // On affiche l'application
    QTimer::singleShot(2000, &w, SLOT(show()));*/


    return a.exec();
}
