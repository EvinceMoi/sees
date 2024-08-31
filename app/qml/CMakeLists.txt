qt_policy(SET QTP0001 NEW)

qt_add_library(qmlui STATIC)

set(qml_files
	MainWindow.qml
	Player.qml
	Panel.qml
	IconButton.qml
	SearchBox.qml
	RoomCover.qml
	FollowView.qml
	SearchView.qml
	InfoView.qml
	Shortcuts.qml
)
set(singleton_qml_files
	Fonts.qml
	Icons.qml
)
set_source_files_properties(${singleton_qml_files}
	PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)
set(res_files
	res/font/CaskaydiaCove.ttf
)

qt_add_qml_module(qmlui
	URI ui
	VERSION 1.0
	OUTPUT_DIRECTORY ui
	QML_FILES ${qml_files} ${singleton_qml_files}
	RESOURCES ${res_files}
	NO_PLUGIN
)
