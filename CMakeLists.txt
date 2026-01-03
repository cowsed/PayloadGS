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
        Payload.qml
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


        assets/BaseJoint.qml
        assets/UpperArm.qml
        assets/ForeArm.qml
        assets/Wrist.qml

    RESOURCES
        assets/maps/goegap_2k.hdr
        assets/maps/textureData.png
        assets/meshes/cube_001_mesh.mesh
        assets/images/launch_logo.jpg
        assets/images/groundstation_icon.png
        assets/images/payload_icon.png
        assets/images/rocket_icon.png

        assets/images/compass_payload.png
        assets/images/compass_rocket.png
        assets/images/compass_northsouth.png
    NO_RESOURCE_TARGET_PATH
    SOURCES
    SOURCES telemetrylogparser.h telemetrylogparser.cpp
    RESOURCES assets/maps/armCameraTextureData.png
    RESOURCES assets/meshes/cube_001_mesh.mesh assets/meshes/cube_002_mesh.mesh assets/meshes/cube_003_mesh.mesh assets/meshes/cylinder_mesh.mesh
    RESOURCES assets/meshes/cube_mesh.mesh
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
