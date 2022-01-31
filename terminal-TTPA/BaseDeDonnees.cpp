
/**
  *
  * @file terminal-TTPA/baseDeDonnees.cpp
  *
  * @brief Définition de la classe BaseDeDonnees pour Android
  *
  * @author Thierry Vaira
  *
  * @version 1.1
  *
  */

#include "BaseDeDonnees.h"

#include <QDebug>

BaseDeDonnees* BaseDeDonnees::baseDeDonnees = NULL;
int BaseDeDonnees::nbAcces = 0;

BaseDeDonnees::BaseDeDonnees()
{
    #ifdef DEBUG_BASEDEDONNEES
    qDebug() << Q_FUNC_INFO;
    #endif

    nomBDD = "qsqlite";
    db = QSqlDatabase::addDatabase("QSQLITE", nomBDD);
}

BaseDeDonnees::~BaseDeDonnees()
{
    #ifdef DEBUG_BASEDEDONNEES
    qDebug() << Q_FUNC_INFO;
    #endif
}

BaseDeDonnees* BaseDeDonnees::getInstance()
{    
	if (baseDeDonnees == NULL)
	{
		baseDeDonnees = new BaseDeDonnees();
	}
	nbAcces++;
	#ifdef DEBUG_BASEDEDONNEES
	qDebug() << Q_FUNC_INFO << "nbAcces" << nbAcces;
	#endif
	return baseDeDonnees;    
}

void BaseDeDonnees::detruireInstance()
{    
	if (baseDeDonnees != NULL)
	{
		if (nbAcces > 0)
		{
			nbAcces--;
		}
	}
	#ifdef DEBUG_BASEDEDONNEES
	qDebug() << Q_FUNC_INFO << "nbAcces" << nbAcces;
	#endif
	if (nbAcces == 0)
	{
		baseDeDonnees->deconnecter();
		delete baseDeDonnees;
		baseDeDonnees = NULL;
	}
}

QSqlDatabase BaseDeDonnees::getNomBDD()
{
    return QSqlDatabase::database(nomBDD, false);
}

bool BaseDeDonnees::estConnecte()
{
    QMutexLocker verrou(&mutex);
    return getNomBDD().isOpen();
}

void BaseDeDonnees::deconnecter()
{
    QMutexLocker verrou(&mutex);
    if(getNomBDD().isOpen())
    {
        qDebug() << Q_FUNC_INFO << getNomBDD().driverName();
        getNomBDD().close();
    }
}

bool BaseDeDonnees::estOuvert()
{
    QMutexLocker verrou(&mutex);
    return getNomBDD().isOpen();
}

bool BaseDeDonnees::ouvrir(QString fichierBase)
{
    if (getNomBDD().driverName() != "QSQLITE")
    {
        return false;
    }

    QMutexLocker verrou(&mutex);
    if(!getNomBDD().isOpen())
    {
       QFile sfile(QString("assets:/db") + QString("/" + fichierBase));
       QFile dfile(QString("./" + fichierBase));

       // supprime le fichier destination
       /*if (sfile.exists())
       {
           if (dfile.exists())
           {
               dfile.remove();
           }
       }*/

       // copie
       if (sfile.exists())
       {
           sfile.copy(dfile.fileName());
           bool retour = dfile.setPermissions(QFileDevice::ReadUser|QFileDevice::WriteUser|QFileDevice::ReadOther);
           qDebug() << Q_FUNC_INFO << retour << dfile.permissions();
           //QFileInfo info(dfile);
           //qDebug() << Q_FUNC_INFO << "path" << info.absoluteFilePath();
           QFile dossierApplication("/data/data/org.qtproject.TTPA");
           retour = dossierApplication.setPermissions(QFileDevice::ReadUser|QFileDevice::WriteUser|QFileDevice::ExeUser|QFileDevice::ReadOther|QFileDevice::WriteOther|QFileDevice::ExeOther);
           qDebug() << Q_FUNC_INFO << retour << dossierApplication.permissions();
           QFile dossierFichiers("/data/data/org.qtproject.TTPA/files");
           retour = dossierFichiers.setPermissions(QFileDevice::ReadUser|QFileDevice::WriteUser|QFileDevice::ExeUser|QFileDevice::ReadOther|QFileDevice::WriteOther|QFileDevice::ExeOther);
           qDebug() << Q_FUNC_INFO << retour << dossierFichiers.permissions();
       }

       getNomBDD().setDatabaseName(QString("./") + fichierBase);

       #ifdef DEBUG_BASEDEDONNEES
       qDebug() << Q_FUNC_INFO << getNomBDD().databaseName();
       #endif

       if(getNomBDD().open())
       {
           #ifdef DEBUG_BASEDEDONNEES
           qDebug() << Q_FUNC_INFO << QString::fromUtf8("Ouverture réussie de '%1'").arg(getNomBDD().databaseName());
           #endif

           return true;
       }
       else
       {
           qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : impossible d'ouvrir la base de données !");
           return false;
       }
    }
    else
    {
        return true;
    }
}

/* pour : UPDATE, INSERT et DELETE */
bool BaseDeDonnees::executer(QString requete)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery enregistrements(db);
    bool estCorrect;

    if (getNomBDD().isOpen())
    {
        if(requete.contains("UPDATE") || requete.contains("INSERT") || requete.contains("DELETE"))
        {
            estCorrect = enregistrements.exec(requete);

            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(estCorrect)).arg(requete);
            #endif

            if (estCorrect)
            {
                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(enregistrements.lastError().text()).arg(requete);

                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);

            return false;
        }
    }
    else
    {
        return false;
    }

}

