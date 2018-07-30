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
import MuseScore 1.0
import QtQuick.Controls.Material 2.2
import Qt.labs.handlers 1.0
import Qt.labs.platform 1.0 as Platform
import QtGraphicalEffects 1.0

Rectangle {
    id: main
    anchors.fill: parent
    color: "grey"
    //    width:  Screen.width
    //    height: Screen.height
    //    height: parent.height
    //    width: parent.width

    //    Material.primary: Material.Blue
    //    Material.accent: Material.Indigo


    Item{

        anchors.fill: parent



        Rectangle{
            id: topBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            height: 55

            Button{
                id:topBarLeftButton
                flat: true
                //                anchors.top: parent.top
                anchors.left: parent.left
                //                anchors.topMargin: 5
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter

                height: 46
                width: height
                icon.width: 38
                icon.height: 38
                icon.source: "qrc:/images/ic_menu_black_48dp.png"
                onClicked: footer.open()
                z:flickid.z+1
            }

            Item{

                anchors.top: parent.top
                anchors.left: topBarLeftButton.right
                anchors.right: parent.right
                anchors.leftMargin: 5
                anchors.bottom: parent.bottom

                Flickable{

                    anchors.fill: parent
                    contentHeight: parent.height
                    contentWidth: topBarNAme.width
                    clip: true

                    Label{
                        id:topBarNAme
                        text: app.score.name
                        //                        width: parent.width
                        //                        height: topBar.height
                        anchors.verticalCenter: parent.verticalCenter
                        //                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 25
                        font.weight: Font.DemiBold
                    }
                }
            }


        }

        Item{
            anchors{
                top: topBar.bottom
                left: parent.left
                right: parent.right
                bottom: bottombar.top
                leftMargin: 5
                topMargin: 5
                bottomMargin: 5
            }
            visible: !app.score.score

            MouseArea{
                anchors.fill: parent
                onClicked: openScore.open();
            }


            Column{
                anchors.centerIn: parent
                Image {
                    sourceSize: Qt.size(100,100)
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "qrc:/images/ic_folder_open_white_48dp.png"
                }
                Label{
                     anchors.horizontalCenter: parent.horizontalCenter
                    text: "Tap to Open"
                    font.pointSize: 22
                    color: "#eaeae1"
                }
            }
        }



        Flickable{
            id: flickid
            //            anchors.fill: parent
            anchors{
                top: topBar.bottom
                left: parent.left
                right: parent.right
                bottom: bottombar.top
                leftMargin: 5
                topMargin: 5
                bottomMargin: 5
                //                leftMargin: 30
            }

            //            contentHeight: Math.max(Screen.height , flick2.height)
            //            contentWidth:   Math.max(Screen.width , flick2.width) //Screen.width

            clip: true


            property real origHeight
            property real origWidth
            property point intitialContentPosition


            flickableDirection: Flickable.HorizontalAndVerticalFlick

            onContentHeightChanged: {
                console.log("Content height changed " + contentHeight + " flickable height " + height)
            }


            PinchArea {
                id:pincha
                width: Math.max(flickid.contentWidth, flickid.width)
                height: Math.max(flickid.contentHeight, flickid.height)
                pinch.minimumScale: 0.3
                pinch.maximumScale: 2
                pinch.dragAxis: Pinch.XAndYAxis

                //            anchors.centerIn: parent

                property real initialWidth
                property real initialHeight
                property real  m_initialWidth
                property real m_initialHeight

                property real last_scale:1

                onPinchStarted: {
                    initialWidth = flickid.contentWidth
                    initialHeight = flickid.contentHeight
                }

                onPinchUpdated: {
                    // adjust content pos due to drag
                    flickid.contentX += pinch.previousCenter.x - pinch.center.x
                    flickid.contentY += pinch.previousCenter.y - pinch.center.y

                    var currscale;


                    console.log("initial width " + initialWidth + " height " + initialHeight)

                    currscale = last_scale +((Number(Math.round(pinch.scale+'e2')+'e-2')-1))


                    if(currscale < 0.6)
                        currscale = 0.6
                    else if(currscale > 2)
                        currscale = 2

                    flick2.scale = currscale

                    // resize content
                    flickid.resizeContent(Math.ceil(m_initialWidth * currscale) , Math.ceil(m_initialHeight * currscale), pinch.center)



                    console.log("current width " + flickid.contentWidth + " height " + flickid.contentHeight)


                    console.log(" scale " + flick2.scale)
                }

                onPinchFinished: {
                    // Move its content within bounds.
                    last_scale = last_scale +(pinch.scale-1)
                    if(last_scale < 0.6)
                        last_scale = 0.6
                    else if(last_scale > 2)
                        last_scale = 2
                    flickid.returnToBounds()
                }

                MouseArea{
                    propagateComposedEvents: true
                    anchors.fill: parent
                    onDoubleClicked: {
                        flick2.scale = 1
                        parent.last_scale = 1
                        flickid.contentHeight = parent.m_initialHeight
                        flickid.contentWidth = parent.m_initialWidth
                    }
                }

                //                Rectangle{
                //                    anchors.fill: parent
                //                    color: "red"
                //                    opacity: 0.3
                //                }


                Item{
                    id: flick2
                    anchors.centerIn: parent
                    height: view.height - 120
                    width: view.width
                    ScoreView{
                        id:view
                        color: "white"
                        anchors.centerIn: parent

                        Connections{
                            target: app.score

                            onScoreChanged:{


                                console.log("Score Changed height " + height + " width " + width)

                                flickid.contentHeight = height -120;
                                flickid.contentWidth = width  //+ flotating.width*2
                                pincha.m_initialHeight = height -120;
                                pincha.m_initialWidth = width
                            }
                        }

                        Component.onCompleted: {

                            var _height, _width

                            _height = Screen.height - topBar.height - bottombar.height
                            _width = Screen.width

                            //                            anchors.fill = undefined





                            if(_height < _width){
                                console.log("Setting height " + _height)
                                height=_height
                            }else{
                                console.log("Setting width " +_width)
                                width = _width
                            }


                            app.setScoreView(view);
                            console.log("ScoreView h " + height + " w " + width )

                            _height = height
                            _width = width


                            flickid.contentHeight = _height
                            flickid.contentWidth = _width //+ flotating.width*2
                            pincha.m_initialHeight = _height
                            pincha.m_initialWidth = _width

                        }

                        MouseArea{
                            id: mouse
                            anchors.fill: parent
                            //                    propagateComposedEvents: true
                            onClicked: {
                                //                               console.log("Mouse Area clicked")
                                app.clicked(mouse.x,mouse.y)
                            }
                        }

                        Rectangle{
                            id:selectRect
                            opacity: 0.2
                            color: "lightblue"
                            z: parent.z+1
                            visible: page == app.currentPage
                            property int page;
                            onXChanged: page = app.currentPage
                            onYChanged: page = app.currentPage

                        }
                    }
                }
            }
        }

        Rectangle{
            id:bottombar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height:  55
            visible: app.score.score

            property bool expanded: false

            Behavior on height{
                NumberAnimation{duration: 200}
            }


            Row{

                id: page_row
                anchors.left: parent.left
                anchors.verticalCenter:  parent.verticalCenter
                anchors.leftMargin: 5*dp
                //                anchors.bottomMargin: 5*dp

                spacing: 5*dp
                visible: app.score.maxPage > 1



                Button{

                    height: 46
                    width: height
                    highlighted: true

                    icon.width: 38
                    icon.height: 38
                    icon.source: "qrc:/images/ic_arrow_back_white_48dp.png"

                    enabled: app.currentPage > 1;
                    onClicked: app.gotoPage(app.currentPage-1)

                    anchors.verticalCenter:  parent.verticalCenter

                }
                Label{
                    text: app.currentPage + "/" + app.score.maxPage
                    //                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter:  parent.verticalCenter
                }


                Button{

                    height: 46
                    width: height
                    highlighted: true

                    icon.width: 38
                    icon.height: 38
                    icon.source: "qrc:/images/ic_arrow_forward_white_48dp.png"

                    enabled: app.currentPage < app.score.maxPage;
                    onClicked: app.gotoPage(app.currentPage+1)

                    anchors.verticalCenter:  parent.verticalCenter

                }

            }

            Item{
                anchors.left: page_row.right
                anchors.right: play_btn.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom

                Rectangle{
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    width: 1
                    color: "grey"
                }

                Rectangle{
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: 30
                    width: 1
                    color: "grey"
                }


                Row{
                    anchors.centerIn: parent
                    anchors.top: parent.top
                    height: parent.height
                    spacing: 5

                    Button{
                        anchors.verticalCenter: parent.verticalCenter
                        text: "♪"
                        padding: 0
                        height: parent.height - 10
                        width: height*0.70
                        enabled: app.score.spatium>0.7

                        //                        leftPadding: 0
                        //                        rightPadding: 0

                        //                        flat: true
                        highlighted: true
                        font.pointSize: 25
                        onClicked: {
                            //                            if(app.score.spatium<0.7)
                            app.score.spatium =  app.score.spatium - 0.20
                        }

                        font.weight: Font.Light

                        Component.onCompleted: {
                            //                                contentItem.color = "#eaeae1"
                            console.log("Button height " + height)
                        }


                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            opacity: enabled ? 1.0 : 0.3
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight

                        }

                        //                        anchors.margins: 0

                        //                        background.anchors.top: top
                        //                        background.anchors.bottom: bottom
                        //                        background.anchors.left: left
                        //                        background.anchors.right: right
                        //                        background.anchors.margins: 0


                    }

                    Button{
                        anchors.verticalCenter: parent.verticalCenter
                        text: "♪"
                        font.pointSize: 38
                        font.weight: Font.Light
                        highlighted: true
                        enabled: app.score.spatium<3


                        height: parent.height - 10
                        width: height*0.70

                        contentItem: Text {
                            text: parent.text
                            font: parent.font
                            opacity: enabled ? 1.0 : 0.3
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight

                        }


                        onClicked: {
                            //                            if(app.score.spatium<3)
                            app.score.spatium =   app.score.spatium + 0.20
                        }
                        padding: 0

                        Component.onCompleted: {
                            //                                contentItem.color = "#eaeae1"
                        }

                    }
                }

            }

            Button{

                id: play_btn
                height: 46
                width: height
                highlighted: true

                anchors.right: parent.right
                anchors.verticalCenter:  parent.verticalCenter
                anchors.rightMargin: 10

                icon.width: 38
                icon.height: 38
                icon.source: app.playing ? "qrc:/images/ic_pause_white_48dp.png" : "qrc:/images/ic_play_arrow_white_48dp.png"

                //                background.radius: height

                Component.onCompleted: {
                }


                onClicked: {
                    if(app.playing)
                        app.stop()
                    else
                        app.play()
                }

            }






        }

        Connections{
            target: app
            ignoreUnknownSignals: true
            onSelectedRectangle:{
                selectRect.x = rect.x;
                selectRect.y = rect.y;
                selectRect.width = rect.width;
                selectRect.height=rect.height;
            }
        }




    }
    SideBar{
        id: footer
    }


    Dialog{
        id: transposeDialog
        title: qsTr("Transpose By")
        standardButtons: Dialog.Ok | Dialog.Cancel

        onAccepted: {
            app.score.transpose(dropdown.currentIndex)
            close();
        }

        height: 4*(Screen.height/5);
        width:   main.width - 50
        modal:true
        x:parent.width/2-width/2
        y: parent.height/2 - height/2
        contentWidth: width

        ComboBox{
            id: dropdown
            model: app.score.transposeValues
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 20
            anchors.rightMargin: 20
            //            anchors.centerIn: parent
            anchors.verticalCenter: parent.verticalCenter
        }
    }


    Dialog{
        id: staveDialog
        title: qsTr("Stave Control")


        //        MouseArea{
        //            anchors.fill: parent
        //            enabled: parent.visible
        //            onClicked: parent.close();
        //        }

        //        width: contentItem.width
        //        height:

        standardButtons: Dialog.Ok

        modal:true
        x:parent.width/2-width/2
        y: parent.height/2 - height/2

        height: staveDialog_flick.height
        width: staveDialog_flick.width

        clip: true

        Flickable{
            id: staveDialog_flick
            contentHeight: col.height + rep.count*(0) + 95
            contentWidth: width
            height: Math.min(contentHeight, main.height - 50)
            width: main.width - 50
            flickableDirection: Flickable.VerticalFlick
            interactive: contentHeight > height
            clip: true

            onContentHeightChanged: {
                console.log("Content height " + contentHeight + " repeater count " + rep.count + " Height " + height )
                //                height = Math.min(contentHeight , main.height)
            }
            onHeightChanged: {
                console.log("Height " + height)
            }

            ColumnLayout {
                id:col
                spacing: 0
                width: parent.width
                Repeater{
                    id: rep
                    model: app.score.nstaves
                    width: parent.width


                    Flow{
                        id: row
                        //                    anchors.centerIn: parent
                        property bool inited: false

                        //                        anchors.left: parent.left
                        //                        anchors.right: parent.right
                        //                                                    height: (vollabel.height + volume.height) * 1.2
                        width:  parent.width
                        padding: 5

                        Component.onCompleted:{
                            console.log("ROW height " + height)
                            console.log("Volcol height " + volcol.height)
                            console.log("RAAW height " +  (vollabel.height + volume.height))
                            inited = true
                        }

                        Label{
                            text: "Stave #" + (index+1)
                        }

                        Column{
                            id: volcol
                            leftPadding: 10

                            Label{
                                id:vollabel
                                text: "volume " + volume.value
                            }
                            Slider{
                                id: volume
                                from:0
                                to: 100
                                stepSize: 1
                                snapMode: Slider.SnapAlways
                                value: app.score.getVolume(index);

                                onValueChanged:{
                                    if(!inited)
                                        return;
                                    app.score.setStaveVolume(index , value)
                                }

                            }
                        }
                        CheckBox{
                            text: "Muted"
                            //                        Component.onCompleted: checked = app.score.getMuted(index)
                            checked: app.score.getMuted(index)

                            onCheckedChanged: {

                                if(!inited)
                                    return;
                                app.score.muteStave(index , checked)
                            }
                        }
                        CheckBox{
                            text: "Visible"
                            checked:  app.score.getStavevisible(index)
                            onCheckedChanged:{
                                if(!inited)
                                    return;
                                app.score.hideStave(index , !checked)
                            }
                        }


                    }

                }
            }
        }
    }

    ExportDialog{
        id:exportDialog
        onAccepted: {
            app.saveAs("" , list.currentItem.m_ext)
            close();
        }
    }

    ExportDialog{
        id:shareDialog
        title: qsTr("Share")

        onAccepted: {
            app.share(list.currentItem.m_ext)
            close();
        }

    }


    DefaultFileDialog{
        id: openScore
        title: qsTr("Please choose a file")
        folder: shortcuts.home
        nameFilters: [ "All supported (*.mscz *.mscx *.xml *.mxml)" ,"Muse Score files (*.mscz *.mscx)" , "MusicXML files (*.musicxml *.mxml)"]
        mparent: main

        onAccepted: {
            app.openScore(fileUrl);
        }

    }

    DefaultFileDialog{
        id: savepdfDialog
        title: qsTr("Select location to export PDF")
        folder: shortcuts.home
        selectExisting: false
        mparent: main

        onAccepted: {
            app.exportPDF(fileUrl)
        }

    }
    ProgressDialog{
        id: progressdialog
        Connections{
            target: app
            ignoreUnknownSignals: true

            onExportProgress:{
                progressdialog.updateValue(progress);
            }
        }
    }
    About{
        id: aboutDialog
    }

}
