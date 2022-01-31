/**
  *
  * \file terminal-TTPA/CommunicationBluetooth.cpp
  *
  * \brief Définition de la classe CommunicationBluetooth
  *
  * \author Hammouma Youssef
  *
  * \version 0.1
  *
  */
#include "CommunicationBluetooth.h"
#include "peripheriqueTTPA.h"
#include "peripheriqueTTPA.h"
#include <QBluetoothDeviceInfo>
#include <QDebug>
#include <QtEndian>
#include <unistd.h>

CommunicationBluetooth::CommunicationBluetooth() : nomPeripheriqueBluetoothLocal(""), adressePeripheriqueBluetoothLocal(""), discoveryAgentDevice(NULL), etatRecherche(false), peripheriqueRobotTTPADetecte(false),peripheriqueRobotTTPAConnecte(false),peripheriqueRobotTTPADeconnecte(true),peripheriqueEcranTTPADetecte(false),peripheriqueEcranTTPAConnecte(false),peripheriqueEcranTTPADeconnecte(true),peripheriqueTableTTPADetecte(false),peripheriqueTableTTPAConnecte(false),peripheriqueTableTTPADeconnecte(true),erreurRobotDetecte(false),seanceTerminee(false),numeroTable(NUMERO_TABLE)
{
    if (!peripheriqueBluetoothLocal.isValid())
    {
        qDebug() << Q_FUNC_INFO << "Bluetooth désactivé !";
        //qCritical("Bluetooth désactivé !");
        return;
    }

    // Le Bluetooth de la tablette
    peripheriqueBluetoothLocal.powerOn();
    nomPeripheriqueBluetoothLocal = peripheriqueBluetoothLocal.name();
    adressePeripheriqueBluetoothLocal = peripheriqueBluetoothLocal.address().toString();
    qDebug() << Q_FUNC_INFO << nomPeripheriqueBluetoothLocal << adressePeripheriqueBluetoothLocal;

    // Recherche
    //peripheriqueBluetoothLocal.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    discoveryAgentDevice = new QBluetoothDeviceDiscoveryAgent(this);

    // Slot pour la recherche de périphériques Bluetooth TTPA
    connect(discoveryAgentDevice, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this,
SLOT(ajouterPeripheriqueTTPA(QBluetoothDeviceInfo)));
    connect(discoveryAgentDevice, SIGNAL(canceled()), this, SLOT(rechercheTerminee()));
    connect(discoveryAgentDevice, SIGNAL(finished()), this, SLOT(rechercheTerminee()));
    connect(discoveryAgentDevice, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SLOT(rechercheErreur(QBluetoothDeviceDiscoveryAgent::Error)));

    etatSeance = EtatDeLaSeance::Initial;
}

CommunicationBluetooth::~CommunicationBluetooth()
{
    deconnecterPeripheriquesTTPA();
    qDeleteAll(peripheriquesTTPA);
    qDebug() << Q_FUNC_INFO;
}

void CommunicationBluetooth::rechercherPeripheriquesTTPA()
{
    if(!estValide())
        return;
    qDebug() << Q_FUNC_INFO;

    qDeleteAll(peripheriquesTTPA);
    peripheriquesTTPA.clear();
    peripheriqueTableTTPADetecte = false;
    peripheriqueRobotTTPADetecte = false;
    peripheriqueEcranTTPADetecte = false;
    emit detecte();
    emit peripheriquesTTPAUpdated();    

    if(discoveryAgentDevice != NULL)
    {
        discoveryAgentDevice->start();
        if (discoveryAgentDevice->isActive())
        {
            etatRecherche = true;
            emit recherche();
        }
    }
}

void CommunicationBluetooth::arreterRecherche()
{
    if(!estValide())
        return;
    qDebug() << Q_FUNC_INFO;
    if (!etatRecherche)
        return;
    if(discoveryAgentDevice != NULL && discoveryAgentDevice->isActive())
    {
        discoveryAgentDevice->stop();
    }
}

bool CommunicationBluetooth::estValide()
{
    return peripheriqueBluetoothLocal.isValid();
}

QString CommunicationBluetooth::getNomPeripheriqueBluetoothLocal()
{
    return nomPeripheriqueBluetoothLocal;
}

