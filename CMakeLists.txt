cmake_minimum_required(VERSION 3.14)

project(QtXmlParser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_executable(QtXmlParser
  main.cpp
  xmlelement.cpp xmlelement.hpp
  abstractxmlelement.cpp abstractxmlelement.hpp
  folder.cpp folder.hpp
  project.cpp project.hpp
  loaders.cpp loaders.hpp
)
target_link_libraries(QtXmlParser Qt${QT_VERSION_MAJOR}::Core)
