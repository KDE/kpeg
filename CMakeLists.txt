project(pege)

find_package(KDE4 REQUIRED)

add_subdirectory(themes)
add_subdirectory(icons)
add_subdirectory(src)

install(FILES pege.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