QString CommunicationBluetooth::getAdressePeripheriqueBluetoothLocal()
{
    return adressePeripheriqueBluetoothLocal;
}

QVariant CommunicationBluetooth::getPeripheriquesTTPA()
{
    return QVariant::fromValue(peripheriquesTTPA);
}

PeripheriqueTTPA* CommunicationBluetooth::getPeripheriqueTTPA(QString nom)
{
    bool trouve = false;
    PeripheriqueTTPA *peripheriqueTTPA = nullptr;

    // liste les appareils TTPA détectés
    for(int i=0; i < peripheriquesTTPA.size() && !trouve; i++)
    {
        if(((PeripheriqueTTPA *)peripheriquesTTPA[i])->getNom() == nom)
        {
            peripheriqueTTPA = (PeripheriqueTTPA *)peripheriquesTTPA[i];
            qDebug() << Q_FUNC_INFO << "nom" << peripheriqueTTPA->getNom();
            trouve = true;
        }
    }
    return peripheriqueTTPA;
}


bool CommunicationBluetooth::envoyerPeripheriqueTTPA(QString nom, QString message)
{
    // liste les appareils TTPA détectés
    for(int i=0; i < peripheriquesTTPA.size(); i++)
    {
        if(((PeripheriqueTTPA *)peripheriquesTTPA[i])->getNom() == nom)
        {
            PeripheriqueTTPA *peripheriqueTTPA = (PeripheriqueTTPA *)peripheriquesTTPA[i];
            qDebug() << Q_FUNC_INFO << peripheriqueTTPA->getNom() << "message" << message;
            return peripheriqueTTPA->envoyer(nom + message);
        }
    }
    return false;
}

bool CommunicationBluetooth::demarrerSeance(QString nomJoueur, QString zoneObjectif, QString zoneRobot, int frequenceBalle, int nbBalles, int effet, int puissance, bool rotation)
{
    if(etatSeance == EtatDeLaSeance::Initial)
    {
        etatSeance = EtatDeLaSeance::EnCours;
        demarrerSeanceTable(nbBalles);
        demarrerSeanceRobot(frequenceBalle, nbBalles, effet, puissance, rotation);
        demarrerSeanceEcran(nomJoueur, zoneObjectif, zoneRobot);
        return true;
    }
    return false;
}

bool CommunicationBluetooth::demarrerSeanceRobot(int frequenceBalle, int nbBalles, int effet, int puissance, bool rotation)
{
    qDebug() << Q_FUNC_INFO << NOM_BLUETOOTH_ROBOT + QString::number(numeroTable) << frequenceBalle << nbBalles << effet << puissance << rotation;

    QString QfrequenceBalle = QString::number(frequenceBalle);
    QString QnbBalles = QString::number(nbBalles);
    QString Qeffet = QString::number(effet);
    QString Qpuissance = QString::number(puissance);
    QString Qrotation = QString::number(rotation);

    QString trameReset = ":RESET*00\r\n";
    #ifdef SIMULATEURS
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), trameReset);
    #else
    //envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), trameReset);
    #endif

    QString trameDemarrageRobot = ":START;" + QfrequenceBalle + ";" + QnbBalles + ";" + Qeffet + ";"+ Qpuissance + ";" + Qrotation + "*00\r\n";
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), trameDemarrageRobot);

    return true;
}

bool CommunicationBluetooth::demarrerSeanceEcran(QString nomJoueur, QString zoneObjectif, QString zoneRobot)
{
    qDebug() << Q_FUNC_INFO << NOM_BLUETOOTH_ECRAN + QString::number(numeroTable) << nomJoueur << zoneObjectif << zoneRobot;

    QString trameReset = ":RESET*00\r\n";
    QString trameDemarrageEcran = ":START;" + nomJoueur + ";" + zoneObjectif + ";" +zoneRobot + "*00\r\n";

    #ifdef SIMULATEURS
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), trameReset);
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), trameDemarrageEcran);
    #else
    //envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), trameReset);
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), trameDemarrageEcran);
    #endif

    return true;
}

