qt6_add_library(player STATIC)

target_sources(player
	PRIVATE
		MpvPlayer.h MpvPlayer.cpp
		MpvProps.h
)

target_include_directories(player
	PUBLIC
		.
)

target_link_libraries(player
	PUBLIC
		Qt6::Quick
		MpvQt::MpvQt
)
