#include "ttpaihm.h"
#include "ui_ttpaihm.h"
#include "statistiquesseance.h"
#include "communicationbluetooth.h"
#include <QApplication>
#include <QDebug>
#include <sys/cdefs.h>
#include <QColor>
#include <QFrame>


/**
  *
  * @file ecran-TTPA/ttpaihm.cpp
  *
  * @brief Définition de la classe TtpaIhm
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */

/**
 * @brief Constructeur de la fenêtre principale
 *
 * @fn TtpaIhm::TtpaIhm
 *
 * @param parent QObject Adresse de l'objet Qt parent (ici 0 car c'est la fenêtre principale)
 */
TtpaIhm::TtpaIhm(QWidget *parent) : QWidget(parent), ui(new Ui::TtpaIhm), timerHorloge(nullptr), compteurDureeSeance(0), zonesTable(StatistiquesSeance::NbZonesTotales, nullptr)
{
    qDebug() << Q_FUNC_INFO;
    ui->setupUi(this);
/**
 * @brief ajout d'un bouton dans chaque écran peut être désactiver avec le .pro
 */
#ifndef QT_NO_DEBUG_OUTPUT
    // Pour simuler un impact
    editNumeroZone = new QLineEdit(this);
    editNumeroZone->setFocus();
    editNumeroZone->setFixedSize(0,0);
    ui->hLayoutVersEcranResultats->addWidget(editNumeroZone);
#endif
    // Choix de l'écran de départ
    ui->ecranTtpa->setCurrentIndex(TtpaIhm::PageAttente);
    //ui->ecranTtpa->setCurrentIndex(TtpaIhm::PageSeance);
    //ui->ecranTtpa->setCurrentIndex(TtpaIhm::PageResultats);

    //Plein écran
    //const int largeur = qApp->desktop()->availableGeometry(this).width();
    //const int hauteur = qApp->desktop()->availableGeometry(this).height();
    //resize(largeur, hauteur);
    //showMaximized();
    showFullScreen();

    raccourcisClavier();

    //Timer
    timerHorloge = new QTimer(this); // Instancie dynamiquement le temporisateur du rafraichissement de l'heure
    connect(timerHorloge, SIGNAL(timeout()),this,SLOT(afficherHeure())); // Pour le déclenchement périodique de l'affichage de l'heure    
    timerHorloge->start(PERIODE_HORLOGE); // Toutes les secondes (1000 ms)

#ifndef QT_NO_DEBUG_OUTPUT
    QAction *actionAllerDroite = new QAction(this);
    actionAllerDroite->setShortcut(QKeySequence(Qt::Key_F3));
    addAction(actionAllerDroite);
    connect(actionAllerDroite, SIGNAL(triggered()), this, SLOT(allerSuivant()));// Pour passer à l'écran suivant
    QAction *actionAllerGauche = new QAction(this);
    actionAllerGauche->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F3));
    addAction(actionAllerGauche);
    connect(actionAllerGauche, SIGNAL(triggered()), this, SLOT(revenirPrecedent()));// Pour revenir à l'écran précédent
#endif

    statistiquesSeance = new StatistiquesSeance(this);
    connect(statistiquesSeance, SIGNAL(affichageZone(int, int, QString)), this, SLOT(afficherZone(int, int, QString)));
    connect(statistiquesSeance, SIGNAL(ballesTotales(int, int)), this, SLOT(afficherBallesTotalEcranSeance(int, int)));
    connect(statistiquesSeance, SIGNAL(ballesTotalesHorsTable(int)),this ,SLOT(afficherBallesTotalHorsTable(int)));
    connect(statistiquesSeance, SIGNAL(ballesBonnes(int)), this, SLOT(afficherBallesBonnes(int)));

    statistiquesSeance->initialiserStatistiques();
#ifdef BLUETOOTH
    communicationBluetooth = new CommunicationBluetooth(this);
    communicationBluetooth->demarrer();
    connect(communicationBluetooth, SIGNAL(impactZone(QString)), this, SLOT(recevoirNumeroZone(QString)));
    connect(communicationBluetooth, SIGNAL(departSeance(QString,QString,QString)), this, SLOT(commencerSeance(QString,QString,QString)));// Pour commencer la seance
    connect(communicationBluetooth, SIGNAL(mettreEnPauseSeance()), this, SLOT(mettreEnPauseSeance()));
    connect(communicationBluetooth, SIGNAL(repriseSeance()), this, SLOT(reprendreSeance()));
    connect(communicationBluetooth, SIGNAL(finSeance()), this, SLOT(finirSeance()));
    connect(communicationBluetooth, SIGNAL(resetSeance()), this, SLOT(resetSeance()));
    connect(communicationBluetooth, SIGNAL(erreurRobot(QString)), this, SLOT(erreurRobot(QString)));
    connect(communicationBluetooth, SIGNAL(connecte()), this, SLOT(afficherConnexion()));
    //connect(communicationBluetooth, SIGNAL(deconnecte()), this, SLOT((deconnecterJoueur())));
#else
    communicationBluetooth = nullptr;
#endif

    // slot pour les boutons dans chaque écran peut être désactiver avec le .pro ou rajouter le ifndef
#ifndef QT_NO_DEBUG_OUTPUT
    connect(editNumeroZone, SIGNAL(textChanged(const QString)), this, SLOT(saisirNumeroZone(const QString)));
    //editNumeroZone->setFocus();
#endif
#ifdef SIMULATION
    // Simulation en attendant la communication avec le terminal mobile
    qsrand(QTime::currentTime().msec());
    connecterJoueur("Simon GAUZY");
    setZoneRobot(qrand()%int(StatistiquesSeance::NbZonesImpact));
    setZoneObjectif(qrand()%int(StatistiquesSeance::NbZonesImpact));
#endif
}

/**
 * @brief Destructeur de la fenêtre principale
 *
 * @fn TtpaIhm::~TtpaIhm
 *
 */
TtpaIhm::~TtpaIhm()
{
    // arrêt du timerSequence
    //emit arret();
    delete ui;
    qDebug() << Q_FUNC_INFO;
}

void TtpaIhm::commencerSeance(QString nomJoueur, QString zoneObjectif, QString zoneRobot)
{
    qDebug() << Q_FUNC_INFO;
    resetSeance();
    initialiserZone();
    connecterJoueur(nomJoueur);
    setZoneObjectif(zoneObjectif);
    setZoneRobot(zoneRobot);
    statistiquesSeance->afficherStatistiquesZones();
    statistiquesSeance->afficherStatistiquesBallesTotales();
    compteurDureeSeance = 0;
    connect(timerHorloge, SIGNAL(timeout()),this,SLOT(afficherDureeSeance())); // Pour le comptage et l'affichage de la durée d'une séance
    //QPixmap SystemeOn(":play-button.png" );
    //ui->labelSystemeOn->setPixmap(SystemeOn);
    ui->ecranTtpa->setCurrentIndex(TtpaIhm::PageSeance);
}

void TtpaIhm::mettreEnPauseSeance()
{
    qDebug() << Q_FUNC_INFO;
    disconnect(timerHorloge, SIGNAL(timeout()),this,SLOT(afficherDureeSeance())); // Pour le comptage et l'affichage de la durée d'une séance
    QPixmap systemeOff(":pause.png" );
    //ui->labelSystemeOff->setAlignment(Qt::AlignHCenter);
    //ui->labelSystemeOff->setAlignment(Qt::AlignRight);
    //SystemeOff = SystemeOff.scaled(QSize(150, 150),Qt::IgnoreAspectRatio);
    ui->labelSystemeOff->setPixmap(systemeOff);
    ui->labelSystemeOff->setVisible(true);
}

void TtpaIhm::reprendreSeance()
{
    qDebug() << Q_FUNC_INFO;
    connect(timerHorloge, SIGNAL(timeout()),this,SLOT(afficherDureeSeance())); // Pour le comptage et l'affichage de la durée d'une séance
    ui->labelSystemeOff->setVisible(false);
    ui->labelErreurRobot->setVisible(false);
}

void TtpaIhm::finirSeance()
{
    qDebug() << Q_FUNC_INFO;
    disconnect(timerHorloge, SIGNAL(timeout()),this,SLOT(afficherDureeSeance())); // Pour le comptage et l'affichage de la durée d'une séance
    ui->ecranTtpa->setCurrentIndex(TtpaIhm::PageResultats);
    afficherStatistiqueEcranResultats();
}
void TtpaIhm::resetSeance()
{
    deconnecterJoueur();
    disconnect(timerHorloge, SIGNAL(timeout()),this,SLOT(afficherDureeSeance())); // Pour le comptage et l'affichage de la durée d'une séance
    statistiquesSeance->initialiserStatistiques();
    ui->labelSystemeOff->setVisible(false);
    ui->labelErreurRobot->setVisible(false);
    ui->ecranTtpa->setCurrentIndex(TtpaIhm::PageAttente);
}

void TtpaIhm::erreurRobot(QString infoDuRobot)
{
    qDebug() << Q_FUNC_INFO;
    disconnect(timerHorloge, SIGNAL(timeout()),this,SLOT(afficherDureeSeance())); // Pour le comptage et l'affichage de la durée d'une séance
    QPixmap systemeOff(":pause.png" );
    InfoRobot(infoDuRobot);
    ui->labelErreurRobot->setVisible(true);
    ui->labelSystemeOff->setPixmap(systemeOff);
    ui->labelSystemeOff->setVisible(true);
    //ui->labelErreurRobot->setCurrentIndex(TtpaIhm::PageAttente);
}
/**
 * @brief Affiche l'heure périodiquement (slot)
 *
 * @fn TtpaIhm::afficherHeure
 *
 */
void TtpaIhm::afficherHeure()
{
    QString affichageHeure;
    QTime heure = QTime::currentTime();
    affichageHeure = "<span style=\"font-size:42pt; font-weight:600;\">" + heure.toString("hh : mm : ss") + "</span>";
    ui->labelHorlogeEcranSeance->setText(affichageHeure);
    ui->labelHorlogeEcranResultats->setText(affichageHeure);
    ui->labelHorlogeEcranSeance->setStyleSheet("color: rgb(250, 142, 7)");
    ui->labelHorlogeEcranResultats->setStyleSheet("color: rgb(250, 142, 7)");
    //ui->labelHorlogeEcranSeance->setText(heure.toString("hh : mm : ss"));
    //ui->labelHorlogeEcranResultats->setText(heure.toString("hh : mm : ss"));
}

void TtpaIhm::afficherConnexion()
{
    QString etat;
    etat = "<span style=\"font-size:32pt; font-weight:600;\">Connecté</span>";
    ui->labelEtatConnexion->setText(etat);
}
/**
 * @brief Actions nécéssaires a la deconnexion du joueur (slot)
 *
 * @fn TtpaIhm::deconnecterJoueur
 *
 */
void TtpaIhm::deconnecterJoueur()
{
    qDebug() << Q_FUNC_INFO;
    ui->labelEtatConnexion->setText("En attente de connexion");
    ui->labelNomJoueur->setText("");
    //resetSeance();
}

/**
 * @brief Actions nécéssaires a la connexion du joueur (slot)
 *
 * @fn TtpaIhm::connecterJoueur
 *
 */
void TtpaIhm::connecterJoueur(QString nomJoueur)
{
    QString nomDuJoueur;    
    nomDuJoueur = "<span style=\"font-size:32pt; font-weight:600;\">" + QString::fromUtf8("Joueur :  %1").arg(nomJoueur) + "</span>";
    ui->labelNomJoueur->setText(nomDuJoueur.toUtf8());
    ui->labelNomJoueur->setStyleSheet("color: rgb(250, 142, 7);");
    ui->labelNomJoueur_2->setText(nomDuJoueur.toUtf8());
    ui->labelNomJoueur_2->setStyleSheet("color: rgb(250, 142, 7);");
}

void TtpaIhm::InfoRobot(QString infoDuRobot)
{
    QString infoRobot;
    infoRobot = "<span style=\"font-size:18pt; font-weight:600;\">" + QString::fromUtf8("Info :  %1").arg(infoDuRobot) + "</span>";
    ui->labelErreurRobot->setText(infoRobot.toUtf8());
    ui->labelErreurRobot->setStyleSheet("color: rgb(250, 0, 0);");

}
/**
 * @brief Affiche la durée d'une Seance(slot)
 *
 * @fn TtpaIhm::afficherDureeSeance
 *
 */
void TtpaIhm::afficherDureeSeance()
{
    QString affichageDuree;
    QString affichageFinDuree;
    QTime duree(0, 0);
    compteurDureeSeance++;
    QTime dureeSeance = duree.addSecs(compteurDureeSeance);
    if(compteurDureeSeance >= 3600)
    {
        affichageDuree = "<span style=\"font-size:32pt; font-weight:600;\">" + dureeSeance.toString("hh : mm : ss") + "</span>";
        affichageFinDuree = "<span style=\"font-size:32pt; font-weight:600;\">" /*+ QString::fromUtf8("Durée de la séance")*/ + dureeSeance.toString("mm : ss") + "</span>";
        ui->labelDureeSeance->setText(affichageDuree);
        ui->labelDureeSeance->setStyleSheet("color: #FFFFFF;");
        ui->labelTimeFinSeance->setText(affichageFinDuree);
        ui->labelTimeFinSeance->setStyleSheet("color: #FFFFFF;");
    }
    else
    {
        affichageDuree = "<span style=\"font-size:32pt; font-weight:600;\">"  + dureeSeance.toString("mm : ss") + "</span>";
        affichageFinDuree = "<span style=\"font-size:32pt; font-weight:600;\">" /*+ QString::fromUtf8("Durée de la séance : ")*/ + dureeSeance.toString("mm : ss") + "</span>";
        ui->labelDureeSeance->setText(affichageDuree);
        ui->labelDureeSeance->setStyleSheet("color: #FFFFFF;");
        ui->labelTimeFinSeance->setText(affichageFinDuree);
        ui->labelTimeFinSeance->setStyleSheet("color: #FFFFFF;");
    }
}

