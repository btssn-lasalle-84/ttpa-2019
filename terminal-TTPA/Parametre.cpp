#include "Parametre.h"
#include <QDebug>

/**
  *
  * \file terminal-TTPA/Parametre.cpp
  *
  * \brief Définition de la classe Parametre
  *
  * \author Hammouma Youssef
  *
  * \version 1.4
  *
  */
Parametre::Parametre(QString id, QString nbBalles, QString nbBallesMinute, QString effet, QString puissance, QString rotation, QString zoneObjectif, QString zoneRobot,QObject *parent) : QObject(parent), id(id), nbBalles(nbBalles), nbBallesMinute(nbBallesMinute), effet(effet), puissance(puissance), rotation(rotation), zoneObjectif(zoneObjectif), zoneRobot(zoneRobot)
{
    qDebug() << Q_FUNC_INFO << "id" << id << "nbBalles" << nbBalles << "nbBallesMinute" << nbBallesMinute << "effet" << effet << "puissance" << puissance << "rotation" << rotation << "zoneObjectif" << zoneObjectif << "zoneRobot" << zoneRobot;
}

QString Parametre::getId()
{
    return id;
}

QString Parametre::getNbBalles()
{
    return nbBalles;
}

QString Parametre::getNbBallesMinute()
{
    return nbBallesMinute;
}

QString Parametre::getEffet()
{
    return effet;
}

QString Parametre::getPuissance()
{
    return puissance;
}

QString Parametre::getRotation()
{
    return rotation;
}

QString Parametre::getZoneObjectif()
{
    return zoneObjectif;
}

QString Parametre::getZoneRobot()
{
    return zoneRobot;
}
