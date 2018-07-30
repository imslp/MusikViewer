/* ==========================================================================================
//   						GPL License and Copyright Notice
// This file is part of MuseViewer.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with MuseViewer; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>
// ========================================================================================== */



import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Drawer {
    id:footer
    height: parent.height


    width: mainColumn.width

    z: flickid.z+1

    background: Rectangle{ color: Material.color(Material.BlueGrey) }
    Item{
        anchors.fill: parent


        Rectangle{
            width: 1
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            color: "#eaeae1"
        }

        Image {
            id: image
            source: "qrc:/icon/xxxhdpi.png"
            sourceSize: Qt.size(90,90)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
        }

        Flickable{
            anchors.top: image.bottom
            anchors.topMargin: 15
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            clip: true
            interactive: contentHeight > height

            contentHeight: mainColumn.height

            Component.onCompleted: {
                console.log("Content Height " + contentHeight)
            }

            Column{

                id:mainColumn

                Material.background: Material.Blue
                anchors.horizontalCenter: parent.horizontalCenter

                //                    leftPadding: 15
                //                    rightPadding: 15

                width: Screen.width*0.6666




                Button{
                    text: "Open Score"
                    onClicked: {
                        footer.close();
                        openScore.open();
                    }

                    anchors.left: parent.left
                    anchors.right: parent.right


                    Component.onCompleted: {
                        contentItem.color = "#eaeae1"
                    }
                    //                        leftPadding: 15
                    //                        rightPadding: 15



                    topPadding: 0
                    bottomPadding: 0
                    anchors{
                        margins: 0
                    }
                    background.anchors.top: top
                    background.anchors.bottom: bottom

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right

                        anchors{
                            margins: 0
                        }
                        height: 1
                        anchors.topMargin: 0
                        color: "#455963"
                    }
                    flat: true
                }


                Button{
                    text: qsTr("Transpose")
                    enabled: app.score.score
                    onClicked:{
                        footer.close();
                        transposeDialog.open()
                    }

                    anchors.left: parent.left
                    anchors.right: parent.right

                    topPadding: 0
                    bottomPadding: 0
                    anchors{
                        margins: 0
                    }
                    background.anchors.top: top
                    background.anchors.bottom: bottom

                    Component.onCompleted: {
                        contentItem.color = "#eaeae1"
                    }

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right

                        anchors{
                            margins: 0
                        }
                        height: 1
                        anchors.topMargin: 0
                        color: "#455963"
                    }

                    flat: true
                }
                Button{
                    text: "Stave Control"
                    enabled: app.score.score
                    onClicked: {
                        footer.close();
                        staveDialog.open()
                    }
                    anchors.left: parent.left
                    anchors.right: parent.right

                    topPadding: 0
                    bottomPadding: 0
                    anchors{
                        margins: 0
                    }
                    background.anchors.top: top
                    background.anchors.bottom: bottom

                    Component.onCompleted: {
                        contentItem.color = "#eaeae1"
                    }

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right

                        anchors{
                            margins: 0
                        }
                        height: 1
                        anchors.topMargin: 0
                        color: "#455963"
                    }
                    flat: true
                }

                Button{
                    text: "Save As"
                    enabled: app.score.score
                    onClicked: {
                        footer.close();
                        exportDialog.open();
                    }

                    anchors.left: parent.left
                    anchors.right: parent.right

                    topPadding: 0
                    bottomPadding: 0
                    anchors{
                        margins: 0
                    }
                    background.anchors.top: top
                    background.anchors.bottom: bottom

                    Component.onCompleted: {
                        contentItem.color = "#eaeae1"
                    }

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right

                        anchors{
                            margins: 0
                        }
                        height: 1
                        anchors.topMargin: 0
                        color: "#455963"
                    }
                    flat: true


                    Rectangle{
                        anchors.top: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right

                        anchors{
                            margins: 0
                        }
                        height: 1
                        anchors.topMargin: 0
                        color: "#455963"
                    }
                }
                Button{
                    text: "Share"
                    enabled: app.score.score
                    onClicked: {
                        footer.close();
                        shareDialog.open()
                    }

                    anchors.left: parent.left
                    anchors.right: parent.right

                    topPadding: 0
                    bottomPadding: 0
                    anchors{
                        margins: 0
                    }
                    background.anchors.top: top
                    background.anchors.bottom: bottom

                    Component.onCompleted: {
                        contentItem.color = "#eaeae1"
                    }

                    Rectangle{
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right

                        anchors{
                            margins: 0
                        }
                        height: 1
                        anchors.topMargin: 0
                        color: "#455963"
                    }
                    flat: true


                    Rectangle{
                        anchors.top: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right

                        anchors{
                            margins: 0
                        }
                        height: 1
                        anchors.topMargin: 0
                        color: "#455963"
                    }
                }


                Column{

                    spacing: 15

                    anchors.horizontalCenter: parent.horizontalCenter


                    Item{
                        height: 15 //parent.spacing
                        width: 1
                    }

                    Column{

                        anchors.horizontalCenter: parent.horizontalCenter
                        enabled: !app.playing

                        Label{
                            color: "#eaeae1"
                            text: "Playback Speed"
                            bottomPadding: 0
                        }
                        Label{
                            visible: app.playing
                            color: "red" //"#eaeae1"
                            text: "Pause playback before \n changing Playback Speed"

                            bottomPadding: 0
                        }

                        RowLayout{
                            anchors.horizontalCenter: parent.horizontalCenter


                            Button{
                                text: "-"
                                flat: true
                                font.pointSize: 32
                                onClicked:{
                                    if(app.score.tempo>5)
                                    app.score.tempo  = Math.round(app.score.tempo) - 5
                                }
                                padding: 0

                                font.weight: Font.Light

                                Component.onCompleted: {
                                    contentItem.color = "#eaeae1"
                                }
                            }

                            Row{
                                TextField{
                                    anchors.verticalCenter: parent.verticalCenter
                                    text:  Math.round(app.score.tempo)
                                    font.pointSize: 18
                                    color: "#eaeae1"
                                    width: 30
                                    horizontalAlignment: TextInput.AlignHCenter
                                    verticalAlignment: TextInput.AlignVCenter
                                    inputMethodHints: Qt.ImhDigitsOnly
                                    validator: IntValidator{ bottom: 1 ; top: 350 ; }
                                    onEditingFinished: { app.score.tempo = text;}
                                }
                                Text{
                                     anchors.verticalCenter: parent.verticalCenter
                                    text: "  bpm"
                                    font.pointSize: 18
                                    color: "#eaeae1"


                                }
                            }
                            Button{
                                text: "+"
                                font.pointSize: 32
                                font.weight: Font.Light
                                flat: true
                                onClicked:{
                                    //                                        enabled = false
                                    if(app.score.tempo<350)
                                    app.score.tempo  = Math.round(app.score.tempo) + 5
                                }
                                padding: 0

                                //                                    Timer{
                                //                                        interval: 250
                                //                                        repeat: false
                                //                                        running: !parent.enabled
                                //                                        onTriggered: parent.enabled=true;
                                //                                    }

                                Component.onCompleted: {
                                    contentItem.color = "#eaeae1"
                                }

                            }
                        }
                    }

                    Column{
                        spacing: 0
                        anchors.horizontalCenter: parent.horizontalCenter
                        Label{
                            text: "Line spacing"
                            color: "#eaeae1"
                            bottomPadding: 0

                        }

                        Row{
                            anchors.horizontalCenter: parent.horizontalCenter

                            Button{
                                anchors.verticalCenter: parent.verticalCenter
                                text: "-"
                                flat: true
                                font.pointSize: 32
                                onClicked: {
                                    if(app.score.spatium>0.5)
                                        app.score.spatium =  app.score.spatium - 0.20
                                }
                                padding: 0
                                font.weight: Font.Light

                                Component.onCompleted: {
                                    contentItem.color = "#eaeae1"
                                }
                            }

                            Row{
                                TextField{
                                    anchors.verticalCenter: parent.verticalCenter
                                    text:  app.score.spatium
                                    font.pointSize: 18
                                    color: "#eaeae1"
                                    width: 30
                                    horizontalAlignment: TextInput.AlignHCenter
                                    verticalAlignment: TextInput.AlignVCenter
                                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                                    validator: DoubleValidator{ bottom: 0.5 ; top: 5 ; decimals:2 }
                                    onEditingFinished:  { app.score.spatium = text;}
                                }
                                Text{
                                     anchors.verticalCenter: parent.verticalCenter
                                    text: "  mm"
                                    font.pointSize: 18
                                    color: "#eaeae1"


                                }
                            }
                            Button{
                                anchors.verticalCenter: parent.verticalCenter
                                text: "+"
                                font.pointSize: 32
                                font.weight: Font.Light
                                flat: true
                                onClicked: {
                                    if(app.score.spatium<3)
                                        app.score.spatium =   app.score.spatium + 0.20
                                }
                                padding: 0

                                Component.onCompleted: {
                                    contentItem.color = "#eaeae1"
                                }

                            }
                        }

                    }
                    Label{
                        anchors.right: parent.right
                        anchors.rightMargin: 5
                        topPadding: 10
                        font.pointSize: 18
                        color: "#eaeae1"
                        text: qsTr("About")
                        MouseArea{
                            anchors.fill: parent
                            onClicked:{ footer.close() ;  aboutDialog.open() }
                        }
                    }
                }
            }
        }
    }
}