/**
 * @brief Initialise le vecteur des zones de la table
 *
 * @fn TtpaIhm::initialiserZone
 *
*/
void TtpaIhm::initialiserZone()
{
    // Initialise le vecteur des zones de la table
    zonesTable[StatistiquesSeance::Zone1] = ui->labelZone1;
    zonesTable[StatistiquesSeance::Zone2] = ui->labelZone2;
    zonesTable[StatistiquesSeance::Zone3] = ui->labelZone3;
    zonesTable[StatistiquesSeance::Zone4] = ui->labelZone4;
    zonesTable[StatistiquesSeance::Zone5] = ui->labelZone5;
    zonesTable[StatistiquesSeance::Zone6] = ui->labelZone6;
    zonesTable[StatistiquesSeance::Zone7] = ui->labelZone7;
    zonesTable[StatistiquesSeance::ZoneHorsTable] = ui->labelZoneHorsTable;

    QPixmap aucune;
    for(int i=0; i <= StatistiquesSeance::Zone7; i++)
    {
        zonesTable[i]->clear();
        zonesTable[i]->setPixmap(aucune);
        zonesTable[i]->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(35, 93,166);border: 2px solid rgb(255, 255, 255);");
    }
    zonesTable[StatistiquesSeance::ZoneHorsTable]->setStyleSheet("color: #FFFFFF;");

    ui->labelDureeSeance->setStyleSheet("color: #FFFFFF;");
}

/**
 * @brief Affiche le nombre d'impacts et le pourcentage dans une zone
 *
 * @fn TtpaIhm::afficherZone
 *
 * @param numeroZone int numéro de la zone compris entre 1 et 7
 * @param nbImpacts int nombre d'impacts dans la zone
 * @param pourcentage QString calculer pour chaque zone
 */
void TtpaIhm::afficherZone(int numeroZone, int nbImpacts, QString pourcentage)
{
    qDebug() << Q_FUNC_INFO << "numeroZone=" << numeroZone << "pourcentage" << pourcentage;
    if(numeroZone == statistiquesSeance->getZoneRobot())
        return;

    if(numeroZone >= StatistiquesSeance::Zone1 && numeroZone <= StatistiquesSeance::Zone7)
    {
        QString affichageZone = "<span style=\"font-size:32pt; font-weight:600;\">" + QString::number(nbImpacts) + "<br />" + pourcentage + "</span>";
        zonesTable[numeroZone]->setText(affichageZone);
    }
    else if(numeroZone == StatistiquesSeance::ZoneHorsTable)
    {
        QString affichageZone = "<span style=\"font-size:32pt; font-weight:600;\">Hors Table : " + QString::number(nbImpacts) + " (" + pourcentage + ")</span>";
        zonesTable[numeroZone]->setText(affichageZone);
    }
}

/**
* @brief Action crtl+Q pour Quitte
*
* @fn TtpaIhm::raccourcisClavier
*
*/
void TtpaIhm::raccourcisClavier()
{
    QAction *actionQuitter = new QAction("&Quitter", this);
    actionQuitter->setShortcut(QKeySequence(QKeySequence::Quit)); // Ctrl+Q
    addAction(actionQuitter);
    connect(actionQuitter, SIGNAL(triggered()), this, SLOT(quitter()));// Pour le déclenchement Ctrl+Q pour quitter l'application
}

/**
 * @brief Permet de quitter l'application
 *
 * @fn TtpaIhm::quitter
 *
 */
void TtpaIhm::quitter()
{
    // on ferme la fenêtre
    close();
}

/**
 * @brief Affiche le nombre de balle ayant toucher la table sur le nombre total de balle jouer par le robot sur l'écran de séance
 *
 * @fn TtpaIhm::afficherBallesTotalEcranSeance
 *
 * @param nombreTotalDeBalle et le nombre total jouer par le robot
 * @param nombreTotalDeBalleTable et le nombre total ayant toucher la table (hors table et zoneRobot non comprise)
 */
