cmake_minimum_required(VERSION 3.16)

project(RadioController LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

find_package(Qt6 REQUIRED COMPONENTS Network)

add_executable(RadioController
  main.cpp
  proto_parser.h proto_parser.cpp
)
target_link_libraries(RadioController PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(RadioController PRIVATE Qt6::Network)

include(GNUInstallDirs)
install(TARGETS RadioController
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
