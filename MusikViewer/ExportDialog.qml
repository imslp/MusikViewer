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
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2 as Dialogs
import QtQuick.Controls.Material 2.2
import Qt.labs.handlers 1.0
import Qt.labs.platform 1.0 as Platform
import QtGraphicalEffects 1.0

Dialog{
    standardButtons: Dialog.Ok | Dialog.Cancel
    x:parent.width/2-width/2
    y: parent.height/2 - height/2
    title: qsTr("Save As")
    property alias list: saveAsList

    height: 4*(Screen.height/5);
    width:   main.width - 50
    modal:true

    //        Frame{
    //            height: parent.height
    //            width: parent.width
    ListView{
        id: saveAsList
        width: parent.width
        height: parent.height
        contentWidth: width
        clip: true
        currentIndex: -1
        model:  ListModel{
            id: tumblermodel
            ListElement{
                name:"MuseScore Compressed"
                subtitle:""
                ext:"mscz"
            }
            ListElement{
                name:"MuseScore file"
                subtitle:""
                ext:"mscx"
            }
            ListElement{
                name:"MusicXML"
                ext:"musicxml"
                subtitle:""
            }
            ListElement{
                name:"MusicXML Compressed"
                ext:"mxl"
                subtitle:""
            }
            ListElement{
                name: "PDF file"
                ext: "pdf"
                subtitle:"Exports all pages"
            }
            ListElement{
                name:"PNG image"
                ext: "png"
                subtitle: "Exports current page"
            }
            //                ListElement{
            //                    name:"Midi audio file"
            //                    ext: "mid"
            //                    subtitle:""
            //                }
            ListElement{
                name: "WAV audio file"
                ext: "wav"
                subtitle:""
            }
            ListElement{
                name: "FLAC audio file"
                ext: "flac"
                subtitle:""
            }
            ListElement{
                name: "OGG audio file"
                ext: "ogg"
                subtitle:""
            }
            ListElement{
                name: "MP3 audio file"
                ext: "mp3"
                subtitle:""
            }
        }

        delegate: Item {
            id: saveAsDelegate
            property string m_ext: ext
            height: exportnamelabel.height*2 + 30
            width: parent.width
            property bool selected: saveAsList.currentIndex == index;
            Rectangle{
                anchors.top:parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 0.5
                color: "black"
            }
            Rectangle{
                anchors.bottom:parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 0.5
                color: "black"
            }

            MouseArea{
                id: saveDelegateMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: saveAsList.currentIndex = index;
                onContainsMouseChanged:{
                    if(containsMouse && !parent.selected)
                        delegateGreyRect.opacity = 0.3
                    else
                        delegateGreyRect.opacity = 0
                }
            }

            Item{
                id: _item
                height: parent.height
                states: State {
                    name: "highlighted"
                    when: saveAsDelegate.selected
                    PropertyChanges {
                        target: _item
                        width : saveAsDelegate.width
                    }
                    PropertyChanges {
                        target: gradient1
                        position: 1.0
                    }
                }
                transitions: Transition {
                    reversible: true
                    SequentialAnimation{
                        NumberAnimation{target: _item ;  property: "width" ;duration: 80}
                        NumberAnimation{ target: gradient1 ; property: "position" ;duration: 140}
                    }

                }

                LinearGradient{
                    anchors.fill: parent
                    start: Qt.point(0 , 0)
                    end: Qt.point(width , 0)
                    gradient: Gradient{
                        GradientStop{
                            position: 0
                            color: "lightblue"
                        }
                        GradientStop{
                            id:gradient1
                            position: 0
                            color: "lightblue"
                        }
                        GradientStop{
                            id:gradient2
                            position: 1.1
                            color: "white"
                        }
                    }
                }
            }


            Rectangle{
                id: delegateGreyRect
                anchors.fill: parent
                color: "lightgrey"
                opacity: 0.0
                Behavior on opacity {

                    NumberAnimation {
                        duration: 100
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            Column{
                anchors.centerIn: parent
                z: parent.z+1
                Label{
                    id: exportnamelabel
                    text: name + " (."+ext+")"
                    wrapMode: "WordWrap"
                    //                        Component.onCompleted: {
                    //                            console.log("extension" + ext)
                    //                        }
                }
                Label{
                    text: subtitle
                }
            }
        }
        //            highlight: Rectangle{
        //                color: Material.color(Material.BlueGrey)
        //            }

    }
}
