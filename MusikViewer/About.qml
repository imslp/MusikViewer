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
    height: parent.height - 50
    width: parent.width - 50
    modal: true
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    contentItem: Rectangle{
        anchors.fill: parent

        Flickable{
            anchors{
                top: parent.top
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
            width: parent.width - 40
            contentWidth: width
            contentHeight: col.height + 10
            interactive: contentHeight > height
            flickableDirection: Flickable.VerticalFlick
            clip: true

            ColumnLayout{
                id: col
                width: parent.width
                anchors.centerIn: parent
                spacing: 10

                Image {
                    sourceSize: Qt.size(80,80)
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/icon/xxxhdpi.png"
                }
                Column{
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 3
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pointSize: 18
                        text: "MuseViewer"
                    }
                    Label{
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: "Version: 1.0"
                    }
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.maximumWidth:  parent.width
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: qsTr("Copyright (C) 2012-2018 Werner Schweer and others
Copyright (C) 2018 Project Petrucci LLC")
                    onWidthChanged: console.log("Width " + width)
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.maximumWidth:  parent.width
                    width: parent.width
                    wrapMode: Text.WordWrap
                    clip: true
                    text: qsTr("This program is free software; you can redistribute it andor modify it under the terms of the GNU General Public License version 2.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.")
                }
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.maximumWidth:  parent.width
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: "Full GPL license can be found <a href='https://www.gnu.org/licenses/gpl-2.0.en.html'>here</a>"
                    onLinkActivated: Qt.openUrlExternally(link)
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.maximumWidth:  parent.width
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: qsTr("Source code is hosted <a href='https://github.com/ProjectPetrucci/MusikViewer'>here</a>")
                    onLinkActivated: Qt.openUrlExternally(link)
                }
                Rectangle{
                    width: parent.width
                    Layout.minimumWidth: parent.width
                    height: 1
                    color: "black"
                }
                Image {
                    sourceSize: Qt.size(80,80)
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/qt-logo.jpg"
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.maximumWidth:  parent.width
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: "Qt is a C++ toolkit for cross-platform application development.
Qt provides single-source portability across all major desktop operating systems. It is also available for embedded Linux and other embedded and mobile operating systems.
Qt is available under three different licensing options designed to accommodate the needs of our various users.
Qt licensed under our commercial license agreement is appropriate for development of proprietary/commercial software where you do not want to share any source code with third parties or otherwise cannot comply with the terms of the GNU LGPL version 3 or GNU LGPL version 2.1.
Qt licensed under the GNU LGPL version 3 is appropriate for the development of Qt applications provided you can comply with the terms and conditions of the GNU LGPL version 3.
Qt licensed under the GNU LGPL version 2.1 is appropriate for the development of Qt applications provided you can comply with the terms and conditions of the GNU LGPL version 2.1.
Please see <a href='https://qt.io/licensing'>qt.io/licensing</a> for an overview of Qt licensing.
Copyright (C) 2015 Digia Plc and/or its subsidiary(-ies) and other contributors.
Qt and the Qt logo are trademarks of Digia Plc and/or its subsidiary(-ies).
Qt is a Digia product developed as an open source project. See <a href='https://qt.io'>qt.io</a> for more information."
                    textFormat: Text.RichText
                    onLinkActivated: Qt.openUrlExternally(link)
                }

                Rectangle{
                    width: parent.width
                    Layout.minimumWidth: parent.width
                    height: 1
                    color: "black"
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.maximumWidth:  parent.width
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: "MusicXML is an open file format for exchanging digital sheet music, supported by many applications.
Copyright © 2004-2017 the Contributors to the MusicXML"
                    onLinkActivated: Qt.openUrlExternally(link)
                }


            }
        }
    }
}
