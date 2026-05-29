cmake_minimum_required(VERSION 3.16)

project(RadioController LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Network)

find_package(PkgConfig REQUIRED)

# find the third party packages with pkgconfig
pkg_check_modules(GPIODCXX REQUIRED libgpiodcxx)


add_executable(RadioController
  main.cpp
  proto_parser.h proto_parser.cpp
  vendor/sx127x.c
  vendor/sx127x_custom_spi.cpp
  radio.cpp
  radio.hpp
  radio_server.hpp
  radio_server.cpp
  interrupt_waiter.h
  interrupt_waiter.cpp
)
target_include_directories(RadioController PRIVATE ./)
target_include_directories(RadioController PRIVATE vendor/sx127x/include)
target_link_libraries(RadioController PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(RadioController PRIVATE Qt6::Network)
# target_link_libraries(RadioController PRIVATE lgpio)

target_include_directories(RadioController PRIVATE ${libgpiod_INCLUDE_DIRS})
target_link_libraries(RadioController PRIVATE gpiodcxx)

include(GNUInstallDirs)
install(TARGETS RadioController
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
