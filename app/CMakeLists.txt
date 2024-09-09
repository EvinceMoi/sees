add_subdirectory(qml)
add_subdirectory(player)
add_subdirectory(source)

add_executable(sees
	main.cpp
	Application.h Application.cpp
	resources.qrc
)

target_link_libraries(sees
	PRIVATE
		Qt6::Core
		Qt6::Quick
		Qt6::Qml
		player
		qmlui
		source
)
