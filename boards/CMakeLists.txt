file(GLOB boards
	"*.desktop"
	"*.png"
	"*.svg"
)

install(FILES ${boards} DESTINATION ${DATA_INSTALL_DIR}/kpeg/boards)