void TtpaIhm::afficherBallesTotalEcranSeance(int nombreTotalDeBalle, int nombreTotalDeBalleTable)
{
    QString nbBallesTotales = QString::number(nombreTotalDeBalle);
    QString nbBallesTotalesTable = QString::number(nombreTotalDeBalleTable);
    qDebug() << Q_FUNC_INFO << "nbBallesTotales=" << nbBallesTotales  << "nbBallesTotalesTable=" << nbBallesTotalesTable;
    QString affichage = "<span style=\"font-size:32pt; font-weight:600;\">" + nbBallesTotalesTable + " / " + nbBallesTotales + "</span>";
    ui->labelNbBallesTotales->setText(affichage);
    ui->labelNbBallesTotales->setStyleSheet("color: #FFFFFF;");
}

/**
 * @brief Affiche le nombre de balle hors zone durant toute la partie et l'affiche sur l'écran résultats(surcharge)
 *
 * @fn TtpaIhm::afficherBallesTotalHorsTable
 *
 * @param nombreTotalDeBalleHorsTable et le nombre total de balle hors table (zoneRobot + hors table)
 */
void TtpaIhm::afficherBallesTotalHorsTable(int nombreTotalDeBalleHorsTable)
{    
    qDebug() << Q_FUNC_INFO << "nombreTotalDeBalleHorsTable=" << nombreTotalDeBalleHorsTable;
    ui->labelNbBallesHorsTable->setStyleSheet("color: #FFFFFF;");
    QString nbBallesTotalesHorsTable = QString::number(nombreTotalDeBalleHorsTable);
    qDebug() << Q_FUNC_INFO << "nbBallesTotalesTable=" << nbBallesTotalesHorsTable;
    QString affichage = "<span style=\"font-size:32pt; font-weight:600;\">" + nbBallesTotalesHorsTable + "</span>";
    ui->labelNbBallesHorsTable->setText(affichage);
    ui->labelNbBallesHorsTable->setStyleSheet("color: #FFFFFF;");
}

/**
 * @brief Affiche le nombre de balle ayant atteints l'objectifs sur l'écran résultats
 *
 * @fn TtpaIhm::afficherBallesBonnes
 *
 * @param nombreDeBallesBonnes et le nombre total ayant atteint la zone objectif
 */
void TtpaIhm::afficherBallesBonnes(int nombreDeBallesBonnes)
{
    ui->labelZoneObjectifs->setStyleSheet("color: #FFFFFF;");
    QString nbBallesBonnes = QString::number(nombreDeBallesBonnes);
    qDebug() << Q_FUNC_INFO << "nbBallesBonnes=" << nbBallesBonnes;
    QString affichageBallesBonnes = "<span style=\"font-size:32pt; font-weight:600;\">" + nbBallesBonnes + "</span>";
    ui->labelZoneObjectifs->setText(affichageBallesBonnes);
    ui->labelZoneObjectifs->setStyleSheet("color: #FFFFFF;");
}

/**
 * @brief Affiche la couleur d'une zone lors qu'elle est impacter
 *
 * @fn TtpaIhm::afficherImpactCouleurs
 *
 * @param numeroZone et le numéro de la zone toucher par ImpacterZone et l'affiche en couleur en temps réel écran Seance
 */
void TtpaIhm::afficherImpactCouleurs(int numeroZone)
{
    qDebug() << Q_FUNC_INFO;
    if((numeroZone >= StatistiquesSeance::Zone1 && numeroZone <= StatistiquesSeance::Zone7) || (numeroZone == StatistiquesSeance::ZoneHorsTable))
    {
        if(numeroZone == statistiquesSeance->getZoneObjectif())
        {
            if(numeroZone != StatistiquesSeance::ZoneNonDefinie)
                zonesTable[numeroZone]->setStyleSheet("background-color: rgb(3, 254, 7);color: rgb(255, 255, 255);border: 2px solid rgb(255, 255, 255);");
        }
        else if(numeroZone == statistiquesSeance->getZoneRobot())
        {
            if(numeroZone != StatistiquesSeance::ZoneNonDefinie)
                zonesTable[numeroZone]->setStyleSheet("background-color: rgb(250, 254, 125);color: rgb(255, 255, 255);border: 2px solid rgb(255, 255, 255);");
        }
        else
            zonesTable[numeroZone]->setStyleSheet("background-color: #ff5e30;color: rgb(255, 255, 255);border: 2px solid rgb(255, 255, 255);");
        if(numeroZone == statistiquesSeance->getZoneRobot())
            zonesTable[StatistiquesSeance::ZoneHorsTable]->setStyleSheet("background-color: rgb(250, 254, 125);rgb(255, 255, 255);border: 2px solid rgb(255, 255, 255);");
        numeroZoneImpacte = numeroZone;
        QTimer::singleShot(TEMPS_IMPACT_ZONE, this, SLOT(effacerImpactZone()));
    }
}

