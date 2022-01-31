#include "statistiquesseance.h"
#include <QDebug>

/**
  *
  * @file ecran-TTPA/statistiquesseance.cpp
  *
  * @brief Définition de la classe statistiquesseance.cpp
  *
  * @author Botella-Broc Yohann
  *
  * @version 1.4
  *
  */

/**
 * @brief Constructeur des statistique d'une Seance
 *
 * @fn TtpaIhm::TtpaIhm
 *
 * @param parent QObject Adresse de l'objet Qt parent numeroZoneObjectif définit la zone de l'objectif numeroZoneRobot définit la zone du robot nbImpactsZone définit le nombre d'impacte dans une zone (1 à 7) nbBallesTotales tiré par le robot.
 */
StatistiquesSeance::StatistiquesSeance(QObject *parent) : QObject(parent), numeroZoneObjectif(StatistiquesSeance::ZoneNonDefinie), numeroZoneRobot(StatistiquesSeance::ZoneNonDefinie), nbImpactsZone(StatistiquesSeance::NbZonesTotales, 0), nbBallesBonnes(0), nbBallesTotales(0), nbBallesTotalesTable(0), nbBallesEnchainees(0), nbBallesEnchaineesMax(0)
{
    qDebug() << Q_FUNC_INFO;
    initialiserStatistiques();
}

/**
 * @brief Destructeur de la seance
 *
 * @fn StatistiquesSeance::~StatistiquesSeance
 *
 */
StatistiquesSeance::~StatistiquesSeance()
{
    qDebug() << Q_FUNC_INFO;
}

/**
 * @brief initialise les statistiques
 *
 * @fn StatistiquesSeance::initialiserStatistiques
 *
 */
void StatistiquesSeance::initialiserStatistiques()
{
    for(int i=0; i < nbImpactsZone.size(); i++)
    {
        nbImpactsZone[i] = 0;
    }
    numeroZoneObjectif = numeroZoneRobot = StatistiquesSeance::ZoneNonDefinie;
    nbBallesBonnes = 0;
    nbBallesTotales = 0;
    nbBallesTotalesTable = 0;
    nbBallesEnchainees = 0;
    nbBallesEnchaineesMax = 0;
    balleToucheTable = true;
    ballePrecedenteToucheTable = true;
    balleToucheRobot = true;
}

/**
 * @brief  Calcul et affiche l'impact sur l'IHM et la table
 *
 * @fn StatistiquesSeance::impacterZone
 *
 * @param numeroZone int le numéro de zone de l'impact
 * @return
 */
void StatistiquesSeance::calculerStatistiques(uint8_t numeroZone)
{
    qDebug() << Q_FUNC_INFO << "numeroZone = " << numeroZone << "numeroZoneObjectif = " << numeroZoneObjectif;

    if(numeroZone == numeroZoneRobot)
    {
        nbImpactsZone[ZoneHorsTable] += 1;
    }
    else
    {
        if(numeroZone >= Zone1 && numeroZone <= ZoneHorsTable)
            nbImpactsZone[numeroZone] += 1;
    }
    if(numeroZone == numeroZoneObjectif)
    {
        nbBallesBonnes++;
    }

    if(numeroZone >= Zone1 && numeroZone <= Zone7)
        qDebug() << Q_FUNC_INFO << "nbBalles zone = " << nbImpactsZone[numeroZone];

    if((numeroZone >= StatistiquesSeance::Zone1 && numeroZone <= StatistiquesSeance::Zone7) || (numeroZone == StatistiquesSeance::ZoneHorsTable))
    {
        nbBallesTotales++;
        qDebug() << Q_FUNC_INFO << "nbBallesTotales = " << nbBallesTotales;
    }

    nbBallesTotalesTable = 0;
    for(int numeroZone = Zone1; numeroZone < NbZonesImpact; ++numeroZone)
    {
        nbBallesTotalesTable += nbImpactsZone[numeroZone];
    }
    qDebug() << Q_FUNC_INFO << "nbBallesTotalesTable = " << nbBallesTotalesTable << "nbBallesBonnes= " << nbBallesBonnes;

    if (numeroZone != ZoneNonDefinie)
    {
       if (numeroZone == numeroZoneObjectif /*|| numeroZoneObjectif == ZoneNonDefinie*/)
       {
           nbBallesEnchainees++;
       }
       else
       {
           nbBallesEnchainees = 0;
       }
    }
    else
       nbBallesEnchainees = 0;

    if (nbBallesEnchainees > nbBallesEnchaineesMax)
       nbBallesEnchaineesMax = nbBallesEnchainees;
    qDebug() << Q_FUNC_INFO << "nbBallesEnchainees = " << nbBallesEnchainees << "nbBallesEnchaineesMax = " << nbBallesEnchaineesMax;
}

bool StatistiquesSeance::impacterZone(uint8_t numeroZone)
{
    qDebug() << Q_FUNC_INFO << "numeroZone=" << numeroZone /*<< "numeroZoneRobot" << numeroZoneRobot*/;

    calculerStatistiques(numeroZone);
    afficherStatistiquesZones();
    afficherStatistiquesBallesTotales();
    afficherStatistiqueBallesBonnes();

    return true;
}

/**
 * @brief fini la seance
 *
 * @fn StatistiquesSeance::finirSeance
 *
 */
void StatistiquesSeance::finirSeance()
{
    //...
    emit ballesBonnes(getBallesBonnes());
    emit ballesTotalesHorsTable(getBallesHorsTable());
}

