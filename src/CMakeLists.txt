cmake_minimum_required(VERSION 3.5)

project(weather-app VERSION 0.1 LANGUAGES CXX)

configure_file(config.ini ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 COMPONENTS Core Network REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Positioning)

include_directories(
    .
    Api
    Data
    Pages
    Settings
    Utils
    Widgets
)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui

        Api/ApiHandler.cpp Api/ApiHandler.h
        Api/GeocodingAPI.cpp Api/GeocodingAPI.h
        Api/WeatherAPI.cpp Api/WeatherAPI.h
        Api/DetailedWeatherAPI.cpp Api/DetailedWeatherAPI.h
        Api/Parser.cpp Api/Parser.h

        Data/Data.h
        Data/DetailedWeatherData.cpp Data/DetailedWeatherData.h
        Data/GeoLocationData.cpp Data/GeoLocationData.h
        Data/WeatherData.cpp Data/WeatherData.h

        Pages/Page.cpp Pages/Page.h
        Pages/HomePage.cpp Pages/HomePage.h
        Pages/DetailedWeatherPage.cpp Pages/DetailedWeatherPage.h
        Pages/ErrorPage.h Pages/ErrorPage.cpp
        Pages/CustomCompleter.cpp Pages/CustomCompleter.h

        Settings/Settings.cpp Settings/Settings.h
        Settings/SettingsDialog.cpp Settings/SettingsDialog.h
        Settings/WidgetsManager.h

        Utils/Serializable.h
        Utils/Serializer.cpp Utils/Serializer.h
        Utils/UserLocation.cpp Utils/UserLocation.h
        Utils/GraphDialog.cpp Utils/GraphDialog.h

        Widgets/BasicWidget.h Widgets/BasicWidget.cpp
        Widgets/WeatherWidget.cpp Widgets/WeatherWidget.h
        Widgets/ErrorWidget.h Widgets/ErrorWidget.cpp
        Widgets/WeatherWidget.cpp Widgets/WeatherWidget.h
        Widgets/DetailedPageWidgets.cpp Widgets/DetailedPageWidgets.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(weather-app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET weather-app APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(weather-app SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(weather-app
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(weather-app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(weather-app PRIVATE Qt6::Network)
target_link_libraries(weather-app PRIVATE Qt6::Positioning)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.weather-app)
endif()
set_target_properties(weather-app PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS weather-app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(weather-app)
endif()
