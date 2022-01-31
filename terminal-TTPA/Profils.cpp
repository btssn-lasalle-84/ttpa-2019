#include "Profils.h"
#include "BaseDeDonnees.h"
#include "Profil.h"

#include<QDebug>

/**
  *
  * \file terminal-TTPA/Profils.cpp
  *
  * \brief Définition de la classe Profils
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
Profils::Profils(QObject *parent) : QObject(parent)
{
    bdd = BaseDeDonnees::getInstance();
    bdd->ouvrir("ttpa.sqlite");
}

Profils::~Profils()
{
    BaseDeDonnees::detruireInstance();
}

bool Profils::recupererNomProfils(QString idJoueur)
{
    QString requete = "SELECT idProfil, idJoueur, idParametres, nom FROM profils WHERE idJoueur ='" + idJoueur + "'";
    QVector<QStringList> lesProfils;

    if(idJoueur.isEmpty())
    {
        profils.clear();
        emit listeProfilsUpdated();
        return true;
    }

    bool etat = bdd->recuperer(requete, lesProfils);
    if(etat)
    {
        qDebug() << Q_FUNC_INFO << lesProfils;
        profils.clear();
        profils.append(new Profil());
        for(int i = 0; i < lesProfils.size(); i++)
        {
            Profil *profil = new Profil(lesProfils.at(i).at(0), lesProfils.at(i).at(1), lesProfils.at(i).at(2), lesProfils.at(i).at(3), this);
            profils.append(profil);
        }
        emit listeProfilsUpdated();
        return true;
    }
    return false;
}

bool Profils::insererProfil(QString nom, QString idJoueur, QString idParametres)
{
    qDebug() << Q_FUNC_INFO ;
    QString requete = "INSERT INTO profils(idJoueur,idParametres,nom) VALUES('" + idJoueur + "', '" + idParametres + "', '" + nom + "')";
    bool etat = bdd->executer(requete);
    qDebug() << Q_FUNC_INFO << "requete" << requete;
    if(etat)
    {
        recupererNomProfils(idJoueur);
        return true;
    }
    return false;
}

QVariant Profils::getListeProfils()
{
    return QVariant::fromValue(profils);
}
