#include "Joueur.h"
#include <QDebug>

/**
  *
  * \file terminal-TTPA/Joueur.cpp
  *
  * \brief Définition de la classe Joueur
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */

Joueur::Joueur(QString id, QString nom, QObject *parent) : QObject(parent), id(id), nom(nom)
{
    qDebug() << Q_FUNC_INFO << "id" << id << "nom" << nom;
}

QString Joueur::getNom()
{
    return nom;
}

QString Joueur::getId()
{
    return id;
}