/* uniquement pour récupérer (SELECT) UN champ d'UN seul enregistrement
   Remarque : le champ est stocké dans un QString
 */
bool BaseDeDonnees::recuperer(QString requete, QString &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery enregistrements(db);
    bool estCorrect;

    if (getNomBDD().isOpen())
    {
        if (requete.contains("SELECT"))
        {
            estCorrect = enregistrements.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(estCorrect)).arg(requete);
            #endif

            if (estCorrect)
            {
                // on se positionne sur l'enregistrement
                enregistrements.first();

                // on vérifie l'état de l'enregistrement retourné
                if(!enregistrements.isValid())
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Résultat non valide !");
                    #endif

                    return false;
                }

                // on récupère sous forme de QString la valeur du champ
                if (enregistrements.isNull(0))
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Aucun résultat !");
                    #endif

                    return false;
                }
                donnees = enregistrements.value(0).toString();

                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif

                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(enregistrements.lastError().text()).arg(requete);

                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);
            return false;
        }
    }
    else
    {
        return false;
    }
}

/* uniquement pour récupérer (SELECT) plusieurs champs d'UN seul enregistrement
   Remarque : les différents champs sont stockés dans un QStringList
 */
bool BaseDeDonnees::recuperer(QString requete, QStringList &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery enregistrements(db);
    bool estCorrect;

    if (getNomBDD().isOpen())
    {
        if (requete.contains("SELECT"))
        {
            estCorrect = enregistrements.exec(requete);

            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << QString::fromUtf8("<BaseDeDonnees::recuperer(QString, QStringList)> retour %1 pour la requete : %2").arg(QString::number(estCorrect)).arg(requete);
            #endif

            if (estCorrect)
            {
                // on se positionne sur l'enregistrement
                enregistrements.first();

                // on vérifie l'état de l'enregistrement retourné
                if (!enregistrements.isValid())
                {
                    #ifdef DEBUG_BASEDEDONNEES
                    qDebug() << Q_FUNC_INFO << QString::fromUtf8("Résultat non valide !");
                    #endif

                    return false;
                }

                // on récupère sous forme de QString la valeur de tous les champs sélectionnés
                // et on les stocke dans une liste de QString
                for(int i = 0; i < enregistrements.record().count(); i++)
                {
                    if (!enregistrements.isNull(i))
                    {
                        donnees << enregistrements.value(i).toString();
                    }
                }

                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif

                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(enregistrements.lastError().text()).arg(requete);

                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);

            return false;
        }
    }
    else
    {
        return false;
    }
}

/* uniquement pour récupérer (SELECT) un seul champ de plusieurs enregistrements
   Remarque : le champ des différents enregistrements est stocké dans un QVector de QString
 */
bool BaseDeDonnees::recuperer(QString requete, QVector<QString> &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery enregistrements(db);
    bool estCorrect;
    QString valeur;

    if (getNomBDD().isOpen())
    {
        if (requete.contains("SELECT"))
        {
            estCorrect = enregistrements.exec(requete);

            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(estCorrect)).arg(requete);
            #endif

            if(estCorrect)
            {
                // pour chaque enregistrement
                while (enregistrements.next() )
                {
                    // on récupère sous forme de QString la valeur du champs sélectionné
                    valeur = enregistrements.value(0).toString();

                    #ifdef DEBUG_BASEDEDONNEES
                    //qDebug() << Q_FUNC_INFO << "Enregistrement -> " << data;
                    #endif

                    // on stocke l'enregistrement dans le QVector
                    donnees.push_back(valeur);
                }

                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif

                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(enregistrements.lastError().text()).arg(requete);
                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);

            return false;
        }
    }
    else
    {
        return false;
    }
}

/* uniquement pour récupérer (SELECT) plusieurs champs de plusieurs enregistrements
   Remarque : les différents champs des différents enregistrements sont stockés dans un QVector de QStringList
 */
bool BaseDeDonnees::recuperer(QString requete, QVector<QStringList> &donnees)
{
    QMutexLocker verrou(&mutex);
    QSqlQuery enregistrements(db);
    bool estCorrect;
    QStringList valeurs;

    if(getNomBDD().isOpen())
    {
        if (requete.contains("SELECT"))
        {
            estCorrect = enregistrements.exec(requete);
            #ifdef DEBUG_BASEDEDONNEES
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Retour %1 pour la requete : %2").arg(QString::number(estCorrect)).arg(requete);
            #endif

            if (estCorrect)
            {
                // pour chaque enregistrement
                while (enregistrements.next() )
                {
                    // on récupère sous forme de QString la valeur de tous les champs sélectionnés
                    // et on les stocke dans une liste de QString
                    for(int i=0; i < enregistrements.record().count(); i++)
                    {
                        valeurs << enregistrements.value(i).toString();
                    }

                    #ifdef DEBUG_BASEDEDONNEES
                    //qDebug() << Q_FUNC_INFO << "Enregistrement -> " << data;
                    /*for(int i=0;i<r.record().count();i++)
                        qDebug() << r.value(i).toString();*/
                    #endif

                    // on stocke l'enregistrement dans le QVector
                    donnees.push_back(valeurs);

                    // on efface la liste de QString pour le prochain enregistrement
                    valeurs.clear();
                }

                #ifdef DEBUG_BASEDEDONNEES
                qDebug() << Q_FUNC_INFO << "Enregistrement -> " << donnees;
                #endif

                return true;
            }
            else
            {
                qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : %1 pour la requête %2").arg(enregistrements.lastError().text()).arg(requete);

                return false;
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO << QString::fromUtf8("Erreur : requête %1 non autorisée !").arg(requete);

            return false;
        }
    }
    else
    {
        return false;
    }
}
