import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2
import "./scripts.js" as JS



Felgo.FlickablePage{
    id: _signUpPage
    property bool allFieldsValid: nameField.isInputCorrect && emailField.isInputCorrect && passwordField.isInputCorrect && confirmPasswordField.isInputCorrect && termsOfServiceCheck.checked
    navigationBarHidden: true
    flickable.contentHeight: _col5.implicitHeight
    QC2.ProgressBar{
        id: _signUpPageProgress
        height: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        indeterminate: false
        visible: indeterminate
    }
    Connections{
        target: _user
        function onLogin(){
            _signUpPageProgress.indeterminate = false
            _appLoader.state = "Home"
        }
        function onSignUpFailed(message){
            let msg = JS.parseSignInSignUpErrorMessage(message)
            _signUpPageProgress.indeterminate = false
            nameField.errorMessage = msg
            nameField.hasError = true
            emailField.errorMessage = msg
            emailField.hasError = true
            passwordField.errorMessage = msg
            passwordField.hasError = msg
            confirmPasswordField.errorMessage = msg
            confirmPasswordField.hasError = true
        }
    }
    Item{
        id: _item3
        anchors.fill: parent
        ColumnLayout{
            id: _col5
            anchors.fill: _item3
            anchors.margins: 10
            spacing: 42
            Felgo.AppText{
                id: _welcomeMsg
                text: "Boyeyi Malamu"
                Layout.alignment: Qt.AlignHCenter
                font.pixelSize: 72
                Layout.topMargin: 20
            }

            ValidatedField {
                id: nameField
                Layout.fillWidth: true
                label: qsTr("Ba Kombo na bino")
                placeholderText: qsTr("koma kombo na yo")

                //value: _user.getFullName()
                // we only allow names composed by letters and spaces
                validator: RegExpValidator {
                    regExp: /[\w ]+/
                }

                // when the active focus is taken away from the textField we check if we need to display an error
                textField.onActiveFocusChanged: {
                    hasError = !textField.activeFocus && !textField.acceptableInput
                }
                onFocusChanged: {
                    hasError = !textField.activeFocus && !textField.acceptableInput
                }

                errorMessage: qsTr("Kombo ekoki kozala na makomi ya '@!{/' te.")
            }
            Felgo.TextFieldRow {
              id: phoneField
              Layout.fillWidth: true
              label: qsTr("Nimero ya tshombo")
              placeHolder: qsTr("Koma nimero na yo ya tshombo")
              Layout.preferredHeight: 40
              Layout.bottomMargin: 20
              // Only allow to enter dialable characters, display proper virtual keyboard on mobile
              textFieldItem.inputMethodHints: Qt.ImhDialableCharactersOnly
              textFieldItem.validator: RegExpValidator{regExp: /\d{1,11}(?:,\d{1,11})+$/}
            }

            Felgo.TextFieldRow {
              id: birthField
              Layout.fillWidth: true
              Layout.preferredHeight: 40
              label: qsTr("Mibu ya botama")
              placeHolder: qsTr("Fina awa")
              property date _date

              // instead of requiring the user to manually type a date we just show a native date picker when he/she taps on it.
              clickEnabled: true
              onClicked: nativeUtils.displayDatePicker()


              // When accepted, set displayed date to selected value
              Connections {
                target: nativeUtils
                onDatePickerFinished: {
                  if (accepted){
                      birthField._date = new Date(date)
                    birthField.value = birthField._date.toLocaleDateString(Locale.ShortFormat)
                  }
                }
              }
            }
            ValidatedField {
              id: emailField
              label: qsTr("Email na yo")
              placeholderText: qsTr("Koma email na yo")
                Layout.fillWidth: true
              // when the active focus is taken away from the textField we check if we need to display an error
              textField.onActiveFocusChanged: {
                hasError = !textField.activeFocus && !textField.acceptableInput
              }
              onFocusChanged: {
                  hasError = !textField.activeFocus && !textField.acceptableInput
              }
              errorMessage: qsTr("")

              // customize the text field to automatically discard invalid input and displays a "clear" text icon
              textField.inputMode: textField.inputModeEmail
            }
            ValidatedField {
              id: passwordField
              Layout.fillWidth: true
              // we display an error message if the password length is less than 6
              property bool isPasswordTooShort: textField.text.length > 0 && textField.text.length < 6

              // this hides characters by default
              textField.inputMode: textField.inputModePassword
              label: qsTr("Mot de passe na yo")
              placeholderText: qsTr("Koma mot de passe na yo")

              hasError: isPasswordTooShort
              onFocusChanged: {
                  hasError = isPasswordTooShort
              }
              errorMessage: qsTr("Mot de passe ezali mukuse")
            }
            ValidatedField {
              id: confirmPasswordField
              Layout.fillWidth: true
              property bool arePasswordsDifferent: passwordField.textField.text != confirmPasswordField.textField.text

              label: qsTr("Mot de passe lisusu")
              placeholderText: qsTr("Koma mot de passe na yo lisusu")
              textField.inputMode: textField.inputModePassword

              // we display an error message when the two password are different
              hasError: arePasswordsDifferent
              onFocusChanged: {
                  hasError = arePasswordsDifferent
              }
              errorMessage: qsTr("Mot de passe ekeseni")
            }

            Felgo.AppCheckBox {
              id: termsOfServiceCheck
              width: parent.width
              text: qsTr("Ndima")
              Layout.fillWidth: true
            }

            Felgo.AppText {
              text: qsTr("Read our <a href=\"https://felgo.com/doc/privacy-notes/\">privacy policy</a>")
              onLinkActivated: nativeUtils.openUrl(link)
              Layout.fillWidth: true
            }
            Felgo.AppButton {
              text: qsTr("Mikomisa")
              Layout.alignment: Qt.AlignHCenter
              width: parent.width * 1.07
              enabled: _signUpPage.allFieldsValid
              disabledColor: "lightgrey"
              fontBold: true
              onClicked: {
                  const signUp = {
                      "email": emailField.textField.text,
                      "password": passwordField.textField.text,
                      "telephone": phoneField.value,
                      "fullName": nameField.textField.text,
                      "dateOfBirth": birthField._date
                  };
                  _signUpPageProgress.indeterminate = true
                  _user.signUpUser(signUp)
              }
            }
            Flow{
                spacing: 8
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                Text{
                    text: "Oza deja abone?"
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: rememberMe.labelFontSize
                    font.underline: _hover.hovered
                }
                Text{
                    text: "Fina awa"
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: rememberMe.labelFontSize
                    color: _hover2.hovered ? "#FFD706" : "#000"
                    HoverHandler{
                        enabled: true
                        id: _hover2
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            _signUpPage.navigationStack.pop()
                        }
                    }
                }
            }
        }
    }
}







