project(kpeg)

if (NOT KDE4_FOUND)
  find_package(KDE4 REQUIRED)
  find_package(LibKDEGames REQUIRED)
  include(KDE4Defaults)
  include_directories(${KDE4_INCLUDES} ${KDEGAMES_INCLUDE_DIRS})
else (NOT KDE4_FOUND)
  set(KDEGAMES_LIBRARY kdegames)
endif (NOT KDE4_FOUND)

add_subdirectory(themes)
add_subdirectory(icons)
add_subdirectory(src)

install(PROGRAMS kpeg.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})