import Felgo 3.0 as Felgo
import QtQuick 2.15

Felgo.App {
    id: _root
    Component{
        id: _registration
        Felgo.NavigationStack {
            id: _registrationStack
            SignInPage{
                id: _loginPage
            }
        }
    }
    Component{
        id: _homeNavigation
        HomeNavigation{}
    }
    Loader{
        id: _appLoader
        anchors.fill: parent
        visible: true
        active: visible
        state: "Login"
        sourceComponent: _registration
        states: [
            State {
                name: "Login"
                PropertyChanges {
                    target: _appLoader
                    sourceComponent: _registration
                }
            },
            State {
                name: "Home"
                PropertyChanges {
                    target: _appLoader
                    sourceComponent: _homeNavigation
                }
            }
        ]
    }
}
