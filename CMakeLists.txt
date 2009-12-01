cmake_minimum_required(VERSION 2.6)
project(pege)

find_package(KDE4 REQUIRED)
find_package(LibKDEGames REQUIRED)
include(KDE4Defaults)

add_subdirectory(themes)
add_subdirectory(icons)
add_subdirectory(src)

install(FILES pege.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})