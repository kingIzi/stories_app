import Felgo 3.0 as Felgo
import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QC2


Felgo.Navigation {
    id: _navBar
    currentIndex: 0
    Component.onCompleted: {
        if (currentIndex === 0){
            _user.selectAllVideos()
        }
    }

    onCurrentIndexChanged: {
        if (currentIndex === 0){
            _user.selectAllVideos()
        }
        else if (currentIndex === 1){
            _user.selectFavoriteVideos()
        }
    }
    Felgo.NavigationItem {
        title: "Boyeyi bolamu"
        icon: Felgo.IconType.calculator
        Felgo.NavigationStack {
            HomePage{
                id: _homePage
            }
        }
    }

    Felgo.NavigationItem {
        title: "Ba video olingi"
        icon: Felgo.IconType.list

        Felgo.NavigationStack {
            FavoritesPage{
                id: _favoritesPage
            }
        }
    }

    Felgo.NavigationItem {
        title: "Ngai"
        icon: Felgo.IconType.cogs

        Felgo.NavigationStack {
            ProfilePage{
                id: _profilePage
            }
        }
    }
}
