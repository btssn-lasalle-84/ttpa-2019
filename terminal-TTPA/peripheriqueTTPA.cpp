/**
  *
  * \file peripheriqueTTPA.cpp
  *
  * \brief Définition de la classe PeripheriqueTTPA
  *
  * \author Hammouma Youssef
  *
  * \version 0.1
  *
  */
#include "peripheriqueTTPA.h"
#include <unistd.h> //usleep

PeripheriqueTTPA::PeripheriqueTTPA(QString nom, QString adresseMAC, QObject *parent) : QObject(parent), nom(nom), adresseMAC(adresseMAC), etatConnexion(false), connexionErreur(false), socket(NULL)
{
    qDebug() << Q_FUNC_INFO << nom << adresseMAC;
}

QString PeripheriqueTTPA::getNom() const
{
    return nom;
}

QString PeripheriqueTTPA::getAdresseMAC() const
{
    return adresseMAC;
}

void PeripheriqueTTPA::connecter()
{
    if (!socket)
    {
        socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
        connect(socket, SIGNAL(connected()), this, SLOT(socketConnecte()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(socketDeconnecte()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
        connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketErreur(QBluetoothSocket::SocketError)));
        //QBluetoothUuid uuid = QBluetoothUuid(QBluetoothUuid::SerialPort);
        //socket->connectToService(QBluetoothAddress(adresseMAC), uuid);
    }
    else if (estConnecte())
    {
        deconnecter();
    }
    qDebug() << Q_FUNC_INFO << nom << adresseMAC;
    QBluetoothUuid uuid = QBluetoothUuid(QBluetoothUuid::SerialPort);
    socket->connectToService(QBluetoothAddress(adresseMAC), uuid);
    socket->open(QIODevice::ReadWrite);
}

void PeripheriqueTTPA::deconnecter()
{
    if (!socket)
        return;
    if (estConnecte())
    {
        qDebug() << Q_FUNC_INFO << nom << adresseMAC;
        socket->disconnectFromService();
        socket->close();
    }
}

void PeripheriqueTTPA::socketConnecte()
{
    qDebug() << Q_FUNC_INFO << nom << adresseMAC << socket->state();
    etatConnexion = true;
    emit connecte();
    //connexionErreur = false;
    //emit erreur();
}

void PeripheriqueTTPA::socketDeconnecte()
{
    qDebug() << Q_FUNC_INFO << nom << adresseMAC << socket->state();
    etatConnexion = false;
    emit deconnecte();
    //connexionErreur = false;
    //emit erreur();
}

void PeripheriqueTTPA::socketReadyRead()
{
    //qDebug() << Q_FUNC_INFO << nom << adresseMAC << socket->state();
    QByteArray trame;

    if(!estConnecte())
        return;

    if (socket->canReadLine())
    {
        trame = socket->readLine();
        //qDebug() << Q_FUNC_INFO << nom << adresseMAC << "trame" << trame;
        emit trameRecue(nom, QString(trame));
    }
}

void PeripheriqueTTPA::socketErreur(QBluetoothSocket::SocketError error)
{
    qDebug() << Q_FUNC_INFO << nom << adresseMAC << "erreur" << error << "state" << socket->state();
    /*etatConnexion = false;
    connexionErreur = true;
    emit connecte();
    emit erreur();*/
}

bool PeripheriqueTTPA::envoyer(QString trame)
{
    if (!estConnecte())
    {
        return false;
    }

    //if(connexionErreur)
    //    return false;

    trame = "$" + trame;
    qDebug() << Q_FUNC_INFO << nom << adresseMAC << "trame" << trame;
    qint64 nbOctets = socket->write(trame.toLatin1());
    if(nbOctets > 0)
        return true;
    return false;
}

bool PeripheriqueTTPA::estConnecte()
{    
    if(!socket)
        return false;
    //qDebug() << Q_FUNC_INFO << nom << adresseMAC << "isOpen" << socket->isOpen() << "state" << socket->state();

    if(socket->isOpen() && socket->state() == QBluetoothSocket::ConnectedState)
        return true;
    return false;
}
