project(kpeg)

set(kpeg_SRCS
	main.cpp
	gameview.cpp
	window.cpp
)

kconfig_add_kcfg_files(kpeg_SRCS settings.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/*-apps-kpeg.png")
ecm_add_app_icon(kpeg_SRCS ICONS ${ICONS_SRCS})

add_executable(kpeg ${kpeg_SRCS})

target_link_libraries(kpeg
        KF5KDEGames
        KF5::XmlGui
        KF5::DBusAddons
        Qt5::Widgets
        Qt5::Svg
)

install(TARGETS kpeg ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kpegui.rc DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/kpeg)
install(FILES kpeg.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR})
install(DIRECTORY qml DESTINATION ${DATA_INSTALL_DIR}/kpeg)

