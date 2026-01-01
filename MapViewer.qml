import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtPositioning
import QtLocation

Item {
    id: mapComp
    required property geoCoordinate payloadCoordinate
    required property geoCoordinate stationCoordinate
    required property geoCoordinate rocketCoordinate

    Material.theme: Material.Light
    Plugin {
        id: onlineOSMPlugin
        name: 'osm'

        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "https://basemap.nationalmap.gov/arcgis/rest/services/USGSImageryOnly/MapServer"
        }
    }
    Plugin {
        id: offlineSatPlugin
        name: 'osm'
        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }

        PluginParameter {
            name: "osm.mapping.offline.directory"
            value: ':/maptiles/assets/offline-tiles/'
        }
        // PluginParameter {
        // name: "osm.mapping.host"
        // value: "file:///home/unknown/Clubs/Launch/Misc/PayloadGS/PayloadGS/assets/map-tiles/"
        //   }
    }

    Plugin {
        id: localhostSatPlugin
        name: 'osm'

        PluginParameter {
            name: "osm.mapping.host"
            value: "http://192.168.5.177:8000/"
        }
        PluginParameter {
            name: "osm.mapping.copyright"
            value: "RIT Launch Initiative"
        }
    }

    RowLayout {
        anchors.fill: parent

        MapView {
            id: mapView
            Layout.fillWidth: true
            Layout.fillHeight: true

            property int iconSize: 50

            map.plugin: localhostSatPlugin
            map.zoomLevel: 12
            map.activeMapType: map.supportedMapTypes[7]

            Component.onCompleted: {
                mapView.map.addMapItemGroup(mapItems)
                mapView.map.fitViewportToVisibleMapItems()
            }
            MapItemGroup {
                id: mapItems
                MapIcon {
                    id: groundStation
                    coordinates: mapComp.stationCoordinate
                    path: "qrc:/assets/images/groundstation_icon.png"
                }
                MapIcon {
                    id: payload
                    coordinates: mapComp.payloadCoordinate
                    path: "qrc:/assets/images/payload_icon.png"
                }
                MapIcon {
                    id: rocket
                    coordinates: mapComp.rocketCoordinate
                    path: "qrc:/assets/images/rocket_icon.png"
                }
            }
        }
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            Button {

                text: "+"
                onClicked: mapView.map.zoomLevel += 1
            }
            Button {
                text: "-"
                onClicked: mapView.map.zoomLevel -= 1
            }
            Button {
                text: "⌂"
                onClicked: mapView.map.fitViewportToVisibleMapItems()
            }
            Slider {
                Layout.alignment: Qt.AlignHCenter
                Layout.fillHeight: true
                from: 1
                value: mapView.map.zoomLevel
                to: mapView.map.maximumZoomLevel
                orientation: Qt.Vertical
                onValueChanged: mapView.map.zoomLevel = value
            }
        }
    }
}
