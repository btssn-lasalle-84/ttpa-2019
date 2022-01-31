#ifndef TTPAIHM_H
#define TTPAIHM_H

#include <qglobal.h>
#include <QtWidgets>

/**
  *
  * @file ecran-TTPA/ttpaihm.h
  *
  * @brief Déclaration de la classe TtpaIhm
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */

#define BLUETOOTH //!< si le Bluetooth est présent (Raspberry Pi)
//#define SIMULATION

#define PERIODE_HORLOGE     1000 // en ms
#define TEMPS_IMPACT_ZONE    500 // en ms

namespace Ui {
class TtpaIhm;
}

/**
  *
  * @class TtpaIhm
  *
  * @brief La fenêtre principale de l'application
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */
class StatistiquesSeance;
class CommunicationBluetooth;

class TtpaIhm : public QWidget
    {
        Q_OBJECT

    public:
        explicit TtpaIhm(QWidget *parent = nullptr);
        ~TtpaIhm();

    private:
        Ui::TtpaIhm *ui; //!< agrégation de la partie graphique de l'IHM
        QTimer *timerHorloge; //!< temporisateur pour le rafraichissement de l'heure
        int compteurDureeSeance; //!< compteur de secondes pour la duréee d'une séance
        QVector<QLabel *> zonesTable; //!< vecteur pour les labels de zone
        StatistiquesSeance *statistiquesSeance; //!< les statistiques d'une séance
        CommunicationBluetooth *communicationBluetooth; //!< pour communiquer en Bluetooth avec la tablette
        int numeroZoneImpacte; //!< le numero de la zone qui a était impacté par la balle

    #ifndef QT_NO_DEBUG_OUTPUT
        QLineEdit *editNumeroZone; //!< pour la simulation d'un impact de balle à partir du clavier numérique (mode debug)
    #endif

        /**
         * @brief Enumeration des fenetres de l'IHM
         * @enum Page
         */
        enum Page
        {
            PageAttente = 0,
            PageSeance,
            PageResultats,
            NbPages
        };
        void rafraichirHeure();
        void initialiserZone();
        void raccourcisClavier();        

    public slots:
        void afficherHeure();
        void afficherDureeSeance();
        void afficherZone(int numeroZone, int nbImpacts, QString pourcentage);
        void quitter();
        void afficherBallesTotalEcranSeance(int nombreTotalDeBalle, int nombreTotalDeBalleTable);
        void afficherImpactCouleurs(int numeroZone);
        void effacerImpactZone();
        void afficherBallesTotalHorsTable(int nombreTotalDeBalleHorsTable);
        void afficherBallesBonnes(int nombreDeBallesBonnes);
        void afficherStatistiqueEcranResultats();
        void afficherStatistiquePourcentageBallesHorstable();
        void afficherStatistiquePourcentageBallesSerie();
        void afficherStatistiquePourcentageBallesObjectif();
        void setZoneRobot(QString numeroZone);
        void setZoneObjectif(QString numeroZone);
        void recevoirNumeroZone(QString numeroZone);
        void afficherConnexion();
        void connecterJoueur(QString nomJoueur);
        void deconnecterJoueur();
        void commencerSeance(QString nomJoueur, QString zoneObjectif, QString zoneRobot);
        void mettreEnPauseSeance();
        void reprendreSeance();
        void finirSeance();
        void resetSeance();
        void erreurRobot(QString infoDuRobot);
        void InfoRobot(QString infoDuRobot);
    #ifndef QT_NO_DEBUG_OUTPUT
        void saisirNumeroZone(const QString);
        void allerSuivant();
        void revenirPrecedent();
    #endif

    private slots:

    signals:
};

#endif // TTPAIHM_H
