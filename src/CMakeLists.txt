project(pege)

include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(pege_SRCS
	board.cpp
	hole.cpp
	main.cpp
	movement.cpp
	peg.cpp
	puzzle.cpp
	window.cpp
)

kde4_add_kcfg_files(pege_SRCS settings.kcfgc)

kde4_add_executable(pege ${pege_SRCS})

target_link_libraries(pege ${KDEGAMES_LIBRARY} ${KDE4_KDEUI_LIBS})

install(TARGETS pege ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES pege.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(FILES pegeui.rc DESTINATION  ${DATA_INSTALL_DIR}/pege)