bool CommunicationBluetooth::demarrerSeanceTable(int nbBalles)
{
    qDebug() << Q_FUNC_INFO << NOM_BLUETOOTH_TABLE + QString::number(numeroTable) << "nbBalles" << nbBalles;

    QString trameReset = ":RESET*00\r\n";

    #ifdef SIMULATEURS
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), trameReset);
    #else
    //envoyerPeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), trameReset);
    #endif

    QString QnbBalles = QString::number(nbBalles);
    QString trameDemarrageTable = ":START;"+ QnbBalles + "*00\r\n";
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), trameDemarrageTable);

    return true;
}

bool CommunicationBluetooth::mettreEnPauseSeance()
{
    QString tramePause = ":PAUSE*00\r\n";

    if(etatSeance == EtatDeLaSeance::EnCours)
    {
        etatSeance = EtatDeLaSeance::EnPause;
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), tramePause);
        #ifdef SIMULATEURS
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), tramePause);
        #else
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), tramePause);
        #endif
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), tramePause);
        qDebug() << Q_FUNC_INFO;
        return true;
    }
    return false;
}

bool CommunicationBluetooth::reprendreLaSeance()
{
    QString trameReprise = ":RESUME*00\r\n";

    if(etatSeance == EtatDeLaSeance::EnPause)
    {
        etatSeance = EtatDeLaSeance::EnCours;
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), trameReprise);
        #ifdef SIMULATEURS
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), trameReprise);
        #else
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), trameReprise);
        #endif
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), trameReprise);
        qDebug() << Q_FUNC_INFO;
        erreurRobotDetecte = false;
        emit erreurRobot();
        return true;
    }
    return false;
}

bool CommunicationBluetooth::arreterSeance()
{
    qDebug() << Q_FUNC_INFO;

    QString trameArret = ":RESET*00\r\n";

    etatSeance = EtatDeLaSeance::Initial;
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), trameArret);
    #ifdef SIMULATEURS
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), trameArret);
    #else
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), trameArret);
    #endif
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), trameArret);

    return true;
}

bool CommunicationBluetooth::finirSeance()
{
    QString trameFin = ":END*00\r\n";

    if(etatSeance == EtatDeLaSeance::EnCours)
    {
        etatSeance = EtatDeLaSeance::Initial;
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), trameFin);
        #ifdef SIMULATEURS
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), trameFin);
        #else
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), trameFin);
        #endif
        envoyerPeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), trameFin);
        qDebug() << Q_FUNC_INFO;
        seanceTerminee = true;
        emit seanceFinie();
        return true;
    }
    return false;
}

bool CommunicationBluetooth::envoyerZoneImpactee(QString zone)
{
    qDebug() << Q_FUNC_INFO;

    QString trameStatistique = ":STAT;" + zone + "*00\r\n";

    #ifdef SIMULATEURS
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), trameStatistique);
    #else
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), trameStatistique);
    #endif

    return true;
}

bool CommunicationBluetooth::envoyerErreurRobot()
{
    qDebug() << Q_FUNC_INFO;

    QString trameErreur = ":INFO;Bourrage du robot ou Absence de balles*00\r\n";

    #ifdef SIMULATEURS
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), trameErreur);
    #else
    envoyerPeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), trameErreur);
    #endif

    return true;
}

QString CommunicationBluetooth::getZoneObjectif(QString zoneObjectif)
{
    QString numeroZoneObjectif = zoneObjectif;
    QStringRef b(&numeroZoneObjectif, 5, 1);
    numeroZoneObjectif = b.toString();
    qDebug() << Q_FUNC_INFO << "Numero zone objectif : " << numeroZoneObjectif;

    return numeroZoneObjectif;
}

QString CommunicationBluetooth::getZoneRobot(QString zoneRobot)
{
    QString numeroZoneRobot = zoneRobot;
    QStringRef b(&numeroZoneRobot, 5, 1);
    numeroZoneRobot = b.toString();
    qDebug() << Q_FUNC_INFO << "Numero zone robot : " << numeroZoneRobot;

    return numeroZoneRobot;
}

