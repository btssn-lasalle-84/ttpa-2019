/**
  *
  * \file terminal-TTPA/CommunicationBluetooth.h
  *
  * \brief Déclaration de la classe CommunicationBluetooth
  *
  * \author Hammouma Youssef
  *
  * \version 0.1
  *
  */
#ifndef CommunicationBluetooth_H
#define CommunicationBluetooth_H

#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>
#include <QBluetoothUuid>
#include <QBluetoothDeviceInfo>
#include <QBluetoothServiceInfo>
#include <QBluetoothDeviceDiscoveryAgent>

#define SERVICE_UUID      "{00001101-0000-1000-8000-00805F9B34FB}"

#define NOM_BLUETOOTH_ROBOT "robot-ttpa-"
#define NOM_BLUETOOTH_TABLE "table-ttpa-"
#define NOM_BLUETOOTH_ECRAN "ecran-ttpa-"

// Tests :
#define SIMULATEURS

#ifndef SIMULATEURS
// TTPA 2
#define NUMERO_TABLE    2
#define ADRESSE_BLUETOOTH_ROBOT "3C:71:BF:6A:69:DA"
#define ADRESSE_BLUETOOTH_TABLE "98:D3:31:F5:33:C5"
#define ADRESSE_BLUETOOTH_ECRAN "B8:27:EB:13:DA:A5"
#else
// TTPA 11
#define NUMERO_TABLE    11
#define ADRESSE_BLUETOOTH_ROBOT "84:0D:8E:37:77:8A"
#define ADRESSE_BLUETOOTH_TABLE "80:7D:3A:A3:2E:DA"
#define ADRESSE_BLUETOOTH_ECRAN "B8:27:EB:13:DA:A5"
#endif


class PeripheriqueTTPA;

/**
 * \class CommunicationBluetooth
 *
 * \brief Assure la détection des périphériques TTPA, leur connexion (et déconnexion) et une communication (envoyer et recevoir des trames)
 *
 * \author Hammouma Youssef
 *
 * \version 0.1
 *
 * \date 20/03/2019 10:30:20
 *
 */
class CommunicationBluetooth : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nomPeripheriqueBluetoothLocal MEMBER nomPeripheriqueBluetoothLocal NOTIFY detecte)
    Q_PROPERTY(QString adressePeripheriqueBluetoothLocal MEMBER adressePeripheriqueBluetoothLocal NOTIFY detecte)

    Q_PROPERTY(bool peripheriqueEcranTTPADetecte MEMBER peripheriqueEcranTTPADetecte NOTIFY detecte)
    Q_PROPERTY(bool peripheriqueEcranTTPAConnecte MEMBER peripheriqueEcranTTPAConnecte NOTIFY connecte)
    Q_PROPERTY(bool peripheriqueEcranTTPADeconnecte MEMBER peripheriqueEcranTTPADeconnecte NOTIFY deconnecte)

    Q_PROPERTY(bool peripheriqueTableTTPADetecte MEMBER peripheriqueTableTTPADetecte NOTIFY detecte)
    Q_PROPERTY(bool peripheriqueTableTTPAConnecte MEMBER peripheriqueTableTTPAConnecte NOTIFY connecte)
    Q_PROPERTY(bool peripheriqueTableTTPADeconnecte MEMBER peripheriqueTableTTPADeconnecte NOTIFY deconnecte)

    Q_PROPERTY(bool peripheriqueRobotTTPADetecte MEMBER peripheriqueRobotTTPADetecte NOTIFY detecte)
    Q_PROPERTY(bool peripheriqueRobotTTPAConnecte MEMBER peripheriqueRobotTTPAConnecte NOTIFY connecte)
    Q_PROPERTY(bool peripheriqueRobotTTPADeconnecte MEMBER peripheriqueRobotTTPADeconnecte NOTIFY deconnecte)

    Q_PROPERTY(bool etatRecherche MEMBER etatRecherche NOTIFY recherche)
    Q_PROPERTY(QVariant peripheriquesTTPA READ getPeripheriquesTTPA NOTIFY peripheriquesTTPAUpdated)
    Q_PROPERTY(bool erreurRobotDetecte MEMBER erreurRobotDetecte NOTIFY erreurRobot)
    Q_PROPERTY(bool seanceTerminee MEMBER seanceTerminee NOTIFY seanceFinie)
    Q_PROPERTY(int numeroTable MEMBER numeroTable NOTIFY numeroTableUpdated)

