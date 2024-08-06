qt_policy(SET QTP0001 NEW)

qt6_add_library(qmlui STATIC)

file(GLOB_RECURSE QML_SOURCE
	RELATIVE
		${CMAKE_CURRENT_LIST_DIR}
		*.qml
)

qt6_add_qml_module(qmlui
	URI ui
	VERSION 1.0
	QML_FILES ${QML_SOURCE}
	NO_PLUGIN
)

# target_link_directories(qmlui
# 	PUBLIC
# 		${QT_QML_OUTPUT_DIRECTORY}
# )

# message(STATUS ">>> ${QT_QML_OUTPUT_DIRECTORY}")