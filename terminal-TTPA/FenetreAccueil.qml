import QtQml 2.2
import QtQuick 2.10
import QtQuick.Window 2.1
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.2
import QtQuick.Dialogs 1.1
import QtQuick.LocalStorage 2.0

/**
  *
  * @file FenetreAccueil.qml
  *
  * @brief Définition de la fenêtre d'accueil
  *
  * @author Youssef Hammouma
  *
  * @version 0.1
  *
  */

/**
  *
  * @class FenetreAccueil
  *
  * @brief La fenêtre d'accueil
  *
  * @author Youssef Hammouma
  *
  * @version 0.1
  *
  */
Item {
    property bool recherche: communicationBluetooth.etatRecherche

    property bool robotDetecte: communicationBluetooth.peripheriqueRobotTTPADetecte
    property bool robotConnecte: communicationBluetooth.peripheriqueRobotTTPAConnecte
    property bool robotDeconnecte: communicationBluetooth.peripheriqueRobotTTPADeconnecte

    property bool ecranDetecte: communicationBluetooth.peripheriqueEcranTTPADetecte
    property bool ecranConnecte: communicationBluetooth.peripheriqueEcranTTPAConnecte
    property bool ecranDeconnecte: communicationBluetooth.peripheriqueEcranTTPADeconnecte

    property bool tableDetecte: communicationBluetooth.peripheriqueTableTTPADetecte
    property bool tableConnecte: communicationBluetooth.peripheriqueTableTTPAConnecte
    property bool tableDeconnecte: communicationBluetooth.peripheriqueTableTTPADeconnecte

    property bool erreurrobotDetectee: communicationBluetooth.erreurRobotDetecte
    property bool seanceTerminee: communicationBluetooth.seanceTerminee

    onRechercheChanged: {
       if (communicationBluetooth.etatRecherche == true)
       {
            switchBluetooth.text = "Recherche en cours"
       }
       else
       {
           switchBluetooth.text = "Recherche terminée"
           switchBluetooth.position = 0.0
           switchBluetooth.checked = false
           indicateur.running = false
           // 3 sur 3
           //if(communicationBluetooth.peripheriqueRobotTTPADetecte && communicationBluetooth.peripheriqueTableTTPADetecte && communicationBluetooth.peripheriqueEcranTTPADetecte)
           // 2 sur 3
           //if(communicationBluetooth.peripheriqueRobotTTPADetecte && communicationBluetooth.peripheriqueTableTTPADetecte)
           // 1 sur 3
           /*if(communicationBluetooth.peripheriqueRobotTTPADetecte && nomJoueur.length != 0)
           {
               boutonJouer.enabled = true;
           }*/
       }
    }

    onRobotDetecteChanged: {
        if(communicationBluetooth.peripheriqueRobotTTPADetecte)
        {
          console.log("ROBOT DETECTE")
          etatRobot.color = "#F77500"          
        }
    }

    onRobotConnecteChanged: {
        if(communicationBluetooth.peripheriqueRobotTTPAConnecte)
        {
            console.log("ROBOT CONNECTE")
            etatRobot.color = "#57F63B"
            //boutonJouer.enabled = true;
        }
    }

    onRobotDeconnecteChanged: {
        if(communicationBluetooth.peripheriqueRobotTTPADeconnecte)
        {
            console.log("ROBOT DECONNECTE")
            etatRobot.color = "#A0A0A0";
            //boutonJouer.enabled = false;
        }
    }

    onTableDetecteChanged: {
       if(communicationBluetooth.peripheriqueTableTTPADetecte)
       {
         console.log("TABLE DETECTE")
         etatTable.color = "#F77500"
       }
    }

    onTableConnecteChanged: {
        if(communicationBluetooth.peripheriqueTableTTPAConnecte)
        {
            console.log("TABLE CONNECTE")
            etatTable.color = "#57F63B"
            //boutonJouer.enabled = true;
        }
    }

    onTableDeconnecteChanged: {
        if(communicationBluetooth.peripheriqueTableTTPADeconnecte)
        {
            console.log("TABLE DECONNECTE")
            etatTable.color = "#A0A0A0";
            //boutonJouer.enabled = false;
        }
    }

    onEcranDetecteChanged: {
       if(communicationBluetooth.peripheriqueEcranTTPADetecte)
       {
         console.log("ECRAN DETECTE")
         etatEcran.color = "#F77500"
       }
    }

    onEcranConnecteChanged: {
        if(communicationBluetooth.peripheriqueEcranTTPAConnecte)
        {
            console.log("ECRAN CONNECTE")
            etatEcran.color = "#57F63B"
        }
    }

    onEcranDeconnecteChanged: {
        if(communicationBluetooth.peripheriqueEcranTTPADeconnecte)
        {
            console.log("ECRAN DECONNECTE")
            etatEcran.color = "#A0A0A0";
        }
    }

    onErreurrobotDetecteeChanged: {
        console.log("onErreurrobotDetecteeChanged : " + communicationBluetooth.erreurRobotDetecte)
        if(communicationBluetooth.erreurRobotDetecte)
        {
            messageErreurRobot.visible = true;
            boutonJouer.text = "Reprendre";
        }
        else
        {
            messageErreurRobot.visible = false;
            boutonJouer.text = "Pause";
        }
    }

    onSeanceTermineeChanged: {
        console.log("onSeanceTermineeChanged : " + communicationBluetooth.seanceTerminee)
        if(communicationBluetooth.seanceTerminee)
        {
            boutonJouer.text = "Jouer";
            boutonJouer.enabled = false;
            //boutonArreter.enabled = false;
            nomJoueur.enabled = true;
            messageErreurRobot.visible = false;
            pageReglages.enabled = true;
            listeJoueurs.enabled = true;
        }
    }

    MessageDialog {
        id:alerteNomVide
        title:"TTPA"
        text:"Veuillez entrer un nom"
    }

    MessageDialog {
        id:alertePeripherique
        title:"TTPA"
        text:"Aucun péripherique TTPA n'est connecté"
    }

    Label {
        id:messageErreurRobot
        text: "Erreur de bourrage ou Absence de balle !"
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logo.bottom
        anchors.topMargin: 60
        horizontalAlignment: Text.AlignHCenter
        color : "red"
        font.bold: true
        visible: false
    }

    ComboBox {
        id: listeJoueurs
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.centerIn: parent
        currentIndex: 0 // par défaut le premier élément
        textRole: "nom"
        model: joueurs.listeJoueurs        
        onCurrentIndexChanged: {
            if(listeJoueurs.currentIndex > 0)
            {
                console.log("Nom : " + joueurs.listeJoueurs[listeJoueurs.currentIndex].nom)
                nomJoueur.text = joueurs.listeJoueurs[listeJoueurs.currentIndex].nom
                nomJoueur.readOnly = true
                //nomJoueur.visible = false
                profils.recupererNomProfils(joueurs.listeJoueurs[listeJoueurs.currentIndex].id)
            }
            else if(listeJoueurs.currentIndex == 0)
            {
                //nomJoueur.visible = true
                nomJoueur.text = ""
                nomJoueur.readOnly = false
                profils.recupererNomProfils()
                pageReglages.nomSeance.readOnly = false
                pageReglages.nomSeance.text = ""
            }
        }
    }

    TextField {
        id: nomJoueur
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.top: listeJoueurs.bottom
        anchors.topMargin: 20
        //anchors.centerIn: parent
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: TextInput.AlignHCenter
        placeholderText: qsTr("Nom du Joueur")
    }


    Button {
        id: boutonJouer
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.top: nomJoueur.bottom
        anchors.topMargin: 50
        enabled: true
        text: qsTr("Jouer")
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            console.log(boutonJouer.text)
            if(nomJoueur.length == 0)
            {
                alerteNomVide.visible = true
            }
            else if(boutonJouer.text == "Jouer")
            {
                communicationBluetooth.demarrerSeance(nomJoueur.text, (pageReglages.numeroZoneObjectif-1), (pageReglages.numeroZoneRobot-1), pageReglages.frequenceBalles,pageReglages.nbBalles, pageReglages.effet, pageReglages.puissanceBalles, pageReglages.switchPosition);
                communicationBluetooth.seanceTerminee = false;
                boutonArreter.enabled = true;
                boutonJouer.text = "Pause";
                nomJoueur.enabled = false;
                listeJoueurs.enabled = false;
                pageReglages.enabled = false;

                var idJoueur = "";
                if(listeJoueurs.currentIndex == 0)
                {
                    if(nomJoueur.text.length > 0)
                    {
                        idJoueur = joueurs.insererJoueur(nomJoueur.text);
                    }
                }
                else
                {
                    idJoueur = joueurs.listeJoueurs[listeJoueurs.currentIndex].id;
                }

                if(pageReglages.listeProfils.currentIndex == 0)
                {
                    if(pageReglages.nomSeance.length > 0)
                    {
                        var rotation;
                        if(pageReglages.switchPosition)
                            rotation = 1;
                        else
                            rotation = 0;
                        var idParametres = parametres.insererParametres(pageReglages.nbBalles, pageReglages.frequenceBalles, pageReglages.effet, pageReglages.puissanceBalles, rotation, pageReglages.numeroZoneObjectif, pageReglages.numeroZoneRobot);
                        //if(idJoueur.length > 0 && idParametres.length > 0)
                        if(idParametres > 0)
                        {
                            console.log("insererProfil : " + idJoueur + " " + idParametres + " " + pageReglages.nomSeance.text)
                            profils.insererProfil(pageReglages.nomSeance.text, idJoueur, idParametres);
                        }
                    }
                }
            }
            else if(boutonJouer.text == "Pause")
            {
                communicationBluetooth.mettreEnPauseSeance();
                boutonJouer.text = "Reprendre";
            }
            else if(boutonJouer.text == "Reprendre")
            {
                communicationBluetooth.erreurRobotDetecte = false;
                communicationBluetooth.reprendreLaSeance();
                boutonJouer.text = "Pause";
                messageErreurRobot.visible = false;
            }
        }
    }

    Button {
        id: boutonArreter
        width: Screen.desktopAvailableWidth / 3
        height: Screen.desktopAvailableHeight / 16
        anchors.top: boutonJouer.bottom
        anchors.topMargin: 50
        enabled: false
        text: qsTr("Arreter")
        anchors.horizontalCenter: parent.horizontalCenter
        onClicked: {
            boutonJouer.text = "Jouer";
            boutonJouer.enabled = true
            boutonArreter.enabled = false;
            communicationBluetooth.arreterSeance();
            nomJoueur.enabled = true;
            messageErreurRobot.visible = false;
            pageReglages.enabled = true;
            listeJoueurs.enabled = true;
        }
    }


    ToolButton {
        id: toolButton2
        icon.name: "bluetooth"
        icon.source: "icons/terminal-TTPA/"
        icon.color: "transparent"
        icon.height: 32
        icon.width: 32
        anchors.right: parent.right
        onClicked: {
            bluetooth.open()
        }
    }

    Dialog {
        id: bluetooth
        modal: true
        focus: true
        title: "Bluetooth"
        x: (Screen.desktopAvailableWidth - width) / 2
        y: Screen.desktopAvailableHeight / 6
        width: Math.min(Screen.desktopAvailableWidth, Screen.desktopAvailableHeight) / 2
        contentHeight: Screen.desktopAvailableHeight / 2

            Text {
                id: labelnumTable
                text: "Numéro de table : "
                anchors.top: parent.top
                anchors.topMargin: 50
            }

            TextField {
                id: numTable
                text: "2"
                inputMethodHints: "ImhDigitsOnly"
                anchors.top: parent.top
                anchors.topMargin: 30
                anchors.left: labelnumTable.right
                anchors.leftMargin: 20
                anchors.rightMargin: 60
                horizontalAlignment: TextInput.AlignHCenter
                onEditingFinished: {
                    if(numTable != "")
                    {
                        communicationBluetooth.numeroTable = numTable.text
                    }
                }
            }

            Switch {
                id: switchBluetooth
                anchors.top: labelnumTable.bottom
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Rechercher les appareils")
                onClicked: {

                    if(switchBluetooth.position == 1)
                    {
                        console.log("Rechercher les appareils");
                        communicationBluetooth.rechercherPeripheriquesTTPA();
                        indicateur.running = true;
                    }
                    else
                    {
                        indicateur.running = false;
                        communicationBluetooth.arreterRecherche();
                    }
                }
            }

            BusyIndicator {
                id: indicateur
                anchors.top: switchBluetooth.bottom
                anchors.topMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                running: false
            }

            Rectangle {
                id: etatTable
                width: 30
                height: width
                anchors.top: parent.top
                anchors.topMargin: 350
                anchors.horizontalCenter: parent.right
                color: "#A0A0A0"
                border.color: "black"
                border.width: 1
                radius: width*0.5
            }

            Rectangle {
                id: etatEcran
                width: 30
                height: width
                anchors.top: parent.top
                anchors.topMargin: 350
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#A0A0A0"
                border.color: "black"
                border.width: 1
                radius: width*0.5
            }

            Rectangle {
                id: etatRobot
                width: 30
                height: width
                anchors.top: parent.top
                anchors.topMargin: 350
                anchors.right: parent.Right
                color: "#A0A0A0"
                border.color: "black"
                border.width: 1
                radius: width*0.5
            }

            Button {
                id: table
                width: 50
                icon.name: "table"
                icon.width: width
                icon.height: width
                icon.source: "icons/terminal-TTPA/"
                icon.color: "transparent"
                anchors.bottom: etatTable.top
                anchors.horizontalCenter: etatTable.horizontalCenter
                //anchors.horizontalCenter: etatRobot.Center

                background: Rectangle {
                    border.color: "transparent"
                    color: "transparent"
                }
            }

            Button {
                id: ecran
                width: 50
                icon.name: "ecran"
                icon.width: width
                icon.height: width
                icon.source: "icons/terminal-TTPA/"
                icon.color: "transparent"
                anchors.bottom: etatEcran.top
                anchors.horizontalCenter: etatEcran.horizontalCenter
                //anchors.horizontalCenter: etatRobot.Center

                background: Rectangle {
                    border.color: "transparent"
                    color: "transparent"
                }
            }

            Button {
                id: robot
                width: 50
                icon.name: "drone"
                icon.width: width
                icon.height: width
                icon.source: "icons/terminal-TTPA/"
                icon.color: "transparent"
                anchors.bottom: etatRobot.top
                anchors.horizontalCenter: etatRobot.horizontalCenter
                background: Rectangle {
                    border.color: "transparent"
                    color: "transparent"
                }
            }
        }



        Image {
            id: logo
            source: "images/logo.png"
            height: 300
            width: 600
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }


