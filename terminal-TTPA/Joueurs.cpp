#include "Joueurs.h"
#include "Joueur.h"
#include "BaseDeDonnees.h"
#include <QDebug>

/**
  *
  * \file terminal-TTPA/Joueurs.cpp
  *
  * \brief Définition de la classe Joueurs
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
Joueurs::Joueurs(QObject *parent) : QObject(parent)
{
    bdd = BaseDeDonnees::getInstance();
    bdd->ouvrir("ttpa.sqlite");
    recupererJoueurs();
}

Joueurs::~Joueurs()
{
    BaseDeDonnees::detruireInstance();
}

bool Joueurs::recupererJoueurs()
{
    QString requete = "SELECT idJoueur, nom FROM joueurs ORDER BY nom ASC";
    QVector<QStringList> lesJoueurs;

    bool etat = bdd->recuperer(requete, lesJoueurs);
    if(etat)
    {
        qDebug() << Q_FUNC_INFO << lesJoueurs;
        joueurs.clear();
        joueurs.append(new Joueur());
        for(int i=0;i<lesJoueurs.size();i++)
        {
            Joueur *joueur = new Joueur(lesJoueurs.at(i).at(0), lesJoueurs.at(i).at(1), this);
            joueurs.append(joueur);
        }
        emit listeJoueursUpdated();
        return true;
    }
    return false;
}

QString Joueurs::insererJoueur(QString nomJoueur)
{
    QString idJoueur;
    QString requete = "INSERT INTO joueurs(nom) VALUES( '" + nomJoueur + "')";

    bool etat = bdd->executer(requete);
    if(etat)
    {
        requete = "SELECT last_insert_rowid() FROM parametres";
        etat = bdd->recuperer(requete, idJoueur);
        recupererJoueurs();
    }
    return idJoueur;
}

QVariant Joueurs::getListeJoueurs()
{
    return QVariant::fromValue(joueurs);
}