bool CommunicationBluetooth::estPresent(QString nom)
{
    // liste les appareils TTPA déjà détectés
    for(int i=0; i < peripheriquesTTPA.size(); i++)
    {
        PeripheriqueTTPA *peripheriqueTTPA = (PeripheriqueTTPA *)peripheriquesTTPA[i];
        if(peripheriqueTTPA->getNom() == nom)
        {
            return true;
        }
    }
    return false;
}

void CommunicationBluetooth::creerPeripheriquesTTPA()
{
    qDebug() << Q_FUNC_INFO;

    // Si le périphérique n'est pas détecté, on le crée
    QString nom;

    #ifdef SIMULATEURS
    nom = NOM_BLUETOOTH_ECRAN + QString::number(2);
    if(!estPresent(nom))
    {
        PeripheriqueTTPA *ecranTTPA = new PeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(2), ADRESSE_BLUETOOTH_ECRAN, this);
        peripheriquesTTPA.append(ecranTTPA);
        peripheriqueEcranTTPADetecte = true;
        emit detecte();
    }
    #else
    nom = NOM_BLUETOOTH_ECRAN + QString::number(numeroTable);
    if(!estPresent(nom))
    {
        PeripheriqueTTPA *ecranTTPA = new PeripheriqueTTPA(NOM_BLUETOOTH_ECRAN + QString::number(numeroTable), ADRESSE_BLUETOOTH_ECRAN, this);
        peripheriquesTTPA.append(ecranTTPA);
        peripheriqueEcranTTPADetecte = true;
        emit detecte();
    }
    #endif

    nom = NOM_BLUETOOTH_ROBOT + QString::number(numeroTable);
    if(!estPresent(nom))
    {
        PeripheriqueTTPA *robotTTPA = new PeripheriqueTTPA(NOM_BLUETOOTH_ROBOT + QString::number(numeroTable), ADRESSE_BLUETOOTH_ROBOT, this);
        peripheriquesTTPA.append(robotTTPA);
        peripheriqueRobotTTPADetecte = true;
        emit detecte();
    }

    nom = NOM_BLUETOOTH_TABLE + QString::number(numeroTable);
    if(!estPresent(nom))
    {
        PeripheriqueTTPA *tableTTPA = new PeripheriqueTTPA(NOM_BLUETOOTH_TABLE + QString::number(numeroTable), ADRESSE_BLUETOOTH_TABLE, this);
        peripheriquesTTPA.append(tableTTPA);
        peripheriqueTableTTPADetecte = true;
        emit detecte();
    }
}

void CommunicationBluetooth::ajouterPeripheriqueTTPA(const QBluetoothDeviceInfo &info)
{
    qDebug() << Q_FUNC_INFO << info.name() << info.address().toString();

    if(estPresent(info.name()))
        return;

    if(info.name() == (NOM_BLUETOOTH_TABLE + QString::number(numeroTable)))
    {
        qDebug() << Q_FUNC_INFO << "périphérique Table TTPA découvert" << info.name();
        PeripheriqueTTPA *tableTTPA = new PeripheriqueTTPA(info.name(), info.address().toString(), this);
        peripheriquesTTPA.append(tableTTPA);
        peripheriqueTableTTPADetecte = true;
        emit detecte();
    }

    if(info.name() == (NOM_BLUETOOTH_ROBOT + QString::number(numeroTable)))
    {
        qDebug() << Q_FUNC_INFO << "périphérique Robot TTPA découvert" << info.name();
        PeripheriqueTTPA *robotTTPA = new PeripheriqueTTPA(info.name(), info.address().toString(), this);
        peripheriquesTTPA.append(robotTTPA);
        peripheriqueRobotTTPADetecte = true;
        emit detecte();
    }

    #ifdef SIMULATEURS
    if(info.name() == (NOM_BLUETOOTH_ECRAN + QString::number(2)))
    #else
    if(info.name() == (NOM_BLUETOOTH_ECRAN + QString::number(numeroTable)))
    #endif
    {
        qDebug() << Q_FUNC_INFO << "périphérique Ecran TTPA découvert" << info.name();
        PeripheriqueTTPA *ecranTTPA = new PeripheriqueTTPA(info.name(), info.address().toString(), this);
        peripheriquesTTPA.append(ecranTTPA);
        peripheriqueEcranTTPADetecte = true;
        emit detecte();
    }

    // 3 sur 3
    if(peripheriqueEcranTTPAConnecte && peripheriqueRobotTTPAConnecte && peripheriqueTableTTPAConnecte)
    {
        arreterRecherche();
    }
    // 2 sur 3
    /*if(peripheriqueRobotTTPADetecte && peripheriqueTableTTPADetecte)
    {
        arreterRecherche();
    }*/
    // 1 sur 3
    /*if(peripheriqueRobotTTPADetecte)
        arreterRecherche();*/
}

