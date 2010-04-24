project(kpeg)

include_directories(${KDE4_INCLUDES} ${KDEGAMES_INCLUDE_DIR})

set(kpeg_SRCS
	board.cpp
	hole.cpp
	main.cpp
	movement.cpp
	peg.cpp
	puzzle.cpp
	window.cpp
)

kde4_add_kcfg_files(kpeg_SRCS settings.kcfgc)

kde4_add_executable(kpeg ${kpeg_SRCS})

target_link_libraries(kpeg ${KDEGAMES_LIBRARY} ${KDE4_KDEUI_LIBS})

install(TARGETS kpeg ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kpeg.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(FILES pegeui.rc DESTINATION  ${DATA_INSTALL_DIR}/pege)
