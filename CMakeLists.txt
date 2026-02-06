cmake_minimum_required(VERSION 3.16)

project(PayloadGS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Quick3D)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(Qt6 REQUIRED COMPONENTS Charts)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Location)





qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appPayloadGS
    main.cpp
)





qt_add_qml_module(appPayloadGS
    URI PayloadGS
    VERSION 1.0
    QML_FILES
        Main.qml
        Payload3D.qml

        MapPage.qml
        MapViewer.qml
        ShellPage.qml
        ImageControl.qml
        ArmPage.qml
        ImagePreview.qml
        ShellCommandListItem.qml
        StatusIcons.qml
        TelemetryPage.qml
        TimeSinceThing.qml
        RadioPage.qml
        Pages/Telemetry/StorageGauges.qml
        Pages/Telemetry/TempPlot.qml
        MapIcon.qml
        CompassUI.qml

        assets/GhostMaterial.qml
        AngleSliderWithBox.qml

    RESOURCES
        assets/images/launch_logo.jpg
        assets/images/groundstation_icon.png
        assets/images/payload_icon.png
        assets/images/rocket_icon.png

        assets/images/compass_payload.png
        assets/images/compass_rocket.png
        assets/images/compass_northsouth.png
    NO_RESOURCE_TARGET_PATH
    SOURCES telemetrylogparser.h telemetrylogparser.cpp

    RESOURCES
        assets/goegap_2k.hdr

        QML_FILES
        RESOURCES assets/images/picture.png assets/images/no_picture.png
        RESOURCES assets/images/axes_hidden_icon.png assets/images/axes_shown_icon.png
        QML_FILES ControlPage.qml
RESOURCES assets/payload-models/maps/textureData.png assets/payload-models/maps/textureData13.png assets/payload-models/maps/textureData18.png assets/payload-models/meshes/baseJointMesh_mesh.mesh assets/payload-models/meshes/frameMesh_mesh.mesh assets/payload-models/meshes/leftWall_mesh.mesh assets/payload-models/meshes/lowrLinkMesh_mesh.mesh assets/payload-models/meshes/rightWall_mesh.mesh assets/payload-models/meshes/upperLinkMesh_mesh.mesh assets/payload-models/meshes/wristMesh_mesh.mesh
QML_FILES assets/payload-models/Payload.qml
RESOURCES assets/images/wall_shown_icon.png assets/images/wall_hidden_icon.png
RESOURCES assets/arrowshader.vert
RESOURCES assets/arrowshader.frag
QML_FILES OrientationArrow.qml
SOURCES radioparser.h radioparser.cpp
SOURCES lorasettings.h lorasettings.cpp
RESOURCES assets/images/ghost_shown_icon.png assets/images/ghost_hidden_icon.png
QML_FILES Pages/Telemetry/Gauge.qml
    )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPayloadGS PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPayloadGS
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPayloadGS
    PRIVATE
    Qt6::Quick
    Qt6::Quick3D
    Qt6::QuickControls2
    Qt::Charts
    Qt::Core
    Qt::Gui
    Qt::Location
)


include(GNUInstallDirs)
install(TARGETS appPayloadGS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
