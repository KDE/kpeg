file(GLOB themes
	"*.svgz"
	"*.desktop"
	"*.png"
)
install(FILES ${themes} DESTINATION ${DATA_INSTALL_DIR}/pege/themes)
