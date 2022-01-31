#ifndef PROFIL_H
#define PROFIL_H

/**
  *
  * \file terminal-TTPA/Profil.h
  *
  * \brief Déclaration de la classe Profil
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
#include <QObject>

/**
  *
  * \class Profil
  *
  * \brief Déclaration de la classe Profil
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
class Profil : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ getId NOTIFY idChanged)
    Q_PROPERTY(QString idJoueur READ getIdJoueur NOTIFY idJoueurChanged)
    Q_PROPERTY(QString idParametres READ getIdParametres NOTIFY idParametresChanged)
    Q_PROPERTY(QString nom READ getNom NOTIFY nomChanged)
public:
    explicit Profil(QString id="", QString idJoueur="", QString idParametres="", QString nom="", QObject *parent = nullptr);

    QString getId();
    QString getIdJoueur();
    QString getIdParametres();
    QString getNom();

private:
    QString id;
    QString idJoueur;
    QString idParametres;
    QString nom;

signals:
    void idChanged();
    void idJoueurChanged();
    void idParametresChanged();
    void nomChanged();


public slots:
};

#endif // PROFIL_H
