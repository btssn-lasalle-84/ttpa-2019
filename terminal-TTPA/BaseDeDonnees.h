#ifndef BASEDEDONNEES_H
#define BASEDEDONNEES_H

/**
  *
  * @file terminal-TTPA/baseDeDonnees.h
  *
  * @brief Déclaration de la classe BaseDeDonnees pour Android
  *
  * @author Thierry VAIRA
  *
  * @version 1.1
  *
  */

#include <QObject>
#include <QtSql/QtSql>
#include <QSqlDatabase>
#include <QMutex>
#include <QString>

//#define DEBUG_BASEDEDONNEES

/**
  *
  * @class BaseDeDonnees
  *
  * @brief Déclaration de la classe BaseDeDonnees pour Android
  *
  * @author Thierry VAIRA
  *
  * @version 1.1
  *
  */
class BaseDeDonnees : public QObject
{
    Q_OBJECT
    public:
        static BaseDeDonnees* getInstance();
        static void detruireInstance();

        Q_INVOKABLE bool estConnecte();
        Q_INVOKABLE void deconnecter();        
        Q_INVOKABLE bool estOuvert();
        Q_INVOKABLE bool ouvrir(QString fichierBase);

        /* uniquement pour : UPDATE, INSERT et DELETE */
        Q_INVOKABLE bool executer(QString requete);

        /* uniquement pour : SELECT */
        Q_INVOKABLE bool recuperer(QString requete, QString &donnees); // 1 -> 1
        Q_INVOKABLE bool recuperer(QString requete, QStringList &donnees); // 1 -> 1..*
        Q_INVOKABLE bool recuperer(QString requete, QVector<QString> &donnees); // 1..* -> 1
        Q_INVOKABLE bool recuperer(QString requete, QVector<QStringList> &donnees); // 1..* -> 1..*

    private:
        BaseDeDonnees();
        ~BaseDeDonnees();

        static BaseDeDonnees*  baseDeDonnees;
        static int             nbAcces;
        QSqlDatabase           db;
        QString                nomBDD;
        QMutex                 mutex;

        QSqlDatabase           getNomBDD();
};

#endif // BASEDEDONNEES_H
