import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2
import "./scripts.js" as JS

Felgo.FlickablePage {
    id: _signInPage
    navigationBarHidden: true
    Connections{
        target: _user
        function onLogin(){
            _signInPageProgress.indeterminate = false
            console.log("Log in user")
            _appLoader.state = "Home"
        }
        function onSignInFailed(message){
            _signInPageProgress.indeterminate = false
            let msg = JS.parseSignInSignUpErrorMessage(message)
            emailField.errorMessage = msg
            emailField.hasError = true
            passwordField.errorMessage = msg
            passwordField.hasError = true
        }
    }
    QC2.ProgressBar{
        id: _signInPageProgress
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        indeterminate: false
        visible: indeterminate
    }
    ColumnLayout{
        id: _col1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 10
        spacing: 42
        Item{
            Layout.fillWidth: true
            Layout.preferredHeight: _col2.implicitHeight
            Layout.alignment: Qt.AlignTop
            Layout.bottomMargin: 40
            ColumnLayout{
                id: _col2
                anchors.fill: parent
                spacing: 8
                Felgo.AppText{
                    id: _welcomeText
                    text: "Moyindo Vutuka"
                    font.pixelSize: 60
                    font.bold: true
                    font.family: "Times New Roman"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.alignment: Qt.AlignHCenter
                    Layout.topMargin: 42
                }
                Felgo.AppText{
                    id: _lbl2
                    text: "Lisolo ya bana kongo"
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 40
                    font.bold: false
                    font.family: "Times New Roman"
                    Layout.alignment: Qt.AlignHCenter
                }
            }
        }
        Item{
            id: _loginForm
            Layout.fillWidth: true
            Layout.preferredHeight: _col3.implicitHeight
            property string errorMessage
            ColumnLayout{
                id: _col3
                anchors.fill: parent
                spacing: 20
                ValidatedField {
                  id: emailField
                  label: qsTr("Email na yo")
                  placeholderText: qsTr("Kotisa email na yo")
                  Layout.fillWidth: true
                  textField.onActiveFocusChanged: {
                    hasError = !textField.activeFocus && !textField.acceptableInput
                  }
                  onFocusChanged: {
                      hasError = !textField.activeFocus && !textField.acceptableInput
                  }
                  textField.inputMode: textField.inputModeEmail
                }
                ValidatedField {
                  id: passwordField
                  Layout.fillWidth: true
                  //property bool isIncorrectPassword: false
                  textField.inputMode: textField.inputModePassword
                  label: qsTr("Mot de passe na yo")
                  placeholderText: qsTr("Kotisa Mot de passe")
                  onFocusChanged: {
                      hasError = false
                  }
                }
                Felgo.AppCheckBox {
                  id: rememberMe
                  width: parent.width
                  text: qsTr("Kanisa ngai")

                }
                Felgo.AppText{
                    text: "Obosani Mot de passe na yo?"
                    Layout.alignment: Qt.AlignRight
                    font.pixelSize: rememberMe.labelFontSize
                    font.underline: _hover.hovered
                    HoverHandler{
                        enabled: true
                        id: _hover
                    }
                }
            }
        }
        Felgo.AppButton{
            text: "Kota"
            Layout.alignment: Qt.AlignHCenter
            onClicked: {
                _signInPageProgress.indeterminate = true
                _user.signInUser(emailField.textField.text,passwordField.textField.text)
            }
        }
        Flow{
            spacing: 8
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
            Felgo.AppText{
                text: "Oza abone te?"
                Layout.alignment: Qt.AlignRight
                font.pixelSize: rememberMe.labelFontSize
                font.underline: _hover2.hovered
            }
            Felgo.AppText{
                text: "Fina Awa"
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
                        const compo = Qt.createComponent("SignUpPage.qml")
                        const sprite = compo.createObject(_signInPage)
                        _signInPage.navigationStack.push(sprite)
                    }
                }
            }
        }
    }
}
