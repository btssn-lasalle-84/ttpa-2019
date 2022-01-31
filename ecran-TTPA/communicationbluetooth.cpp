#include "communicationbluetooth.h"
#include <QDebug>

/**
  *
  * @file ecran-TTPA/communicationbluetooth.cpp
  *
  * @brief Définition de la classe CommunicationBluetooth
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */

/**
  *
  * @fn CommunicationBluetooth::CommunicationBluetooth
  *
  * @brief Définition du constructeur la classe CommunicationBluetooth
  *
  * @param parent QObject *
  *
  */
CommunicationBluetooth::CommunicationBluetooth(QObject *parent) : QObject(parent), serveur(nullptr), socket(nullptr)
{
    qDebug() << Q_FUNC_INFO;
    etatConnexion = false;
    connexionErreur = false;
    etatSeance = EtatSeance::Initial;
    if (!monBluetooth.isValid())
    {
        qDebug() << Q_FUNC_INFO << "Bluetooth valide" << monBluetooth.isValid();
        return;
    }

    qDebug() << Q_FUNC_INFO << monBluetooth.hostMode();
    monBluetooth.powerOn();
    nom = monBluetooth.name();
    adresse = monBluetooth.address().toString();
    qDebug() << Q_FUNC_INFO << "nom" << nom << "adresse" << adresse;
    monBluetooth.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    //monBluetooth.setHostMode(QBluetoothLocalDevice::HostConnectable);

    connect(&monBluetooth, SIGNAL(deviceConnected(QBluetoothAddress)), this, SLOT(deviceConnected(QBluetoothAddress)));
    connect(&monBluetooth, SIGNAL(deviceDisconnected(QBluetoothAddress)), this, SLOT(deviceDisconnected(QBluetoothAddress)));
    connect(&monBluetooth, SIGNAL(error(QBluetoothLocalDevice::Error)), this, SLOT(error(QBluetoothLocalDevice::Error)));
}

/**
  *
  * @fn CommunicationBluetooth::~CommunicationBluetooth
  *
  * @brief Définition du destructeur la classe CommunicationBluetooth
  *
  *
  */
CommunicationBluetooth::~CommunicationBluetooth()
{
    arreter();
    monBluetooth.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    qDebug() << Q_FUNC_INFO;
}


/**
 * @brief demarre le serveur
 * @fn CommunicationBluetooth::demarrer()
 */
void CommunicationBluetooth::demarrer()
{
    if (!serveur)
    {
        serveur = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouveauClient()));

        QBluetoothUuid uuid = QBluetoothUuid(serviceUuid);
        serviceInfo = serveur->listen(uuid, serviceNom);
        qDebug() << Q_FUNC_INFO << "serviceInfo" << serviceInfo;
    }
}

/**
 * @brief arrete le serveur
 * @fn CommunicationBluetooth::arreter()
 */
void CommunicationBluetooth::arreter()
{
    if (!serveur)
        return;

    serviceInfo.unregisterService();

    if (socket)
    {
        if (socket->isOpen())
           socket->close();
        delete socket;
        socket = nullptr;
    }

    delete serveur;
    serveur = nullptr;
}

/**
 * @brief la socket est connecté
 *
 * @fn CommunicationBluetooth::socketConnected()
 */
void CommunicationBluetooth::socketConnected()
{
    qDebug() << Q_FUNC_INFO;
    connect(this, SIGNAL(donneesRecues(QString)),this, SLOT(decoderTrame(QString)));
    etatConnexion = true;
    connexionErreur = false;
}

/**
 * @brief la socket est déconnecté
 *
 * @fn CommunicationBluetooth::socketDisconnected()
 */
void CommunicationBluetooth::socketDisconnected()
{
    qDebug() << Q_FUNC_INFO;
    //disconnect(this, SIGNAL(donneesRecues(QString)),this, SLOT(decoderTrame(QString)));
    etatConnexion = false;
    connexionErreur = false;
}

/**
 * @brief la socket est prête à lire les informations
 *
 * @fn CommunicationBluetooth::socketReadyRead()
 */
void CommunicationBluetooth::socketReadyRead()
{
    QByteArray donneesRecues;

    while(socket->canReadLine())
    {
        donneesRecues += socket->readLine();
    }

    if(donneesRecues.startsWith("$"))
    {
        qDebug() << Q_FUNC_INFO << "donneesRecues" << donneesRecues;
        decoderTrame(QString(donneesRecues.data()));
        donneesRecues.clear();
    }
}

/**
 * @brief permet de décoder la trame émise par le terminal mobile
 *
 * @fn CommunicationBluetooth::decoderTrame()
 *
 * @param trame QString la trame ecran-ttpa-1
 */
