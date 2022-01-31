/**
  *
  * \file peripheriqueTTPA.h
  *
  * \brief Déclaration de la classe PeripheriqueTTPA
  *
  * \author Hammouma Youssef
  *
  * \version 0.1
  *
  */

#ifndef PeripheriqueTTPA_H
#define PeripheriqueTTPA_H

#include <QObject>
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>

/**
 * \class PeripheriqueTTPA
 *
 * \brief Permet la communication avec les appareils Bluetooth TTPA
 *
 * \author Hammouma Youssef
 *
 * \version 0.1
 *
 * \date 20/03/2019 10:30:20
 *
 */
class PeripheriqueTTPA : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nom READ getNom NOTIFY peripheriqueTTPAChanged)
    Q_PROPERTY(QString adresseMAC READ getAdresseMAC NOTIFY peripheriqueTTPAChanged)
    Q_PROPERTY(bool etatConnexion MEMBER etatConnexion NOTIFY connecte)
    Q_PROPERTY(bool etatConnexion MEMBER etatConnexion NOTIFY deconnecte)
    Q_PROPERTY(bool connexionErreur MEMBER connexionErreur NOTIFY erreur)

public:
    PeripheriqueTTPA(QString nom, QString adresseMAC, QObject *parent = nullptr);
    QString getNom() const;
    QString getAdresseMAC() const;

    /**
     * \fn connecter()
     * \brief Connecte le terminal mobile à l'appareil "TTPA"
     */
    Q_INVOKABLE void connecter();

    /**
     * \fn deconnecter()
     *
     * \brief Deconnecte l'appareil "TTPA" connecté
     *
     */
    Q_INVOKABLE void deconnecter();

    /**
     * \fn envoyer(QString trame)
     *
     * \brief Envoie une trame à l'appareil "TTPA" connecté
     *
     * \param trame QString Trame à envoyer à l'appareil
     *
     */
    Q_INVOKABLE bool envoyer(QString trame);

    /**
     * \fn bool estConnecte()
     *
     * \brief Retourne vrai si l'appareil est connecté
     *
     * \return bool L'etat de connexion de l'appareil
     */
    Q_INVOKABLE bool estConnecte();

private:
    QString nom; //!< nom du périphérique Bluetooth
    QString adresseMAC; //!<
    bool etatConnexion; //!<
    bool connexionErreur; //!<
    QBluetoothSocket *socket; //!<

public slots:
    // pour la socket
    void socketConnecte(); //!< slot exécuté quand la socket de communication est connecté
    void socketDeconnecte(); //!<
    void socketReadyRead(); //!<
    void socketErreur(QBluetoothSocket::SocketError erreur); //!<

signals:
    void peripheriqueTTPAChanged(); //!<
    void connecte(); //!<
    void deconnecte();
    void erreur(); //!<
    void trameRecue(QString nomPeripherique, QString trame);  //!< signal indiquant qu'une trame a été recçue
};

#endif // PeripheriqueTTPA_H
