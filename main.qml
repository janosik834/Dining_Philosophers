import QtQuick 2.9
import QtQuick.Controls 2.2
import Philosophers 1.0
import QtQuick.Layouts 1.3



ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 500
    minimumWidth: 300
    maximumWidth: 1200

    minimumHeight: 170
    maximumHeight: 800

    title: qsTr("Feasting Philosophers")




    Component {
        id: philosopherGui
        Rectangle {
            id: rectangle
            width: 130
            height: 170
            color: "#e8ebe6"

            Button {
                id: button
                anchors.right:  parent.right
                anchors.rightMargin: 5
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 110
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 145
                onClicked: {modelOne.tab.removePhil(index);}
                Image {
                    id: image1
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: "image/remove.png"

                }
            }

            Image {
                id: face
                anchors.top: parent.top
                anchors.topMargin: 25
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 50
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
                source: "image/face.jpg"
            }
            Text {
                id: t_name
                text: name
                y:15
                anchors {
                    top: face.bottom;
                    topMargin: 5
                    horizontalCenter: parent.horizontalCenter}
                font.pixelSize: 12
            }

            Text {
                id: t_state
                text: current_state
                y: 15
                anchors {
                    top: t_name.bottom
                    topMargin: 5
                    horizontalCenter: parent.horizontalCenter}
                font.pixelSize: 12
            }


        }
    }


    RowLayout {
        id: mainlayout
        anchors.fill: parent
        height: 400
        Layout.minimumWidth: 300
        Layout.maximumWidth: 1000
        Layout.preferredWidth: 400
        Layout.minimumHeight: 130
        Layout.maximumHeight: 500
        Layout.preferredHeight: 300
        Layout.topMargin: 100
        Button {
            id: button
            Layout.fillWidth: true
            Layout.minimumWidth: 63
            Layout.preferredWidth: 63
            Layout.maximumWidth: 150

            height: 23
            text: qsTr("ADD")
            onClicked: { modelOne.tab.addPhil(); }
            onDoubleClicked: {update()}
        }
        Button {
            id: button1
            Layout.fillWidth: true
            Layout.minimumWidth: 80
            Layout.maximumWidth: 150
            Layout.preferredWidth: 80
            width: 84
            height: 23
            text: qsTr("REMOVE")
            onClicked: {  modelOne.tab.removePhil(modelOne.tab.getRemovedIndex());}
        }
        ScrollView{
            Layout.fillWidth: true
            Layout.minimumWidth: 150
            Layout.maximumWidth: 1000
            Layout.preferredWidth: 400
            Layout.minimumHeight: 130
            Layout.maximumHeight: 500
            Layout.preferredHeight: 300
            Layout.bottomMargin: 40
            Layout.fillHeight: true
            id: table
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            GridView {
                id: chairs
                anchors.fill: parent
                cellWidth: 130; cellHeight: 170
                model: PhilosophersModel {
                    id: modelOne;
                    tab: PhilosopherList;}
                delegate: philosopherGui
            }
        }
    }
}

