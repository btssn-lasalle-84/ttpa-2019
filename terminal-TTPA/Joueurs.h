#ifndef JOUEURS_H
#define JOUEURS_H

/**
  *
  * \file terminal-TTPA/Joueurs.h
  *
  * \brief Déclaration de la classe Joueurs
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */

#include <QObject>
#include <QVariant>

class BaseDeDonnees;

/**
  *
  * \class Joueurs
  *
  * \brief Déclaration de la classe Joueurs
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
class Joueurs : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant listeJoueurs READ getListeJoueurs NOTIFY listeJoueursUpdated)
public:
    explicit Joueurs(QObject *parent = nullptr);
    ~Joueurs();

    Q_INVOKABLE bool recupererJoueurs();
    //Q_INVOKABLE bool recupererJoueursSeance();
    Q_INVOKABLE QString insererJoueur(QString nomJoueur);
    QVariant getListeJoueurs();

private:
    QList<QObject*> joueurs;
    BaseDeDonnees* bdd;

signals:
    void listeJoueursUpdated();

public slots:
};

#endif // JOUEURS_H
