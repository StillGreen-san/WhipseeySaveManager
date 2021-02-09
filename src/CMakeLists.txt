cmake_minimum_required(VERSION 3.15.0)

find_package( unofficial-nana 1.8 CONFIG REQUIRED )#vcpkg specific

set(SOURCES_ADDITIONAL
	error.cpp
)

add_executable( ${CMAKE_PROJECT_NAME} WIN32 ${CMAKE_PROJECT_NAME}.cpp )
target_include_directories( ${CMAKE_PROJECT_NAME} PRIVATE ${WSM_INC_DIR} )
target_link_libraries( ${CMAKE_PROJECT_NAME} PRIVATE unofficial::nana::nana )
# target_include_directories( ${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${VDF_PARSER_INCLUDE_DIRS} )
# target_include_directories( ${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${SIMPLEINI_INCLUDE_DIRS} )
# target_include_directories( ${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${WINREG_INCLUDE_DIRS} )
target_sources( ${CMAKE_PROJECT_NAME} PRIVATE ${SOURCES_ADDITIONAL} )
set_property( TARGET ${CMAKE_PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
