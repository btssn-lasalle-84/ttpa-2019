import QtQuick 2.10
import QtQuick.Window 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.2
import QtQuick.Dialogs 1.1

/**
  *
  * @file FenetreReglages.qml
  *
  * @brief Définition de la fenêtre de réglages
  *
  * @author Youssef Hammouma
  *
  * @version 0.1
  *
  */

/**
  *
  * @class FenetreReglages
  *
  * @brief La fenêtre de réglages
  *
  * @author Youssef Hammouma
  *
  * @version 0.1
  *
  */

Item {
    id: pageReglages
    property int frequenceBalles: sliderFrequenceBalles.value
    property string balles: nombreBalles.text
    property int nbBalles: sliderNombreBalles.value
    property int effet: effets.currentIndex
    property int puissanceBalles: sliderPuissanceBalles.value
    property bool switchPosition: rotation.position
    property int nbZonesSelectionee
    property TextField nomSeance: nomSeance
    property bool nbClics: false
    property string numeroZoneObjectif
    property string numeroZoneRobot
    property string idParametres
    property ComboBox listeProfils: listeProfils

    ComboBox {
        id:listeProfils
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 50
        currentIndex: 0 // par défaut le premier élément
        textRole: "nom"
        model: profils.listeProfils
        onCurrentIndexChanged: {
            if(listeProfils.currentIndex > 0)
            {
                console.log("idJoueur : " + profils.listeProfils[listeProfils.currentIndex].idJoueur)
                console.log("nom : " + profils.listeProfils[listeProfils.currentIndex].nom)
                nomSeance.text = profils.listeProfils[listeProfils.currentIndex].nom
                nomSeance.readOnly = true

                //RECUPERATION DES PARAMETRES EN FONCTION DU PROFIL SELECTIONNE
                console.log("idParametres : "+ profils.listeProfils[listeProfils.currentIndex].idParametres)
                var parametresProfil = parametres.recupererParametresProfil(profils.listeProfils[listeProfils.currentIndex].idParametres)
                console.log("nbBalles" + parametresProfil[0])
                sliderNombreBalles.value = parametresProfil[0]
                sliderFrequenceBalles.value = parametresProfil[1]
                effets.currentIndex = parametresProfil[2]
                sliderPuissanceBalles.value = parametresProfil[3]
                console.log("rotation : "+ parametresProfil[4])
                rotation.position = parametresProfil[4]
                zoneObjectif.text = "Zone Objectif : " + parametresProfil[5]
                zoneRobot.text = "Zone Robot : " + parametresProfil[6]
            }
            else if(listeProfils.currentIndex == 0)
            {
                nomSeance.text = ""
                nomSeance.readOnly = false
                zoneObjectif.text = "Zone Objectif : "
                zoneRobot.text = "Zone Robot : "
            }
        }
    }

    TextField {
        id: nomSeance
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.top: listeProfils.bottom
        anchors.topMargin: 20
        //anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: TextInput.AlignHCenter
        placeholderText: qsTr("Nom de la séance ")
    }

    Label {
        id: frequenceBalles
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 240
        text: qsTr("Nombre de balles/minutes")
        horizontalAlignment: Text.AlignHCenter
        color: "black"
    }

    Slider {
        id: sliderFrequenceBalles
        width: 200
        height: Screen.desktopAvailableHeight / 16
        visible: true
        anchors.left: parent.left
        anchors.leftMargin: 80
        anchors.top: frequenceBalles.bottom
        from: 30
        to: 60
        stepSize: 15
    }

    Label {
        id: nombreFrequenceBalles
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: sliderFrequenceBalles.bottom
        anchors.topMargin: 5
        text: sliderFrequenceBalles.value
        horizontalAlignment: Text.AlignHCenter
        color: "black"
    }


    Label {
        id: balle
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 240
        text: qsTr("Nombre de balles")
        horizontalAlignment: Text.AlignHCenter
        color: "black"
    }

    Label {
        id: nombreBalles
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: sliderNombreBalles.bottom
        anchors.topMargin: 5
        text: sliderNombreBalles.value
        horizontalAlignment: Text.AlignHCenter
        color: "black"
    }

    Slider {
        id: sliderNombreBalles
        width: 200
        height: Screen.desktopAvailableHeight / 16
        visible: true
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: balle.bottom
        from: 10
        to: 100
        stepSize: 10
    }

    ComboBox {
        id: effets
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: nombreBalles.bottom
        anchors.topMargin: 20
        model: ["Balle coupée", "Balle liftée", "Sans effet"]
    }

    Label {
        id: puissanceBalles
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: effets.bottom
        anchors.topMargin: 50
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Puissance")
    }

    Slider {
        id: sliderPuissanceBalles
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: puissanceBalles.bottom
        from: 20
        to: 100
        stepSize: 20
    }

    Label {
        id: valeurPuissanceBalles
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: sliderPuissanceBalles.bottom
        anchors.topMargin: 2
        horizontalAlignment: Text.AlignHCenter
        text: sliderPuissanceBalles.value + "%"
    }

    Switch {
        id: rotation
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: valeurPuissanceBalles.bottom
        text: qsTr("Rotation")
    }

    Label {
        id:zoneObjectif
        text: "Zone Objectif : " + numeroZoneObjectif
        font.bold: true
        anchors.top: rotation.bottom
        anchors.left: parent.left
        anchors.leftMargin: 300
        anchors.topMargin: 5
    }

    Label {
        id:zoneRobot
        text: "Zone Robot : " + numeroZoneRobot
        anchors.left: zoneObjectif.right
        anchors.leftMargin: 10
        anchors.top: rotation.bottom
        anchors.horizontalCenter: boutonPositionRobot.horizontalCenter
        anchors.topMargin: 5
        font.bold: true
    }

    Button {
        id:boutonPositionRobot
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: rotation.bottom
        anchors.topMargin: 50
        text: qsTr("Choisir la position du robot")
        onClicked: {
            positionRobot.open();
        }

    }

    /**
     * @todo Faire en sorte que le bouton cancel reset le Dialog positionRobot et que le bouton Save valide les paramètres
     */
    Dialog {
        id: positionRobot
        modal: true
        focus: true
        title: "Choisir Objectif Robot"
        x: (Screen.desktopAvailableWidth - width) / 2
        y: Screen.desktopAvailableHeight / 6
        width: Math.min(Screen.desktopAvailableWidth + 500, Screen.desktopAvailableHeight) / 2
        contentHeight: Screen.desktopAvailableHeight - 550
        standardButtons: Dialog.Save | Dialog.Cancel
        background: Rectangle {
            color: "#0861B3"
        }

        ButtonGroup {
            id: zones
            onClicked: {

                if(button.highlighted === false && nbZonesSelectionee < 2)
                {
                    button.highlighted = true;
                    nbClics = true;
                    console.log(nbZonesSelectionee);
                    nbZonesSelectionee = nbZonesSelectionee+1;                   
                }
                else
                {
                    if(button.highlighted === true)
                    {
                        nbZonesSelectionee = nbZonesSelectionee-1;
                        button.highlighted = false;
                        nbClics = false;
                    }
                }

                if(nbZonesSelectionee == 1)
                {
                    numeroZoneObjectif = communicationBluetooth.getZoneObjectif(button.text);
                    console.log("clicked", button.text);
                    positionRobot.title = "Choisir la zone du robot";
                }

                if(nbZonesSelectionee == 0)
                {
                    positionRobot.title = "Choisir Objectif Robot";
                }

                if(nbClics == true && nbZonesSelectionee == 2)
                {
                    numeroZoneRobot = communicationBluetooth.getZoneRobot(button.text);
                    nbClics = false;
                }

            }
        }

        Button {
            id:zone1
            width: 140
            height: 140
            anchors.top: positionRobot.bottom
            anchors.topMargin: 0
            text: qsTr("Zone 1")
            ButtonGroup.group: zones;
        }

        Button {
            id:zone2
            width: 140
            height: 140
            anchors.top: positionRobot.bottom
            anchors.topMargin: 0
            anchors.left: zone1.right
            anchors.leftMargin: 16
            text: qsTr("Zone 2")
            ButtonGroup.group: zones;
        }

        Button {
            id:zone3
            width: 140
            height: 140
            anchors.top: positionRobot.bottom
            anchors.topMargin: 0
            anchors.left: zone2.right
            anchors.leftMargin: 16
            text: qsTr("Zone 3")
            ButtonGroup.group: zones;
        }

        Button {
            id:zone4
            width: positionRobot.availableWidth
            height: 140
            anchors.top: zone2.bottom
            anchors.topMargin: 0
            text: qsTr("Zone 4")
            font.bold: true
            ButtonGroup.group: zones;
        }

        Button {
            id:zone5
            width: 140
            height: 140
            anchors.top: zone4.bottom
            anchors.topMargin: 0
            text: qsTr("Zone 5")
            ButtonGroup.group: zones;
        }

        Button {
            id:zone6
            width: 140
            height: 140
            anchors.top: zone4.bottom
            anchors.topMargin: 0
            anchors.left: zone5.right
            anchors.leftMargin: 16
            text: qsTr("Zone 6")
            ButtonGroup.group: zones;
        }

        Button {
            id:zone7
            width: 140
            height: 140
            anchors.top: zone4.bottom
            anchors.topMargin: 0
            anchors.left: zone6.right
            anchors.leftMargin: 16
            text: qsTr("Zone 7")
            ButtonGroup.group: zones;
        }
   }

}
