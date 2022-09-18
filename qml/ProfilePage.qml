import QtQuick 2.6
import Felgo 3.0

FlickablePage {
  id: root

  title: qsTr("Profile")
  flickable.contentHeight: content.height

  // Here we are just checking that those items are not blank
  property bool areAllFieldsPopulated: isNotBlank(usernameField.value) && isNotBlank(birthField.value) && isNotBlank(phoneField.value)

  function isNotBlank(value) {
    return value.trim(" ") !== ""
  }

  rightBarItem: TextButtonBarItem {
    text: qsTr("Submit")
    //visible: areAllFieldsPopulated
    //onClicked: NativeDialog.confirm(qsTr("Success"), qsTr("Profile properties updated"))
    onClicked: {
        const profile = {
            "avatar": avatar.source,
            "email": usernameField.value,
            "fullName": nameField.value,
            "telephone": phoneField.value
        };
        _user.updateUserProfile(profile)
    }
  }

  Column {
    id: content
    topPadding: dp(Theme.contentPadding)
    anchors { left: parent.left; right: parent.right; }

    Item {
      height: avatar.height + dp(Theme.contentPadding)
      width: parent.width

      RoundedImage {
        id: avatar
        fillMode: AppImage.PreserveAspectCrop
        width: height
        height: dp(150)
        anchors.horizontalCenter: parent.horizontalCenter
        radius: width / 2
        source: _user.getAvatar()
        MouseArea {
          anchors.fill: parent
          // displays a native image picker for each platform
          onClicked: nativeUtils.displayImagePicker(qsTr("Pick profile picture"))
        }

        Connections {
          target: nativeUtils

          // here we get notified that our image is ready and we display it
          onImagePickerFinished: {
            if(accepted) {
                avatar.source = path
            }
          }
        }

        // Dark overlay with a camera icon
        Rectangle {
          anchors.fill: parent
          radius: width / 2
          color: "#33000000"

          Icon {
            anchors.centerIn: parent
            size: parent.height / 4
            icon: IconType.camera
            color: "white"
          }
        }
      }
    }

    // TextFieldRow contains a AppLabel and a AppTextField wrapped in a SimpleRow
    TextFieldRow {
      id: usernameField
      labelWidth: dp(74)
      label: qsTr("Username")
      value: _user.getEmail()
      placeHolder: qsTr("Enter your username")
      textFieldItem.inputMode: textFieldItem.inputModeUsername
    }

    TextFieldRow {
        id: nameField
        labelWidth: dp(74)
        label: qsTr("Full name")
        value: _user.getFullName()
        placeHolder: qsTr("Enter your username")
        textFieldItem.inputMode: textFieldItem.inputModeUsername
    }

    TextFieldRow {
      id: phoneField
      labelWidth: usernameField.labelWidth
      label: qsTr("Telephone")
      value: _user.getTelephone()
      placeHolder: qsTr("Enter your phone number")

      // Only allow to enter dialable characters, display proper virtual keyboard on mobile
      textFieldItem.inputMethodHints: Qt.ImhDialableCharactersOnly
      textFieldItem.validator: RegExpValidator{regExp: /^[0-9,/+ ]+$/}
    }

    TextFieldRow {
      id: birthField
      labelWidth: usernameField.labelWidth
      label: qsTr("Birth Date")
      placeHolder: qsTr("Enter your birth date")
      value: _user.getDateOfBirth()
      // instead of requiring the user to manually type a date we just show a native date picker when he/she taps on it.
      clickEnabled: true
      enabled: false
      onClicked: nativeUtils.displayDatePicker()

      // When accepted, set displayed date to selected value
      Connections {
        target: nativeUtils
        onDatePickerFinished: {
          if (accepted){
            birthField.value = new Date(date).toLocaleDateString(Locale.ShortFormat)
          }
        }
      }
    }
  }
}
