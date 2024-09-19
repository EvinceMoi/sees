#
# SPDX-FileCopyrightText: 2023 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause
#

add_library(MpvQt)
add_library(MpvQt::MpvQt ALIAS MpvQt)

set_target_properties(MpvQt PROPERTIES
    VERSION     ${MPVQT_VERSION}
    SOVERSION   ${MPVQT_SOVERSION}
    EXPORT_NAME MpvQt
)

target_sources(MpvQt
    PRIVATE
        mpvabstractitem.h mpvabstractitem.cpp
        mpvcontroller.h mpvcontroller.cpp
        mpvrenderer.h mpvrenderer.cpp
)

include(GenerateExportHeader)
generate_export_header(MpvQt
    BASE_NAME MpvQt
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/mpvqt_export.h
)
target_include_directories(MpvQt
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${PROJECT_BINARY_DIR}
)

# ecm_generate_export_header(MpvQt
#     BASE_NAME MpvQt
#     VERSION 1.0.0
#     USE_VERSION_HEADER
# )

# target_include_directories(MpvQt
#     INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/MpvQt>"
# )

target_link_libraries(MpvQt
    PRIVATE
        Qt6::Quick
        Libmpv::Libmpv
)

# ecm_generate_headers(MpvQt_HEADERS
#     HEADER_NAMES MpvAbstractItem
#                  MpvController
#     REQUIRED_HEADERS MpvQt_HEADERS
# )