/**
 * @brief calcul le pourcentage de chaque zone sur le nombre de balle
 *
 * @fn StatistiquesSeance::calculerPourcentage
 * @param nbImpactsZone int le numéro de zone impacté
 * @return double le pourcentage calculé
 */
double StatistiquesSeance::calculerPourcentage(int nbImpactsZone)
{
    if (nbImpactsZone == 0)
        return 0.;

    return (double(nbImpactsZone) / double(nbBallesTotales))*100.;
}

/**
 * @brief calcul le pourcentage du nombre de série max sur le nombre de bale dans l'objectif
 *
 * @fn StatistiquesSeance::calculerPourcentageSerieMax
 * @param nbImpactsZone int le numéro de zone impacté
 * @return QString le pourcentage formaté pour l'affichage "X%"
 */
double StatistiquesSeance::calculerPourcentageSerieMax(int nbImpactsZone)
{
    if (nbImpactsZone == 0)
        return 0.;

    return (double(nbBallesEnchaineesMax) / double(nbBallesBonnes))*100.;
}
/**
 * @brief affiche les statistiques (impact et pourcentage) de toutes les zones
 *
 * @fn StatistiquesSeance::afficherStatistiquesZones
 *
 */
void StatistiquesSeance::afficherStatistiquesZones()
{    
    double pourcentage;
    for(uint8_t numeroZone = Zone1; numeroZone < NbZonesTotales; ++numeroZone)
    {
        if(numeroZone == NbZonesImpact)
            continue;

        pourcentage = calculerPourcentage(nbImpactsZone[numeroZone]);
        emit affichageZone(numeroZone, nbImpactsZone[numeroZone], QString::number(pourcentage,'f',0) + " %");
        qDebug() << Q_FUNC_INFO << "numeroZone=" << numeroZone << "nbImpactsZone" << nbImpactsZone[numeroZone] << "pourcentage" << pourcentage;
    }
}

/**
 * @brief affiche le nombre de balle total et nombre total de balle ayant touché la table
 *
 * @fn StatistiquesSeance::afficherStatistiquesBallesTotales
 *
 */
void StatistiquesSeance::afficherStatistiquesBallesTotales()
{
    emit ballesTotales(getBallesTotal(), getballesToucheTable());
}

/**
 * @brief affiche le nombre de balle hors table
 *
 * @fn StatistiquesSeance::afficherStatistiquesBallesTotalesHorsTable
 *
 */
void StatistiquesSeance::afficherStatistiquesBallesTotalesHorsTable()
{
    emit ballesTotalesHorsTable(getballesToucheTable());
}

/**
 * @brief affiche le nombre de balle ayant atteint la zone objectifs
 *
 * @fn StatistiquesSeance::afficherStatistiqueBallesBonnes
 *
 */
void StatistiquesSeance::afficherStatistiqueBallesBonnes()
{
    emit ballesBonnes(getBallesBonnes());
}
/**
 * @brief récupère les donnésde la zone du robot
 *
 * @fn StatistiquesSeance::getBallesTotal
 * @return
 */
int StatistiquesSeance::getZoneRobot()
{
    return numeroZoneRobot;
}

/**
 * @brief définie la Zone où sera la zone robot
 *
 * @fn StatistiquesSeance::setZoneRobot
 */
void StatistiquesSeance::setZoneRobot(int numeroZone)
{
    numeroZoneRobot = numeroZone;
    qDebug() << Q_FUNC_INFO << "numeroZoneRobot" << numeroZoneRobot;
}

/**
 * @brief récupère les donnés des balles tiré
 *
 * @fn StatistiquesSeance::getBallesTotal
 * @return
 */
int StatistiquesSeance::getBallesTotal()
{
    return nbBallesTotales;
}

/**
 * @brief récupère les donnés des balles hors table
 *
 * @fn StatistiquesSeance::getBallesHorsTable
 * @return
 */
int StatistiquesSeance::getBallesHorsTable()
{
    qDebug() << Q_FUNC_INFO;
    return nbImpactsZone[ZoneHorsTable];
}

/**
 * @brief récupère les donnés des balles ayant touché précédament la table
 *
 * @fn StatistiquesSeance::getballesPrecedentToucheTable
 * @return
 */
int StatistiquesSeance::getballesPrecedentToucheTable()
{
    return ballePrecedenteToucheTable;
}

/**
 * @brief récupère les donnés des balles récupère les donnés des balles ayant touché la table
 *
 * @fn StatistiquesSeance::getballesToucheTable
 * @return
 */
int StatistiquesSeance::getballesToucheTable()
{
    return nbBallesTotalesTable;
}

/**
 * @brief récupère les donnés de la zone de l'objectif
 *
 * @fn StatistiquesSeance::getZoneObjectif
 * @return
 */
int StatistiquesSeance::getZoneObjectif()
{
    return numeroZoneObjectif;
}

/**
 * @brief définie la Zone où sera la zone objectif
 *
 * @fn StatistiquesSeance::setZoneObjectif
 */
void StatistiquesSeance::setZoneObjectif(int numeroZone)
{
    numeroZoneObjectif = numeroZone;
    qDebug() << Q_FUNC_INFO << "numeroZoneObjectif" << numeroZoneObjectif;
}

int StatistiquesSeance::getBallesBonnes()
{
    return nbBallesBonnes;
}

int StatistiquesSeance::getBallesEnchainees()
{
    return nbBallesEnchaineesMax;
}
