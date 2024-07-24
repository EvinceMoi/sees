add_requires("mpv", { system = true })

target("see-qt")
    add_rules("qt.widgetapp")

    add_includedirs(".") -- all include path start in src

    add_headerfiles("player/*.h")
    add_files("player/*.cpp")
	
    add_headerfiles("ui/*.h")
    add_files("ui/*.cpp")

    add_files("*.cpp")

    -- add files with Q_OBJECT meta (only for qt.moc)

    add_frameworks("QtCore", "QtGui", "QtWidgets", "QtOpenGLWidgets")
    -- add_frameworks("QtOpenGLWidgets")
	add_packages("mpv")