void CommunicationBluetooth::rechercheTerminee()
{
    creerPeripheriquesTTPA();
    qDebug() << Q_FUNC_INFO;    
    etatRecherche = false;
    emit recherche();
    emit detecte();
    connecterPeripheriquesTTPA();
    emit peripheriquesTTPAUpdated();
}

void CommunicationBluetooth::rechercheErreur(QBluetoothDeviceDiscoveryAgent::Error erreur)
{
    qDebug() << Q_FUNC_INFO << "erreur" << erreur;
}

void CommunicationBluetooth::recevoirTramePeripheriqueTTPA(QString nomPeripherique, QString trame)
{
    qDebug() << Q_FUNC_INFO << "nomPeripherique" << nomPeripherique << "trame" << trame;

    if(etatSeance != EtatDeLaSeance::EnCours)
        return;

        if(trame.contains("IMPACT") && trame.contains(";"))
        {
            //$table-ttpa-1:IMPACT;4*XX\r\n
            QStringRef traitementImpactTable(&trame,trame.indexOf(";")+1, 1);
            QString donneesImpact = traitementImpactTable.toString();
            int zoneImpact = donneesImpact.toInt();
            #ifndef SIMULATEURS
            if(zoneImpact >= 1 && zoneImpact <= 9)
            {
                donneesImpact = QString::number(zoneImpact-1);
                qDebug() << Q_FUNC_INFO << "donneesImpact" << donneesImpact;
                envoyerZoneImpactee(donneesImpact);
            }
            #else
            if(zoneImpact >= 0 && zoneImpact <= 7)
            {
                if(zoneImpact == 0)
                    zoneImpact = 8; // Hors table
                else
                    zoneImpact -= 1; // décalage
                donneesImpact = QString::number(zoneImpact);
                qDebug() << Q_FUNC_INFO << "donneesImpact" << donneesImpact;
                envoyerZoneImpactee(donneesImpact);
            }
            #endif
        }
        else if(trame.contains("ERROR")) // ROBOT
        {
            //$robot-ttpa-1:ERROR;0;1;0*XX\r\n
            //             ^
            //             |----->
            //             |-------->
            //             |---------->
            int pos = trame.indexOf(":");
            QStringRef traitementBourrageRobot(&trame, pos+7, 1);
            QStringRef traitementAbsenceDeBalleRobot(&trame, pos+9, 1);
            QStringRef traitementErreurParametresRobot(&trame, pos+11, 1);
            QString bourrageDeBalle = traitementBourrageRobot.toString();
            QString absenceDeBalle = traitementAbsenceDeBalleRobot.toString();
            QString erreurParametres = traitementErreurParametresRobot.toString();
            qDebug() << Q_FUNC_INFO << "bourrageBalle" << bourrageDeBalle << "absenceBalle" << absenceDeBalle <<        "erreurParametre" << erreurParametres;

            // 1. envoyer trame à l'écran
            envoyerErreurRobot();
            // 2. signaler à l'IHM
            erreurRobotDetecte = true;
            emit erreurRobot();
            // A voir : passage à l'état ERREUR ou PAUSE ?
            mettreEnPauseSeance();
        }
        else if(trame.contains("FINISH"))
        {
            finirSeance();
        }
}