/**
 * @brief Efface la couleur émise par afficherImpactCouleurs pour la remettre de la couleur habituel au bout de 500 ms(bleu)
 *
 * @fn TtpaIhm::effacerImpactZone
 */
void TtpaIhm::effacerImpactZone()
{
    if(numeroZoneImpacte != StatistiquesSeance::ZoneNonDefinie)
        zonesTable[numeroZoneImpacte]->setStyleSheet("color: rgb(255, 255, 255);background-color: rgb(35, 93,166);border: 2px solid rgb(255, 255, 255);");
    zonesTable[StatistiquesSeance::ZoneHorsTable]->setStyleSheet("color: #FFFFFF;background-color : rgb(35, 93,166);");
    int numeroZoneObjectif = statistiquesSeance->getZoneObjectif();
    if(numeroZoneObjectif != StatistiquesSeance::ZoneNonDefinie)
        zonesTable[numeroZoneObjectif]->setStyleSheet(("background-color: rgb(25,25,255);color: rgb(110, 254, 7);border: 2px solid rgb(255, 255, 255);"));
}

/**
 * @brief Affiche sur l'écran résultats toutes les statistique nécessaire comme balles ayant atteint l'objectifs, balles hors de la table et le nombre de série de balle sur l'objectifs
 *
 * @fn TtpaIhm::afficherStatistiqueEcranResultats
 *
 */
void TtpaIhm::afficherStatistiqueEcranResultats()
{
    QString finSeance;
    QString zoneObjectif;
    QString balleObjectif;
    QString zoneHorsTable;
    QString balleHorsTable;
    QString serieMax;
    QString balleEnchainees;
    QString dureeSeance;

    dureeSeance = "<span style=\"font-size:32pt; font-weight:600;\">" + QString::fromUtf8("Durée de la séance ") + "</span>";
    ui->labelDureeSeanceEcrie->setText(dureeSeance);
    ui->labelDureeSeanceEcrie->setStyleSheet("color: #FFFFFF;");
    finSeance =  "<span style=\"font-size:42pt; font-weight:600;\">" + QString::fromUtf8(" Fin de la Séance ") + "</span>";
    ui->labelNbBallesTotalesFin->setText(finSeance);
    ui->labelNbBallesTotalesFin->setStyleSheet("color: #FFFFFF;");
    zoneObjectif = "<span style=\"font-size:32pt; font-weight:600;\">" + QString::number(statistiquesSeance->getBallesBonnes())  + " / " + QString::number(statistiquesSeance->getBallesTotal()) + "</span>";
    ui->labelZoneObjectifs->setText(zoneObjectif);
    ui->labelZoneObjectifs->setStyleSheet("color: #FFFFFF;");
    balleObjectif = "<span style=\"font-size:32pt; font-weight:600;\">" + QString::fromUtf8("Balles ayant atteint l'objectif ") + "</span>";
    ui->labelBallesObjectifs->setText(balleObjectif);
    ui->labelBallesObjectifs->setStyleSheet("color: #FFFFFF;");
    zoneHorsTable = "<span style=\"font-size:32pt; font-weight:600;\">"+ QString::number(statistiquesSeance->getBallesHorsTable()) + " / " + QString::number(statistiquesSeance->getBallesTotal()) + "</span>";
    ui->labelNbBallesHorsTable->setText(zoneHorsTable);
    ui->labelNbBallesHorsTable->setStyleSheet("color: #FFFFFF;");
    balleHorsTable = "<span style=\"font-size:32pt; font-weight:600;\">" + QString::fromUtf8("Balles Hors de la Table ") + "</span>";
    ui->labelBallesHorsTable->setText(balleHorsTable);
    ui->labelBallesHorsTable->setStyleSheet("color: #FFFFFF;");
    balleEnchainees = "<span style=\"font-size:32pt; font-weight:600;\">"+ QString::number(statistiquesSeance->getBallesEnchainees()) + " / " + QString::number(statistiquesSeance->getBallesBonnes()) + "</span>";
    ui->labeltNbSerieMax->setText(balleEnchainees);
    ui->labeltNbSerieMax->setStyleSheet("color: #FFFFFF;");
    serieMax =  "<span style=\"font-size:32pt; font-weight:600;\">" + QString::fromUtf8("Nombre série de balles sur l'objectif ") + "</span>";
    ui->labeltSerieMax->setText(serieMax);
    ui->labeltSerieMax->setStyleSheet("color: #FFFFFF;");
    afficherStatistiquePourcentageBallesObjectif();
    afficherStatistiquePourcentageBallesHorstable();
    afficherStatistiquePourcentageBallesSerie();
}

