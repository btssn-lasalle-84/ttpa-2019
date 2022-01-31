import QtQuick 2.10
import QtQuick.Window 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.2
import QtCharts 2.0

/**
  *
  * @file FenetreStatistiques.qml
  *
  * @brief Définition de la fenêtre de statistiques
  *
  * @author Youssef Hammouma
  *
  * @version 0.1
  *
  */

/**
  *
  * @class FenetreStatistiques
  *
  * @brief La fenêtre de statistiques
  *
  * @author Youssef Hammouma
  *
  * @version 0.1
  *
  */

Item {
    id: pageStatistiques

    Label {
        id: historique
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 5
        text: "Historique des séances de"
        font.bold: true
        font.pixelSize: 22
        color: "#000"
    }

    ComboBox {
        id: nomJoueur
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.left: historique.right
        anchors.leftMargin: 5
        currentIndex: 1
        textRole: "nom"
        font.bold: true;
        font.pixelSize: 22
        model: joueurs.listeJoueurs
        background: Rectangle {
            color: "transparent"
        }
    }

    Label {
        id:date
        text: "Date"
        anchors.top: historique.bottom
        anchors.topMargin: 70
        anchors.left: parent.left
        anchors.leftMargin: 160
        font.pixelSize: 18
        font.bold: true
        font.italic: true
    }

    Label {
        id:nbBalles
        text: "nbBalles"
        anchors.top: historique.bottom
        anchors.topMargin: 70
        anchors.left: date.right
        anchors.leftMargin: 80
        font.pixelSize: 18
        font.bold: true
        font.italic: true
    }

    Label {
        id:effet
        text: "Effet"
        anchors.top: historique.bottom
        anchors.topMargin: 70
        anchors.left: nbBalles.right
        anchors.leftMargin: 80
        font.pixelSize: 18
        font.bold: true
        font.italic: true
    }

    Label {
        id:tauxReussite
        text: "Taux de réussite"
        anchors.top: historique.bottom
        anchors.topMargin: 70
        anchors.left: effet.right
        anchors.leftMargin: 80
        font.pixelSize: 18
        font.bold: true
        font.italic: true
    }


}
