#include "Profil.h"
#include <QDebug>

/**
  *
  * \file terminal-TTPA/Profil.cpp
  *
  * \brief Définition de la classe Profil
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
Profil::Profil(QString id, QString idJoueur, QString idParametres, QString nom,QObject *parent) : QObject(parent), id(id), idJoueur(idJoueur), idParametres(idParametres), nom(nom)
{
    qDebug() << Q_FUNC_INFO << "id" << id << "idJoueur" << idJoueur << "idParametres" << idParametres << "nom" << nom;
}

QString Profil::getId()
{
    return id;
}

QString Profil::getIdJoueur()
{
    return idJoueur;
}

QString Profil::getIdParametres()
{
    return idParametres;
}

QString Profil::getNom()
{
    return nom;
}
