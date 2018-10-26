/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Dialogs module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
//Copyright (C) 2018 Michail Montesatnos <musikviewer@imslp.org>

import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Controls 1.2 as C1
import QtQuick.Controls.Private 1.0 as ControlsPrivate
import QtQuick.Dialogs 1.1
import QtQuick.Dialogs.Private 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import Qt.labs.folderlistmodel 2.1
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.2

//import MyQml 1.0

import "qml"

AbstractFileDialog{
    id: root
    //    property alias customwindow: wind.visible
    //        height: Screen.height
    //        width: Screen.width
    property var mparent
    property string selectedPath;
    property string currpath;
    property string currname;

    onCurrpathChanged: console.log("CurrPath Changed to " + currpath)
    //   height: parent.height
    //   width: parent.width



    property variant mfolderModel

    Keys.onBackPressed:{
        console.log("Back Pressed! MyAbstractFileDialog")
        dirUp();
        event.accepted=true
    }

    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            console.log("Back button of phone pressed")
            event.accepted = true
        }
    }


    Component {
        id: modelComponent
        FolderListModel {
            id: folderModel
            showFiles: !root.selectFolder
            nameFilters: root.selectedNameFilterExtensions
            sortField:  FolderListModel.Name
            sortReversed: view.sortIndicatorOrder === Qt.DescendingOrder

            onFolderChanged: console.log("Folder Changed count " + count + " folder " + folder)

            Component.onCompleted: {
                console.log("Folder model Created!" + folderModel)
            }
            //            Component.onDestroyed: {
            //                console.log("Folder Model destroyed")
            //            }

        }
    }

    onVisibleChanged: {
        if (visible) {
            // If the TableView doesn't have a model yet, create it asynchronously to avoid a UI freeze


            if (!view.model) {
                mfolderModel = modelComponent.createObject(window, { })
                console.log("INIT!!!")
                view.model = mfolderModel
                mfolderModel.nameFilters = root.selectedNameFilterExtensions
                root.folder = mfolderModel.folder
                fileview.model = mfolderModel

                root.currname = futils.__shortcuts[futils.__shortcuts.length-1].name


                var folder =  root.urlToPath(futils.__shortcuts[futils.__shortcuts.length-1].url)

                if(folder.endsWith("/")){
                    folder = folder.slice(0,-1)
                }

                root.currpath = folder
                root.folder = futils.__shortcuts[futils.__shortcuts.length-1].url
            }

            //Clear

            clearSelection();
            savename.text=""
        }

    }

    Component.onCompleted: {

        filterField.currentIndex = root.selectedNameFilterIndex
        //        root.favoriteFolders = settings.favoriteFolders
    }

    Component.onDestruction: {
        settings.favoriteFolders = root.favoriteFolders
    }

    property Settings settings: Settings {
        category: "QQControlsFileDialog"
        //        property alias width: root.width
        //        property alias height: root.height
        //        property alias sidebarWidth: sidebar.width
        //        property alias sidebarSplit: shortcutsScroll.height
        property alias sidebarVisible: root.sidebarVisible
        property variant favoriteFolders: []
    }

    property bool showFocusHighlight: false
    property SystemPalette palette: SystemPalette { }
    property var favoriteFolders: []

    function dirDown(path) {
        view.selection.clear()
        root.folder = "file://" + path
    }
    function dirUp() {

        var folder;
        console.log("Currpath " +currpath )
        console.log("urlPath " + root.urlToPath(root.folder) )
        if(currpath.toUpperCase() == root.urlToPath(root.folder).toUpperCase()){

            root.reject();
            return;
        }
        if (view.model.parentFolder != ""){
            console.log("parent Folder " + view.model.parentFolder )

            if(view.model.parentFolder.toString().endsWith("/")){
                folder = view.model.parentFolder.toString().slice(0,-1)
            }else{
                folder = view.model.parentFolder
            }

            root.folder = folder
        }
    }
    function acceptSelection() {
        // transfer the view's selections to QQuickFileDialog
        //        clearSelection()
        if (selectFolder && view.selection.count === 0)
            addSelection(folder)
        else {
            view.selection.forEach(function(idx) {
                if (view.model.isFolder(idx)) {
                    if (selectFolder)
                        addSelection(view.model.get(idx, "fileURL"))
                } else {
                    if (!selectFolder)
                        addSelection(view.model.get(idx, "fileURL"))
                }
            })
        }
        accept()
    }

    property Action dirUpAction: Action {
        //        text: "\ue810"
        shortcut: "Ctrl+U"
        onTriggered: dirUp()
        //        text: qsTr("Go up to the folder containing this one")

    }


    Rectangle {
        id: window
        //        visible: true
        implicitWidth: mparent.width //Math.min(root.__maximumDimension, Math.max(Screen.pixelDensity * 100, -1))
        implicitHeight: mparent.height //Math.min(root.__maximumDimension, Screen.pixelDensity * 80)
        color: "lightgrey"

        onParentChanged: {
            console.log("New parent!!!! " + parent)
        }

        focus: true

        //        anchors.fill: parent
        //        color: root.palette.window

        Component.onCompleted: {
            ckeys.backpressed.connect(function(){console.log("Back QML!") ; dirUp();})
        }


        Binding {

            target: view.model
            property: "folder"
            value: root.folder
            when: view.model
        }
        //        Binding{
        //            target: fileview.model
        //            property: "folder"
        //            value: root.folder
        //        }

        Binding {
            target: currentPathField
            property: "url"
            value: root.folder
        }

        Drawer {
            id: sidebar
            //            x: 0
            width: parent.width*0.66
            height: parent.height
            ListView {
                id: shortcutsView
                model: futils.__shortcuts.length
                //                anchors.fill: parent
                anchors{
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: filterField.top
                }

                anchors.margins: ControlsPrivate.Settings.hasTouchScreen ? Screen.pixelDensity * 3.5 : anchors.margins
                //                    anchors.bottomMargin: ControlsPrivate.Settings.hasTouchScreen ? Screen.pixelDensity * 3.5 : anchors.margins
                //                    implicitHeight: model.count * sidebarSplitter.rowHeight
                delegate: Rectangle {
                    id: shortcutItem
                    //                        width: sidebarSplitter.width
                    color: shortcutLabel.text == root.currname ? Material.color(Material.LightBlue) : "transparent"
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: shortcutLabel.implicitHeight * 1.2

                    Button{
                        id: shortcutLabel
                        anchors.left: parent.left
                        anchors.right: parent.right
                        flat: true

                        //                        anchors.centerIn: parent
                        anchors.verticalCenter: parent.verticalCenter

                        topPadding: 0
                        bottomPadding: 0
                        anchors{
                            margins: 0
                        }

                        background.anchors.top: top
                        background.anchors.bottom: bottom
                        text: futils.__shortcuts[index].name

                        onClicked: {
                            root.selectedPath = root.urlToPath(futils.__shortcuts[index].url)


                            var folder = root.urlToPath(futils.__shortcuts[index].url)
                            while(folder.endsWith("/")){
                                folder = folder.slice(0,-1)
                            }
                            root.currpath = folder //root.urlToPath(futils.__shortcuts[index].url)
                            root.currname = futils.__shortcuts[index].name
                            root.folder = futils.__shortcuts[index].url
                            sidebar.close();
                        }
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
                        color: "grey"
                    }

                    Rectangle{
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors{
                            margins: 0
                        }
                        height: 0
                        anchors.topMargin: 0
                        color: "grey"
                    }

                }
            }
            ComboBox {
                id: filterField
                model: root.nameFilters
                visible: !selectFolder
                anchors{
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                onCurrentTextChanged: {
                    root.selectNameFilter(currentText)
                    if (view.model)
                        view.model.nameFilters = root.selectedNameFilterExtensions
                }
            }
        }

        Rectangle {
            id: titleBar
            height: dirup.height
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            Button{
                id:drawerbutton
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 5
                flat:true
                height: 46
                width: height
                icon.source: "qrc:/images/ic_menu_black_48dp.png"
                icon.width: 38
                icon.height: 38

                onClicked: sidebar.open()
                //                 anchors.right: parent.right

            }

            Text {
                id: currentPathField
                property string url
                anchors.right: dirup.left
                anchors.rightMargin: 15
                anchors.left: drawerbutton.right
                anchors.leftMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                //                readOnly:root.selectFolder
                clip: true

                onUrlChanged: {
                    console.log("Url Changed " + url)
                    var str = root.urlToPath(url);
                    var regEx = new RegExp(currpath, "ig");
                    console.log("str " + str + " currpath " + currpath )

                    var str2 = currname + str.replace(regEx , "");

                    if(str2.endsWith("/")){
                        str2 = str2.slice(0,-1)
                    }

                    text = str2
                }

                //                onTextChanged: {
                //                    console.log("On Text changed raplce" + root.currpath)
                //                    var mtext = text;
                //                    text = currname + mtext.replace(root.currpath , "")
                //                }

            }
            Button {
                id:dirup
                action: dirUpAction
                //                    style: IconButtonStyle { }
                //                    Layout.maximumWidth: height * 1.5
                //                    text: "\ue810"
                height: 46
                width: height
                icon.source: currpath.toUpperCase() != root.urlToPath(root.folder).toUpperCase() ? "qrc:/images/ic_arrow_upward_black_48dp.png" : "qrc:/images/ic_arrow_back_black_48dp.png"
                icon.width: 38
                icon.height: 38
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 7
                flat: true
                //                    width: height * 1.5
            }

        }


        Item{
            anchors.fill: parent
            visible: view.model.count===0

            Text{
                anchors.centerIn: parent
                text: qsTr("Folder is empty")
                font.pointSize: 22
                color: "white"
            }
        }

        Flickable{
            contentHeight: foldergrid.height + filegrid.height
            contentWidth: window.width
            flickableDirection: Flickable.VerticalFlick
            clip: true
            anchors{
                top: titleBar.bottom
                left: window.left
                right: window.right
                bottom: bottomBar.top
                //                leftMargin: 50
            }
            //            Column{
            //                id: flickcol
            GridLayout{
                id: foldergrid
                width: parent.width
                columns: 2
                //                    spacing: 5

                Repeater{
                    id: view
                    onModelChanged: {
                        console.log("Model Changed " + model)

                    }


                    delegate: Item{
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: height
                        Layout.preferredWidth: width
                        height: width/2.5
                        width: (window.width/2.2) //- 3
                        visible: fileIsDir
                        DropShadow{
                            anchors.fill: foldersdel
                            horizontalOffset: 3
                            verticalOffset: 3
                            radius: 6.0
                            //                                samples: 17
                            color: "#80000000"
                            source: foldersdel
                        }

                        Rectangle{
                            id:foldersdel
                            anchors.fill: parent
                            anchors.margins: 5
                            border.color: "lightgrey"
                            border.width: 1
                            Row {
                                //                implicitWidth: pathText.implicitWidth + pathText.anchors.leftMargin + pathText.anchors.rightMargin
                                //                height: pathText.height * 1.5
                                anchors.fill: parent
                                anchors.leftMargin: 5

                                spacing: 10 //width/10
                                //                                    Component.onCompleted: console.log("Height " + height + " width " + width)
                                Image {
                                    id: folderIcon
                                    source:  fileIsDir ? "qrc:/images/ic_folder_black_48dp.png" : "qrc:/images/ic_file_music_grey600_48dp.png"
                                    sourceSize.height:  parent.height - 25

                                    //                                    sourceSize.width: height
                                    anchors.verticalCenter:  parent.verticalCenter
                                    fillMode: Image.PreserveAspectFit
                                }
                                Text {
                                    id: folderPath
                                    text: fileName //styleData.value
                                    width: foldersdel.width - folderIcon.width - 25
                                    height: parent.height

                                    //                                        font.pointSize: 22
                                    minimumPointSize: 10
                                    fontSizeMode: Text.VerticalFit
                                    clip: true
                                    //                    color: styleData.textColor
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                    renderType: ControlsPrivate.Settings.isMobile ? Text.QtRendering : Text.NativeRendering
                                    anchors.verticalCenter:  parent.verticalCenter
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked:{
                                    root.folder = fileURL
                                    view.model.folder = fileURL
                                }
                            }
                        }
                    }
                }
            }
            GridLayout{
                id: filegrid
                columns: 2
                width: parent.width
                //                    spacing: 5
                anchors.top: foldergrid.bottom

                Repeater{
                    id: fileview
                    model:  root.folderModel
                    delegate: Item{
                        id:delroot
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: height
                        Layout.preferredWidth: width
                        height: width*1.2
                        width: (window.width/2.2)
                        visible: !fileIsDir
                        DropShadow{
                            anchors.fill: filesrect
                            horizontalOffset: 3
                            verticalOffset: 3
                            radius: 6.0
                            //                                samples: 17
                            color: "#80000000"
                            source: filesrect
                        }
                        Rectangle{
                            id: filesrect
                            anchors.fill: parent
                            anchors.margins:5
                            border.color: "lightgrey"
                            border.width: 1
                            clip:true
                            Image {
                                id: fileIcon
                                source:  fileIsDir ? "qrc:/images/ic_folder_open_black_48dp.png" : "qrc:/images/ic_file_music_grey600_48dp.png"
                                //                                    sourceSize.height:  width
                                sourceSize.width: parent.width/2
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                anchors.topMargin: 10
                                //                                    anchors.verticalCenter: parent.verticalCenter
                                //                                    anchors.verticalCenterOffset: -height/2
                                fillMode: Image.PreserveAspectFit
                                //                                    anchors.centerIn: parent
                            }
                            Column{
                                id: filecolumn
                                anchors.top: fileIcon.bottom
                                anchors.margins: 10
                                anchors.topMargin: 5
                                anchors.left: parent.left
                                anchors.right: parent.right
                                height: parent.height - fileIcon.height
                                clip:true
                                Text {
                                    id: pathText
                                    text: fileName //styleData.value
                                    width: filesrect.width - 5
                                    height: filecolumn.height - (filesize.height*2.5)

                                    //                                        font.pointSize: 22
                                    minimumPointSize: 10
                                    fontSizeMode: Text.VerticalFit
                                    clip: true
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    elide: Text.ElideRight
                                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                    renderType: ControlsPrivate.Settings.isMobile ? Text.QtRendering : Text.NativeRendering
                                    //                                        anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.left: parent.left
                                }
                                Text{
                                    id: filesize
                                    text: qsTr("Size:") + (fileSize/1000000.0).toFixed(2) + qsTr("MB")
                                    font.pointSize: 12
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked:{
                                    if (root.addSelection(fileURL))
                                        root.accept()
                                }
                            }
                        }
                    }
                }
            }
            //            }
        }

        Rectangle {
            id: bottomBar
            width: parent.width
            height: !selectExisting ? buttonRow.height + buttonRow.spacing * 2 : 0
            anchors.bottom: parent.bottom
            visible: !selectExisting

            Row {
                id: buttonRow
                anchors.right: parent.right
                anchors.rightMargin: spacing*2
                anchors.left: parent.left
                anchors.leftMargin:spacing*2
                anchors.verticalCenter: parent.verticalCenter
                spacing: 4

                TextField{
                    id:savename
                    width: bottomBar.width - save.width - buttonRow.spacing*6

                }
                Button{
                    id:save
                    text: qsTr("Save")
                    onClicked: {
                        root.addSelection(root.currpath + savename.text)
                        root.accept();
                    }
                }
            }
        }
    }
}