public:
    CommunicationBluetooth();
    ~CommunicationBluetooth();

    enum EtatDeLaSeance
    {
        Initial = 0,
        EnCours = 1,
        EnPause = 2,
        Arretee
    };

    EtatDeLaSeance getEtatSeance();

    /**
     * \fn rechercherPeripheriquesTTPA()
     *
     * \brief Assure la recherche d'appareils bluetooth "TTPA"
     *
     */
    Q_INVOKABLE void rechercherPeripheriquesTTPA();

    /**
     * \fn creerPeripheriquesTTPA()
     *
     * \brief Pour les tests avec des péréphériques connus
     *
     */
    Q_INVOKABLE void creerPeripheriquesTTPA();

    /**
     * \fn void arreterRecherche();
     *
     * \brief Permet d'arreter la recherche des appareils bluetooth "TTPA"
     *
     */
    Q_INVOKABLE void arreterRecherche();

    /**
     * \fn bool estValide()
     *
     * \brief Retourne vrai si l'appareil correspond à un appareil "TTPA"
     *
     * \return Vrai si l'appareil est conforme aux normes "TTPA"
     */
    Q_INVOKABLE bool estValide();

    /**
     * \fn void connecterPeripheriquesTTPA()
     *
     * \brief Connecte individuelle chaque appareil TTPA détecté
     */
    Q_INVOKABLE void connecterPeripheriquesTTPA();

    /**
     * \fn void deconnecterPeripheriquesTTPA()
     *
     * \brief Déconnecte individuelle chaque appareil TTPA détecté
     */
    Q_INVOKABLE void deconnecterPeripheriquesTTPA();

    /**
     * \fn bool envoyerPeripheriqueTTPA()
     *
     * \brief Permet d'envoyer une trame à un périphérique TTPA identifié par son nom
     * \param nom QString nom du périphérique Bluetooth
     * \param message QString trame respectant le protocole TTPA
     * \return bool vrai si la trame a pu être envoyé au périphérique TTPA
     */
    Q_INVOKABLE bool envoyerPeripheriqueTTPA(QString nom, QString message);


    Q_INVOKABLE bool demarrerSeance(QString nomJoueur, QString zoneObjectif, QString zoneRobot, int frequenceBalle, int nbBalles, int effet, int puissance, bool rotation);

    /**
     * \fn bool demarrerSeanceRobot
     *
     * \brief Envoi une trame contenant les paramètres du robot au robot-ttpa
     * \param frequenceBalle int nom du périphérique Bluetooth
     * \param nbBalles int trame respectant le protocole TTPA
     * \param effet int trame respectant le protocole TTPA
     * \param puissance int trame respectant le protocole TTPA
     * \param rotation bool trame respectant le protocole TTPA
     * \return bool vrai si la trame a pu être envoyé au périphérique TTPA
     */
    Q_INVOKABLE bool demarrerSeanceRobot(int frequenceBalle, int nbBalles, int effet, int puissance, bool rotation);

    /**
     * \fn bool demarrerSeanceEcran()
     *
     * \brief Envoi une trame de démarrage de séance à l'ecran
     * \param nomJoueur QString nom du joueur
     * \param zoneObjectif QString trame respectant le protocole TTPA
     * \param zoneRobot QString trame respectant le protocole TTPA
     * \return bool vrai si la trame a pu être envoyé au périphérique TTPA
     */
    Q_INVOKABLE bool demarrerSeanceEcran(QString nomJoueur, QString zoneObjectif, QString zoneRobot);

    /**
     * \fn bool demarrerSeanceTable()
     *
     * \brief Envoi une trame de démarrage de séance à la table
     * \param nbBalles int Nombre de balles
     * \return bool vrai si la trame a pu être envoyé au périphérique TTPA
     */
    Q_INVOKABLE bool demarrerSeanceTable(int nbBalles);

    /**
     * \fn bool mettreEnPauseSeance()
     *
     * \brief Met la séance en pause
     * \return bool vrai si la trame a pu être envoyé au périphérique TTPA
     */
    Q_INVOKABLE bool mettreEnPauseSeance();

    /**
     * \fn bool reprendreLaSeance()
     *
     * \brief Reprend la séance
     * \return bool vrai si la trame a pu être envoyé au périphérique TTPA
     */
    Q_INVOKABLE bool reprendreLaSeance();

    /**
     * \fn bool arreterSeance()
     *
     * \brief Met subitement fin à la séance peut importe son état
     * \return bool vrai si la trame a pu être envoyé au périphérique TTPA
     */
    Q_INVOKABLE bool arreterSeance();

    /**
     *\fn bool finirSeance()
     * @brief Met fin à la séance lorsque le robot indique qu'il à tirer toutes ses balles
     * @return bool vrai si la trame a pu être envoyée au périphérique TTPA
     */
    bool finirSeance();

    /**
     *\fn bool envoyerZoneImpactee()
     * @brief Dès que la table envoie une trame contenant la zone impactée, la méthode l'envoie à l'écran ttpa
     * @param zone QString zone impactée
     * @return bool vrai si la trame a pu être envoyée au périphérique TTPA
     */
    bool envoyerZoneImpactee(QString zone);

    /**
     *\fn bool envoyerErreurRobot()
     * @brief Si le robot détecte un bourrage de balle ou une absence de balle, une trame est envoyée à l'écran ttpa
     * @return bool vrai si la trame a pu être envoyée au périphérique TTPA
     */
    bool envoyerErreurRobot();

    /**
     * \fn QString getZoneObjectif()
     *
     * \brief Permet d'obtenir la zone objectif
     * \param zoneObjectif QString numéro de la zone objectif choisie par le joueur
     * \return QString numéro de la zone objectif
     */
    Q_INVOKABLE QString getZoneObjectif(QString zoneObjectif);

    /**
     * \fn QString getZoneRobot()
     *
     * \brief Permet d'obtenir la zone robot
     * \param zoneRobot QString numéro de la zone robot choisie par le joueur
     * \return QString numéro de la zone robot
     */
    Q_INVOKABLE QString getZoneRobot(QString zoneRobot);

    Q_INVOKABLE PeripheriqueTTPA* getPeripheriqueTTPA(QString nom);

    QString getNomPeripheriqueBluetoothLocal();
    QString getAdressePeripheriqueBluetoothLocal();
    QVariant getPeripheriquesTTPA();

