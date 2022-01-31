import QtQuick 2.10
import QtQuick.Window 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.2

/**
  *
  * @file FenetreTTPA.qml
  *
  * @brief Définition de la fenêtre principale de l'application terminal mobile
  *
  * @author Youssef Hammouma
  *
  * @version 1.4
  *
  */

/**
  *
  * @class FenetreTTPA
  *
  * @brief La fenêtre principale de l'application terminal mobile
  *
  * @author Youssef Hammouma
  *
  * @version 1.4
  *
  */
Window {
    id: fenetreTTPA
    title: qsTr("TTPA 2019")
    visible: true
    color: "#F5F5F5"
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    Material.theme: Material.Light
    Material.accent: Material.Orange

    function lockToPortrait(){
        screen.allowedOrientation = Screen.Portrait;
    }

    SwipeView {
        id: vueBalayage
        currentIndex: onglets.currentIndex
        anchors.fill: parent

        FenetreAccueil {
            id: pageAccueil
        }

        FenetreReglages {
            id:pageReglages
        }

        FenetreStatistiques {
            id:pageStatistiques
        }
    }

    TabBar {
        id: onglets
        width: parent.width
        position: TabBar.Footer
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        transformOrigin: Item.Center
        anchors.horizontalCenter: parent.horizontalCenter
        currentIndex: vueBalayage.currentIndex

        TabButton {
            icon.name: "accueil"
            icon.source: "icons/terminal-TTPA/"
            icon.color: "transparent"
            //width: implicitWidth
        }
        TabButton {
            icon.name: "reglages"
            icon.source: "icons/terminal-TTPA/"
            icon.color: "transparent"
            //width: implicitWidth
        }
        TabButton {
            icon.name: "stats"
            icon.source: "icons/terminal-TTPA/"
            icon.color: "transparent"
            //width: implicitWidth
        }
    }
}
