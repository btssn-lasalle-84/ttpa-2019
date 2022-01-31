#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QIcon>
#include <QQmlContext>
#include <QtSql/QtSql>
#include "CommunicationBluetooth.h"
#include "peripheriqueTTPA.h"
#include "Joueurs.h"
#include "Parametres.h"
#include "Profils.h"

/**
 * @file terminal-TTPA/main.cpp
 *
 * @brief Programme principal terminal-TTPA
 *
 * @details Crée et affiche la fenêtre principale de l'application
 *
 * @author Youssef Hammouma
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
    QCoreApplication::setApplicationName("TTPA");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QQuickStyle::setStyle("Material");
    QIcon::setThemeName("terminal-TTPA");

    CommunicationBluetooth *communicationBluetooth = new CommunicationBluetooth;
    engine.rootContext()->setContextProperty("communicationBluetooth", communicationBluetooth);
    qmlRegisterType<PeripheriqueTTPA>();

    Joueurs *joueurs = new Joueurs;
    engine.rootContext()->setContextProperty("joueurs", joueurs);

    Parametres *parametres = new Parametres;
    engine.rootContext()->setContextProperty("parametres", parametres);

    Profils *profils = new Profils;
    engine.rootContext()->setContextProperty("profils", profils);

    engine.load(QUrl(QStringLiteral("qrc:/FenetreTTPA.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