private:
    QBluetoothLocalDevice peripheriqueBluetoothLocal; //!< le périphérique Bluetooth de la tablette
    QString nomPeripheriqueBluetoothLocal; //!< le nom du périphérique Bluetooth de la tablette
    QString adressePeripheriqueBluetoothLocal; //!< l'adresse MAC du périphérique Bluetooth de la tablette
    QList<QObject*> peripheriquesTTPA; //!< liste des périphériques Bluetooth TTPA détectés au maximum 3 (Robot, Table et Ecran)
    QBluetoothDeviceDiscoveryAgent *discoveryAgentDevice; //!< l'agent permettant de découvrir des périphériques Bluetooth    
    bool etatRecherche; //!<
    bool peripheriqueRobotTTPADetecte; //!<
    bool peripheriqueRobotTTPAConnecte; //!<
    bool peripheriqueRobotTTPADeconnecte; //!<

    bool peripheriqueEcranTTPADetecte; //!<
    bool peripheriqueEcranTTPAConnecte; //!<
    bool peripheriqueEcranTTPADeconnecte; //!<

    bool peripheriqueTableTTPADetecte; //!<
    bool peripheriqueTableTTPAConnecte; //!<
    bool peripheriqueTableTTPADeconnecte; //!<
    bool erreurRobotDetecte; //!<
    bool seanceTerminee;
    EtatDeLaSeance etatSeance;
    int numeroTable;

    bool estPresent(QString nom);

public slots:
    void ajouterPeripheriqueTTPA(const QBluetoothDeviceInfo&);
    void rechercheTerminee();
    void rechercheErreur(QBluetoothDeviceDiscoveryAgent::Error erreur);
    void recevoirTramePeripheriqueTTPA(QString nomPeripherique, QString trame);
    void recevoirConnecte();
    void recevoirDeconnecte();

signals:
    void recherche(); //!< signal indiquant que l'état de la recherche de périphériques a changé
    void detecte();
    void connecte();
    void deconnecte();
    void robotDetecte();
    void ecranDetecte();
    void tableDetecte();
    void peripheriquesTTPAUpdated();
    void erreurRobot();
    void seanceFinie();
    void numeroTableUpdated();
};

#endif // CommunicationBluetooth_H