void CommunicationBluetooth::recevoirConnecte()
{
    PeripheriqueTTPA *peripheriqueTTPA = qobject_cast<PeripheriqueTTPA*>(sender());
    qDebug() << Q_FUNC_INFO << peripheriqueTTPA->getNom() << "connecte" << peripheriqueTTPA->estConnecte();

    if(peripheriqueTTPA->getNom().contains("table-ttpa"))
    {
        if(peripheriqueTTPA->estConnecte())
        {
            peripheriqueTableTTPAConnecte = true;
            emit connecte();
        }
    }
    else if(peripheriqueTTPA->getNom().contains("robot-ttpa"))
    {
        if(peripheriqueTTPA->estConnecte())
        {
            peripheriqueRobotTTPAConnecte = true;
            emit connecte();
        }
    }
    else if(peripheriqueTTPA->getNom().contains("ecran-ttpa"))
    {
        if(peripheriqueTTPA->estConnecte())
        {
            peripheriqueEcranTTPAConnecte = true;
            emit connecte();
        }
    }
}

void CommunicationBluetooth::connecterPeripheriquesTTPA()
{
    qDebug() << Q_FUNC_INFO << "Nb peripheriquesTTPA" << peripheriquesTTPA.size();

    // liste les appareils TTPA détectés
    for(int i=0; i < peripheriquesTTPA.size(); i++)
    {
        PeripheriqueTTPA *peripheriqueTTPA = (PeripheriqueTTPA *)peripheriquesTTPA[i];
        if(!peripheriqueTTPA->estConnecte())
        {
            qDebug() << Q_FUNC_INFO << peripheriqueTTPA << "connexion" << peripheriqueTTPA->getNom();            
            connect(peripheriqueTTPA, SIGNAL(trameRecue(QString,QString)), this, SLOT(recevoirTramePeripheriqueTTPA(QString,QString)));
            connect(peripheriqueTTPA, SIGNAL(connecte()), this, SLOT(recevoirConnecte()));
            connect(peripheriqueTTPA, SIGNAL(deconnecte()), this, SLOT(recevoirDeconnecte()));
            peripheriqueTTPA->connecter();
        }
        else
        {
            qDebug() << Q_FUNC_INFO << peripheriqueTTPA << "deconnexion" << peripheriqueTTPA->getNom();
            disconnect(peripheriqueTTPA, SIGNAL(trameRecue(QString,QString)), this, SLOT(recevoirTramePeripheriqueTTPA(QString,QString)));
            peripheriqueTTPA->deconnecter();
            sleep(1);
            disconnect(peripheriqueTTPA, SIGNAL(connecte()), this, SLOT(recevoirConnecte()));
            disconnect(peripheriqueTTPA, SIGNAL(deconnecte()), this, SLOT(recevoirDeconnecte()));
        }
    }
}

void CommunicationBluetooth::recevoirDeconnecte()
{
    PeripheriqueTTPA *peripheriqueTTPA = qobject_cast<PeripheriqueTTPA*>(sender());
    qDebug() << Q_FUNC_INFO << peripheriqueTTPA->getNom() << "deconnecte" << (!peripheriqueTTPA->estConnecte());

    if(peripheriqueTTPA->getNom().contains("table-ttpa"))
    {
        peripheriqueTableTTPADeconnecte = true;
        emit deconnecte();
    }
    else if(peripheriqueTTPA->getNom().contains("robot-ttpa"))
    {
        peripheriqueRobotTTPADeconnecte = true;
        emit deconnecte();
    }
    else if(peripheriqueTTPA->getNom().contains("ecran-ttpa"))
    {
        peripheriqueEcranTTPADeconnecte = true;
        emit deconnecte();
    }
}

void CommunicationBluetooth::deconnecterPeripheriquesTTPA()
{
    // liste les appareils TTPA détectés
    for(int i=0; i < peripheriquesTTPA.size(); i++)
    {
        PeripheriqueTTPA *peripheriqueTTPA = (PeripheriqueTTPA *)peripheriquesTTPA[i];
        qDebug() << Q_FUNC_INFO << "déconnecter" << peripheriqueTTPA->getNom();
        disconnect(peripheriqueTTPA, SIGNAL(trameRecue(QString,QString)), this, SLOT(recevoirTramePeripheriqueTTPA(QString,QString)));
        peripheriqueTTPA->deconnecter();
        sleep(1);
        disconnect(peripheriqueTTPA, SIGNAL(connecte()), this, SLOT(recevoirConnecte()));
        disconnect(peripheriqueTTPA, SIGNAL(deconnecte()), this, SLOT(recevoirDeconnecte()));
    }
}

