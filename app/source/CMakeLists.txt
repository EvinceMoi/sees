add_library(source STATIC
	Source.h Source.cpp
	MetaInfo.h MediaInfo.h
	Model.h Model.cpp
	SourceProvider.h SourceProvider.cpp
	DouyuProvider.h DouyuProvider.cpp
)

target_link_libraries(source
	PRIVATE
		Qt6::Core
		Qt6::Qml
		Qt6::Sql
)

target_include_directories(source
	PUBLIC
		.
)