void CommunicationBluetooth::decoderTrame(QString trame)
{
    QStringList trames;
    trame.remove("\r\n");
    qDebug() << Q_FUNC_INFO << "trame" << trame;

    const QString typeTrame = "ecran-ttpa";
    QString donnees;
    QString nomJoueur;
    QString zoneRobot;
    QString zoneObjectif;
    QString numeroZone;
    QString infoDuRobot;

    if(trame.startsWith("$" + typeTrame))
    {
        /**
         * Démarrage
         * $ecran-ttpa-1:START;JULIEN;2;1*XX
         *                        |   | |
         *                        |   | |
         *                        |   | +- Zone Robot
         *                        |   +--- Zone Objectif
         *                        +------- Nom Joueur
         *
         * $ecran-ttpa-1:STAT;2*XX
         *                    |
         *                    |
         *                    +----------- Numéro Zone Impact
         */
        donnees = trame.section(':', 1, 1); // START;JULIEN;2;1*XX
        if(trame.contains("START") && (etatSeance == EtatSeance::Initial || etatSeance == EtatSeance::Terminee))
        {
            etatSeance = EtatSeance::EnCours;
            // START;JULIEN;2;1*XX
            nomJoueur = donnees.section(';', 1, 1);
            zoneObjectif = donnees.section(';', 2, 2)/*.at(0)*/;
            zoneRobot = donnees.section(';', 3, 3).at(0);
            qDebug() << Q_FUNC_INFO << "nomJoueur" << nomJoueur << "zoneObjectif" << zoneObjectif << "zoneRobot" << zoneRobot;
            emit departSeance(nomJoueur, zoneObjectif, zoneRobot);
        }
        else if(trame.contains("STAT") && etatSeance == EtatSeance::EnCours)
        {
            // STAT;2*XX
            numeroZone = donnees.section(';', 1, 1).at(0);
            qDebug() << Q_FUNC_INFO << "numeroZone" << numeroZone;
            emit impactZone(numeroZone);
        }
        else if(trame.contains("PAUSE") && etatSeance == EtatSeance::EnCours)
        {
            etatSeance = EtatSeance::EnPause;
            //PAUSE*XX
            emit mettreEnPauseSeance();
        }
        else if(trame.contains("RESUME") && etatSeance == EtatSeance::EnPause)
        {
            etatSeance = EtatSeance::EnCours;
            //RESUME*XX
            emit repriseSeance();
        }
        else if(trame.contains("END") && etatSeance == EtatSeance::EnCours)
        {
            etatSeance = EtatSeance::Terminee;
            //END*XX
            emit finSeance();
        }
        else if(trame.contains("RESET") && (etatSeance == EtatSeance::EnCours || etatSeance == EtatSeance::EnPause || etatSeance == EtatSeance::Terminee))
        {
            etatSeance = EtatSeance::Initial;
            //RESET*XX
            emit resetSeance();
        }
        else if(trame.contains("INFO") && etatSeance == EtatSeance::EnCours)
        {
            etatSeance = EtatSeance::EnPause;
            //INFO*XX
            infoDuRobot = donnees.section(';', 1, 1);
            //QString remove(20,33);
            //QString s = "Bourrage du robot ou absence de balles*00";
            infoDuRobot.remove(38, 41);
            emit erreurRobot(infoDuRobot);
            qDebug() << Q_FUNC_INFO << "infoDuRobot" << infoDuRobot;
        }
    }
}

/**
 * @brief la socket indique une erreur de connexion
 * @fn CommunicationBluetooth::socketErreur()
 * @param error QBluetoothSocket::SocketError code d'erreur
 */
void CommunicationBluetooth::socketErreur(QBluetoothSocket::SocketError error)
{
    qDebug() << Q_FUNC_INFO << "erreur" << error;
    etatConnexion = false;
    connexionErreur = true;
}

CommunicationBluetooth::EtatSeance CommunicationBluetooth::getEtatSeance()
{
    return etatSeance;
}

void CommunicationBluetooth::nouveauClient()
{
    // on récupère la socket
    socket = serveur->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
    //connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    //connect(socket, SIGNAL(stateChanged(QBluetoothSocket::SocketState)), this, SLOT(socketStateChanged(QBluetoothSocket::SocketState)));

    etatConnexion = true;
    qDebug() << Q_FUNC_INFO;
    emit connecte();
}

void CommunicationBluetooth::deviceConnected(const QBluetoothAddress &adresse)
{
    qDebug() << Q_FUNC_INFO << adresse << adresse.toString() << monBluetooth.pairingStatus(adresse);
}

void CommunicationBluetooth::deviceDisconnected(const QBluetoothAddress &adresse)
{
    emit deconnecte();
    qDebug() << Q_FUNC_INFO << adresse;
}

void CommunicationBluetooth::error(QBluetoothLocalDevice::Error erreur)
{
    qDebug() << Q_FUNC_INFO << erreur;
}
