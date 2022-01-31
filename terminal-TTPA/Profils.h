#ifndef PROFILS_H
#define PROFILS_H

/**
  *
  * \file terminal-TTPA/Profils.h
  *
  * \brief Déclaration de la classe Profils
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
  * \class Profils
  *
  * \brief Déclaration de la classe Profils
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
class Profils : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant listeProfils READ getListeProfils NOTIFY listeProfilsUpdated)
public:
    explicit Profils(QObject *parent = nullptr);
    ~Profils();

    Q_INVOKABLE bool recupererNomProfils(QString idJoueur="");
    Q_INVOKABLE bool insererProfil(QString nom, QString idJoueur, QString idParametres);
    QVariant getListeProfils();

private:
    QList<QObject*> profils;
    BaseDeDonnees* bdd;

signals:
    void listeProfilsUpdated();

public slots:
};

#endif // PROFILS_H
