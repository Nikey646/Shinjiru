import QtQuick 2.0
import QtQuick.Controls 2.2

ScrollView {
  ScrollBar.vertical.policy: ScrollBar.AlwaysOn
  Flow {
    spacing: 5
    width: parentWidth

    Repeater {
      model: airingMedia

      Rectangle {
        width: 125
        height: 177

        Image {
          anchors.fill: parent
          source: modelData.coverImage
        }

        Rectangle {
          anchors.fill: parent
          visible: mouseArea.containsMouse
          color: "#000000"
          opacity: 0.65
        }

        Text {
          text: modelData.title
          anchors.fill: parent
          anchors.margins: 5
          visible: mouseArea.containsMouse
          wrapMode: Text.WordWrap
          color: "#ffffff"
          horizontalAlignment: Text.AlignHCenter
          verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
          id: mouseArea
          anchors.fill: parent
          hoverEnabled: true
          onDoubleClicked: {
            airingObject.showAnimePanel(modelData.id)
          }
        }
      }
    }
  }
}
