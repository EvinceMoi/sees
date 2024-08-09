add_library(source STATIC
	SourceProvider.h SourceProvider.cpp
	Douyu.h Douyu.cpp
)

target_link_libraries(source
	PRIVATE
		Qt6::Core
		Qt6::Qml
)
