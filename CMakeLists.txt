cmake_minimum_required(VERSION 3.9)
project(cmusWidgets)
set(CMAKE_CXX_FLAGS " -std=c++11")
set(CMAKE_PREFIX_PATH "/home/vancha94/Qt/5.10.0/gcc_64/")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package( Qt5Network REQUIRED )
find_package( Qt5Sql REQUIRED )
find_package( Qt5Xml REQUIRED )
find_package( Qt5PrintSupport REQUIRED )
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
set(SOURCE_FILES
cmustrayicon.h
main.cpp
icons.qrc
cmustrayicon.cpp
)
add_executable(cmusWidgets ${SOURCE_FILES}
cmustrayicon.h
main.cpp
icons.qrc
cmustrayicon.cpp
)
target_link_libraries(cmusWidgets Qt5::Widgets Qt5::Core Qt5::Network Qt5::Sql
${Boost_LIBRARIES} ${PROTOBUF_LIBRARY})
