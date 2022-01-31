#ifndef STATISTIQUEDETABLE_H
#define STATISTIQUEDETABLE_H

/**
  *
  * @file ecran-TTPA/statistiquesseance.h
  *
  * @brief Déclaration de la classe StatistiquesSeance
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */

#include <qglobal.h>
#include <QObject>
#include <QVector>
#include <QLabel>


/**
  *
  * @class StatistiquesSeance
  *
  * @brief Déclaration de la classe StatistiquesSeance
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */
class StatistiquesSeance : public QObject
{
    Q_OBJECT
    public:
        StatistiquesSeance(QObject *parent=nullptr);
        ~StatistiquesSeance();

        /**
         * @enum Zone
         * @brief les zones numérotés de 1 à 7 de la table plus une zone non définie et le nombre de zone total
         */
        enum Zone
        {
            ZoneNonDefinie = -1,
            Zone1 = 0,
            Zone2,
            Zone3,
            Zone4,
            Zone5,
            Zone6,
            Zone7,
            NbZonesImpact, // 7
            ZoneHorsTable, // 8
            NbZonesTotales, // 9
        };

        bool impacterZone(uint8_t numeroZone);
        void initialiserStatistiques();
        int getBallesBonnes();
        int getBallesTotal();
        int getBallesHorsTable();
        int getballesPrecedentToucheTable();
        int getballesToucheTable();
        int getBallesEnchainees();
        int getZoneObjectif();
        void setZoneObjectif(int numeroZone);
        int getZoneRobot();
        void setZoneRobot(int numeroZone);
        double calculerPourcentage(int nbImpactsZone);
        double calculerPourcentageSerieMax(int nbImpactsZone);
        void afficherStatistiquesZones();
        void afficherStatistiquesBallesTotales();
        void afficherStatistiquesBallesTotalesHorsTable();
        void afficherStatistiqueBallesBonnes();
        
    private:
        int numeroZoneObjectif; //!< Placement de la zone de l'objectif
        int numeroZoneRobot; //!< Placement de la zone d'objectif
        QVector<int> nbImpactsZone; //!< Un vecteur avec le nombre d'impacts dans chaque zone
        int nbBallesBonnes; //!< Nombre de balles ayant touché la zone objetifs
        int nbBallesTotales; //!< Nombre de balles total tirées par le robot et jouables
        int nbBallesTotalesTable; //!< Nombre de balles totales tirées par le robot et ayant impacté la table
        int nbBallesEnchainees;  //!< Série de balles dans la zone objectif
        int nbBallesEnchaineesMax;  //!< Série max de balles dans la zone objectif
        bool balleToucheTable; //!< Balle ayant touché la table
        bool ballePrecedenteToucheTable;//!< Balle précédente qui a touché la table
        bool balleToucheRobot; //!< Balle ayant touché la table du robot

        void calculerStatistiques(uint8_t numeroZone);

    private slots:

    public slots:
        void finirSeance();

    signals:
        void affichageZone(int numeroZone, int nbImpacts, QString pourcentage);
        void ballesTotales(int nbBallesTotales, int nbBallesTotalesTable);
        void ballesTotalesHorsTable(int ballesToucheTable);
        void ballesBonnes(int nbBallesBonnes);
};

#endif // STATISTIQUEDETABLE_H
