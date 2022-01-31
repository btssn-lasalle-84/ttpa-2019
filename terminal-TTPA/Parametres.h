#ifndef PARAMETRES_H
#define PARAMETRES_H

/**
  *
  * \file terminal-TTPA/Parametres.h
  *
  * \brief Déclaration de la classe Parametres
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
  * \class Parametres
  *
  * \brief Déclaration de la classe Parametres
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
class Parametres : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant listeParametres READ getListeParametres  NOTIFY listeParametresUpdated)
public:
    explicit Parametres(QObject *parent = nullptr);
    ~Parametres();

    Q_INVOKABLE bool recupererParametres();
    Q_INVOKABLE QStringList recupererParametresProfil(QString idParametres);
    Q_INVOKABLE QString insererParametres(QString nbBalles, QString nbBallesMinute, QString effet, QString puissance, QString rotation, QString zoneObjectif, QString zoneRobot);
    QVariant getListeParametres();

private:
    QList<QObject*> parametres;
    BaseDeDonnees* bdd;

signals:
    void listeParametresUpdated();

public slots:
};

#endif // PARAMETRES_H
