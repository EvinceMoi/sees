add_subdirectory(qml)
add_subdirectory(player)

add_executable(sees
	main.cpp
)

target_link_libraries(sees
	PRIVATE
		Qt6::Core
		Qt6::Quick
		Qt6::Qml
		player
		qmlui
)