void TtpaIhm::afficherStatistiquePourcentageBallesObjectif()
{
    QString pourcentageBallesObjectif;

    pourcentageBallesObjectif = QString("<span style=\"font-size:32pt; font-weight:600;\">") +                  QString::number(statistiquesSeance->calculerPourcentage(statistiquesSeance->getBallesBonnes()),'f',0) + " %</span>";
    qDebug() << Q_FUNC_INFO << "pourcentageBallesObjectif=" << pourcentageBallesObjectif;
    if(statistiquesSeance->calculerPourcentage(statistiquesSeance->getBallesBonnes()) >= 50.)
    {
        ui->labelPourcentageBallesObjectif->setText(pourcentageBallesObjectif);
        ui->labelPourcentageBallesObjectif->setStyleSheet("color: #00FF00;");
    }
    else
    {
        ui->labelPourcentageBallesObjectif->setText(pourcentageBallesObjectif);
        ui->labelPourcentageBallesObjectif->setStyleSheet("color: #FF0000;");
    }
}

void TtpaIhm::afficherStatistiquePourcentageBallesHorstable()
{
    QString pourcentageBallesHorsTable;

    pourcentageBallesHorsTable = "<span style=\"font-size:32pt; font-weight:600;\">" + QString::number((statistiquesSeance->calculerPourcentage(statistiquesSeance->getBallesHorsTable())),'f',0) + "%</    span>";
    qDebug() << Q_FUNC_INFO << "pourcentageBallesHorsTable=" << pourcentageBallesHorsTable;
    if(statistiquesSeance->calculerPourcentage(statistiquesSeance->getBallesHorsTable()) >= 50.)
    {
        ui->labelPourcentageBallesHorsTable->setText(pourcentageBallesHorsTable);
        ui->labelPourcentageBallesHorsTable->setStyleSheet("color: #FF0000;");
    }
    else
    {
        ui->labelPourcentageBallesHorsTable->setText(pourcentageBallesHorsTable);
        ui->labelPourcentageBallesHorsTable->setStyleSheet("color: #00FF00;");
    }
}

void TtpaIhm::afficherStatistiquePourcentageBallesSerie()
{
    QString pourcentageBallesSerie;

    pourcentageBallesSerie = QString("<span style=\"font-size:32pt; font-weight:600;\">") + QString::number((statistiquesSeance->calculerPourcentageSerieMax(statistiquesSeance->getBallesEnchainees())),'f',0) +"%</span>";
    qDebug() << Q_FUNC_INFO << "pourcentageBallesHorsTable=" << pourcentageBallesSerie;
    if(statistiquesSeance->calculerPourcentageSerieMax(statistiquesSeance->getBallesEnchainees()) >= 50.)
    {
        ui->labelPourcentageBallesSerie->setText(pourcentageBallesSerie);
        ui->labelPourcentageBallesSerie->setStyleSheet("color: #00FF00;");
    }
    else
    {
        ui->labelPourcentageBallesSerie->setText(pourcentageBallesSerie);
        ui->labelPourcentageBallesSerie->setStyleSheet("color: #FF0000;");
    }
}
/**
 * @brief Place le robot sur la table
 *
 * @fn TtpaIhm::setZoneObjectif(QString numeroZone)
 *
 * @param numeroZone indique le numéro de la zone
 */
void TtpaIhm::setZoneObjectif(QString numeroZone)
{
    bool ok = false;
    int valeur = numeroZone.toInt(&ok);
    // une valeur entière seulement pour la simulation ?
    if(!ok)
        return;
    qDebug() << Q_FUNC_INFO << "numeroZoneObjectif" << valeur;
    if(valeur >= StatistiquesSeance::Zone1 && valeur <= StatistiquesSeance::Zone7)
    {
        statistiquesSeance->setZoneObjectif(valeur);
        zonesTable[valeur]->setStyleSheet("background-color: rgb(25,25,255);color: rgb(110, 254, 7);border: 2px solid rgb(255, 255, 255);");
    }
    if ((valeur >= StatistiquesSeance::NbZonesTotales) && (valeur != StatistiquesSeance::ZoneNonDefinie))
    {
        qDebug() << " /!\\ Erreur Zone /!\\ ";
        return;
    }
}

/**
 * @brief Place le robot sur la table
 *
 * @fn TtpaIhm::setZoneRobot(QString numeroZone)
 *
 * @param numeroZone indique le numéro de la zone
 */
