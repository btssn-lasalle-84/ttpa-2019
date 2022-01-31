#ifndef COMMUNICATIONBLUETOOTH_H
#define COMMUNICATIONBLUETOOTH_H

#include <qglobal.h>
#include <QObject>
#include <QLabel>
#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QBluetoothServer>
#include <unistd.h> //usleep

/**
  *
  * @file ecran-TTPA/communicationbluetooth.h
  *
  * @brief Déclaration de la classe CommunicationBluetooth
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */

static const QString serviceUuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));
static const QString serviceNom(QStringLiteral("raspberry"));

/**
  *
  * @class CommunicationBluetooth
  *
  * @brief Déclaration de la classe CommunicationBluetooth
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */
class CommunicationBluetooth: public QObject
{
    Q_OBJECT

    public:
        CommunicationBluetooth(QObject *parent = nullptr);
        ~CommunicationBluetooth();

        /**
         * @brief Les différents état d'une séance d'entraînement
         * @enum EtatSeance
         */
        enum EtatSeance
        {
            Initial = 0,
            EnCours = 1,
            EnPause,
            Terminee
        };

        EtatSeance getEtatSeance();
        void demarrer();
        void arreter();        

    private:
        QBluetoothLocalDevice monBluetooth;  //!< L'interface Bluetooth de la Raspberry Pi
        QBluetoothServer *serveur;  //!< Le serveur Bluetooth
        QBluetoothSocket *socket; //!< la socket de communication Bluetooth
        QBluetoothServiceInfo serviceInfo;
        QString nom;  //!< Le nom de l'interface Bluetooth
        QString adresse;  //!< L'adresse MAC de l'interface Bluetooth
        bool etatConnexion; //!< L'état de connexion
        bool connexionErreur; //!< booléen indiquant une erreur de connexion
        EtatSeance etatSeance; //!< L'état de la séance d'entraînement

        void decoderTrame(QString trame);

    public slots:
        void nouveauClient();
        void socketConnected();
        void socketDisconnected();
        void socketReadyRead();
        void socketErreur(QBluetoothSocket::SocketError error);
        void deviceConnected(const QBluetoothAddress &adresse);
        void deviceDisconnected(const QBluetoothAddress &adresse);
        void error(QBluetoothLocalDevice::Error erreur);

    signals:
        void connecte();
        void deconnecte();
        void erreur();
        //void donneesRecues(QString);
        void impactZone(QString numeroZone);
        void departSeance(QString nomJoueur, QString zoneObjectif, QString zoneRobot);
        void mettreEnPauseSeance();
        void repriseSeance();
        void finSeance();
        void resetSeance();
        void erreurRobot(QString infoDuRobot);
};
#endif // COMMUNICATIONBLUETOOTH_H
