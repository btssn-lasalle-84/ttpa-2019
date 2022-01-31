#ifndef JOUEUR_H
#define JOUEUR_H

/**
  *
  * \file terminal-TTPA/Joueur.h
  *
  * \brief Déclaration de la classe Joueur
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */

#include <QObject>

/**
  *
  * \class Joueur
  *
  * \brief Déclaration de la classe Joueur
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
class Joueur : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ getId NOTIFY idChanged)
    Q_PROPERTY(QString nom READ getNom NOTIFY nomChanged)

public:
    explicit Joueur(QString id="", QString nom="", QObject *parent = nullptr);

    QString getNom();
    QString getId();

private:
    QString id;
    QString nom;

signals:
    void nomChanged();
    void idChanged();

public slots:
};

#endif // JOUEUR_H
