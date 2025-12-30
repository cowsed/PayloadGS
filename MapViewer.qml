import QtQuick
import QtLocation
import QtPositioning

Item {
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

    MapView {
        id: mapView
        property real payloadLatitude: 43.074835
        property real payloadLongitude: -77.669415

        property real stationLatitude: 43.083872
        property real stationLongitude: -77.675680

        anchors.fill: parent

        map.plugin: localhostSatPlugin
        map.center: QtPositioning.coordinate(payloadLatitude, payloadLongitude)
        map.zoomLevel: 12
        map.activeMapType: map.supportedMapTypes[7]
    }
}