void TtpaIhm::setZoneRobot(QString numeroZone)
{
    bool ok = false;
    int valeur = numeroZone.toInt(&ok);
    // une valeur entière seulement pour la simulation ?
    if(!ok)
        return;
    qDebug() << Q_FUNC_INFO << "numeroZoneRobot" << valeur;
    if(valeur >= StatistiquesSeance::Zone1 && valeur <= StatistiquesSeance::Zone7)
    {
        statistiquesSeance->setZoneRobot(valeur);
        QPixmap imgRobot(":robot.png" );
        QPixmap imageRobot = imgRobot.scaled(QSize(170, 170),Qt::IgnoreAspectRatio);
        zonesTable[valeur]->setPixmap(imageRobot);
    }
    if (((valeur) >= StatistiquesSeance::NbZonesTotales && (valeur) != StatistiquesSeance::ZoneNonDefinie))
    {
        qDebug() << " /!\\ Erreur Zone /!\\ ";
        return;
    }
}

void TtpaIhm::recevoirNumeroZone(QString numeroZone)
{
    bool ok = false;
    int valeur = numeroZone.toInt(&ok);
    // une valeur entière seulement pour la simulation ?
    if(!ok)
        return;
    statistiquesSeance->impacterZone(StatistiquesSeance::Zone(valeur));
    afficherImpactCouleurs(StatistiquesSeance::Zone(valeur));
}

#ifndef QT_NO_DEBUG_OUTPUT
/**
 * @brief Permet de simuler un impact de balle à partir du clavier numérique (mode debug)
 *
 * @fn TtpaIhm::saisirNumeroZone
 * @param numeroZone QString
 */
void TtpaIhm::saisirNumeroZone(const QString numeroZone)
{
    bool ok = false;
    int valeur = numeroZone.toInt(&ok);
    // une valeur entière seulement pour la simulation ?
    if(!ok)
        return;

    switch(valeur)
    {
        case 7:
            statistiquesSeance->impacterZone(StatistiquesSeance::Zone1);
            afficherImpactCouleurs(StatistiquesSeance::Zone1);
            editNumeroZone->clear();
        break;
        case 8:
            statistiquesSeance->impacterZone(StatistiquesSeance::Zone2);
            afficherImpactCouleurs(StatistiquesSeance::Zone2);
            editNumeroZone->clear();
        break;
        case 9:
            statistiquesSeance->impacterZone(StatistiquesSeance::Zone3);
            afficherImpactCouleurs(StatistiquesSeance::Zone3);
            editNumeroZone->clear();
        break;
        case 4:
        case 5:
        case 6:
            statistiquesSeance->impacterZone(StatistiquesSeance::Zone4);
            afficherImpactCouleurs(StatistiquesSeance::Zone4);
            editNumeroZone->clear();
        break;
        case 1:
            statistiquesSeance->impacterZone(StatistiquesSeance::Zone5);
            afficherImpactCouleurs(StatistiquesSeance::Zone5);
            editNumeroZone->clear();
        break;
        case 2:
            statistiquesSeance->impacterZone(StatistiquesSeance::Zone6);
            afficherImpactCouleurs(StatistiquesSeance::Zone6);
            editNumeroZone->clear();
        break;
        case 3:
            statistiquesSeance->impacterZone(StatistiquesSeance::Zone7);
            afficherImpactCouleurs(StatistiquesSeance::Zone7);
            editNumeroZone->clear();
        break;
        case 0:
            statistiquesSeance->impacterZone(StatistiquesSeance::ZoneHorsTable);
            afficherImpactCouleurs(StatistiquesSeance::ZoneHorsTable);
            editNumeroZone->clear();
        break;
    }
}

/**
 * @brief Permet d'aller à l'écran suivant (mode debug)
 *
 * @fn TtpaIhm::allerSuivant
 *
 */
void TtpaIhm::allerSuivant()
{
    int ecranCourant = Page(ui->ecranTtpa->currentIndex());
    int ecranSuivant = (ecranCourant+1)%int(TtpaIhm::NbPages);
    ui->ecranTtpa->setCurrentIndex(ecranSuivant);
}

/**
 * @brief Permet de revenir à l'écran précédent (mode debug)
 *
 * @fn TtpaIhm::revenirPrecedent
 *
 */
void TtpaIhm::revenirPrecedent()
{
    int ecranCourant = ui->ecranTtpa->currentIndex();
    int ecranPrecedent = (ecranCourant-1)%int(TtpaIhm::NbPages);
    if(ecranPrecedent == -1)
        ecranPrecedent = NbPages-1;
    ui->ecranTtpa->setCurrentIndex(ecranPrecedent);
}

#endif
