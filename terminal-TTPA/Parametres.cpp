#include "Parametres.h"
#include "Parametre.h"
#include "BaseDeDonnees.h"
#include <QDebug>

/**
  *
  * \file terminal-TTPA/Parametres.cpp
  *
  * \brief Définition de la classe Parametres
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
Parametres::Parametres(QObject *parent) : QObject(parent)
{
    bdd = BaseDeDonnees::getInstance();
    bdd->ouvrir("ttpa.sqlite");
    recupererParametres();
}

Parametres::~Parametres()
{
    BaseDeDonnees::detruireInstance();
}

bool Parametres::recupererParametres()
{
    QString requete = "SELECT idParametres, nbBalles, nbBallesMinute, effet, puissance, rotation, zoneObjectif, zoneRobot FROM parametres";
    QVector<QStringList> lesParametres;

    bool etat = bdd->recuperer(requete, lesParametres);
    if(etat)
    {
        qDebug() << Q_FUNC_INFO << lesParametres;
        lesParametres.clear();
        for(int i=0; i < lesParametres.size(); i++)
        {
            Parametre *parametre = new Parametre(lesParametres.at(i).at(0), lesParametres.at(i).at(1),
                                                 lesParametres.at(i).at(2), lesParametres.at(i).at(3),
                                                 lesParametres.at(i).at(4), lesParametres.at(i).at(5),
                                                 lesParametres.at(i).at(6), lesParametres.at(i).at(7));
            parametres.append(parametre);
        }
        emit listeParametresUpdated();
        return true;
    }
    return false;
}

QStringList Parametres::recupererParametresProfil(QString idParametres)
{
    QString requete = "SELECT parametres.nbBalles, parametres.nbBallesMinute, parametres.effet, parametres.puissance, parametres.rotation, parametres.zoneObjectif, parametres.zoneRobot FROM parametres INNER JOIN profils ON parametres.idParametres = profils.idParametres WHERE parametres.idParametres =" + idParametres;

    QStringList lesParametres;

    bool etat = bdd->recuperer(requete, lesParametres);
    if(etat)
    {
        qDebug() << Q_FUNC_INFO << lesParametres;
    }
    return lesParametres;
}

QString Parametres::insererParametres(QString nbBalles, QString nbBallesMinute, QString effet, QString puissance, QString rotation, QString zoneObjectif, QString zoneRobot)
{
    if(nbBalles.isEmpty() || nbBallesMinute.isEmpty() || effet.isEmpty() || puissance.isEmpty() || rotation.isEmpty() || zoneObjectif.isEmpty() || zoneRobot.isEmpty())
        return QString("");

    QString idParametres;
    QString requete = "INSERT INTO parametres(nbBalles, nbBallesMinute, effet, puissance, rotation, zoneObjectif, zoneRobot) VALUES("+ nbBalles + "," + nbBallesMinute + "," + effet + "," + puissance + "," + rotation + "," + zoneObjectif + "," + zoneRobot + ")";

    bool etat = bdd->executer(requete);
    if(etat)
    {
        requete = "SELECT last_insert_rowid() FROM parametres";
        etat = bdd->recuperer(requete, idParametres);
        recupererParametres();
    }
    return idParametres;
}

QVariant Parametres::getListeParametres()
{
    return QVariant::fromValue(parametres);
}
