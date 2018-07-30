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

Dialog{
    property bool isOpen: false

    y: (parent.height /2) - (height/2)
    x: (parent.width /2) - (width/2)

    onOpened: isOpen = true;
    onClosed: isOpen = false;
    height: parent.height/5
    width: parent.width - 20

    title: "Exporting"

    standardButtons: "Cancel"

    onRejected: {
        app.score.cancelExport();
    }

    modal: true
    closePolicy: "NoAutoClose"

    Component.onCompleted: {
        ckeys.backpressed.connect(function(){
            if(isOpen)
                app.score.cancelExport();
            close();
        })
    }

    Rectangle{
        anchors.fill: parent

//            Label{
//                id: explabel
//                text: qsTr("Exporting")
//                anchors.top: parent.top
//                anchors.left: parent.left
//                anchors.margins: 5
//                font.pointSize: 20
//            }

            RowLayout{
                anchors.verticalCenterOffset: height/2
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width

                ProgressBar{

                    height: parent.height/5
                    id: bar
                     Layout.fillHeight: true
                    Layout.fillWidth: true
                    background: Rectangle{
                        anchors.fill: parent
                        color: "lightgrey"
                    }

                    contentItem: Rectangle{
                        height: parent.height
                        width:  parent.visualPosition * parent.width
                        color: Material.color(Material.Blue)
                    }
                }
                Label{
                    id: label
                     Layout.fillHeight: true
                     Layout.preferredWidth: width
                }
            }

    }

    function updateValue(val){
        if(val>=1){
            bar.value =0;
            close();
        }else{
            if(!isOpen)
                open();
            bar.value = val;
            label.text = (val*100).toFixed(1) + "%"
        }
    }

}
