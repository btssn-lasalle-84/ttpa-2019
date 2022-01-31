#ifndef PARAMETRE_H
#define PARAMETRE_H

/**
  *
  * \file terminal-TTPA/Parametre.h
  *
  * \brief Déclaration de la classe Parametre
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */

#include <QObject>

/**
  *
  * \class Parametre
  *
  * \brief Déclaration de la classe Parametre
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
class Parametre : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ getId NOTIFY idChanged)
    Q_PROPERTY(QString nbBalles READ getNbBalles NOTIFY nbBallesChanged)
    Q_PROPERTY(QString nbBallesMinute READ getNbBallesMinute NOTIFY nbBallesMinuteChanged)
    Q_PROPERTY(QString effet READ getEffet NOTIFY effetChanged)
    Q_PROPERTY(QString puissance READ getPuissance NOTIFY puissanceChanged)
    Q_PROPERTY(QString rotation READ getRotation NOTIFY rotationChanged)
    Q_PROPERTY(QString zoneObjectif READ getZoneObjectif NOTIFY zoneObjectifChanged)
    Q_PROPERTY(QString zoneRobot READ getZoneRobot NOTIFY zoneRobotChanged)
public:
    explicit Parametre(QString id="", QString nbBalles="", QString nbBallesMinute="", QString effet="", QString puissance="", QString rotation="", QString zoneObjectif="", QString zoneRobot="", QObject *parent = nullptr);

    QString getId();
    QString getNbBalles();
    QString getNbBallesMinute();
    QString getEffet();
    QString getPuissance();
    QString getRotation();
    QString getZoneObjectif();
    QString getZoneRobot();

private:
    QString id;
    QString nbBalles;
    QString nbBallesMinute;
    QString effet;
    QString puissance;
    QString rotation;
    QString zoneObjectif;
    QString zoneRobot;


signals:
    void idChanged();
    void nbBallesChanged();
    void nbBallesMinuteChanged();
    void effetChanged();
    void puissanceChanged();
    void rotationChanged();
    void zoneObjectifChanged();
    void zoneRobotChanged();


public slots:
};

#endif // PARAMETRE_H
