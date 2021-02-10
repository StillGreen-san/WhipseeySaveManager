cmake_minimum_required( VERSION 3.16.0 )

project( WhipseeySaveManager )

if( NOT WIN32 )
	message( FATAL_ERROR "Windows Required" )
endif( )

if( MSVC )
	add_compile_options( /W4 /WX )
else( )
	add_compile_options( -Wall -Wextra -pedantic -Werror )
endif( )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

find_package( nana 1.8 REQUIRED CONFIG NAMES nana unofficial-nana )
find_package( Catch2 CONFIG REQUIRED )

set( WSM_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( WSM_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
set( WSM_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test )

set( MAIN_TARGET ${CMAKE_PROJECT_NAME} )
set( TEST_TARGET ${CMAKE_PROJECT_NAME}-Tests )

set( MAIN_SOURCES
	${WSM_SRC_DIR}/error.cpp
	${WSM_SRC_DIR}/data.cpp
)

add_executable( ${MAIN_TARGET} WIN32 ${WSM_SRC_DIR}/${MAIN_TARGET}.cpp )
target_include_directories( ${MAIN_TARGET} PRIVATE ${WSM_INC_DIR} )
target_link_libraries( ${MAIN_TARGET} PRIVATE unofficial::nana::nana )
target_sources( ${MAIN_TARGET} PRIVATE ${MAIN_SOURCES} )
set_property( TARGET ${MAIN_TARGET} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )

set( TEST_SOURCES
	${WSM_TEST_DIR}/data-test.cpp
)

add_executable( ${TEST_TARGET} ${WSM_TEST_DIR}/${TEST_TARGET}.cpp )
target_include_directories( ${TEST_TARGET} PRIVATE ${WSM_INC_DIR} )
target_include_directories( ${TEST_TARGET} SYSTEM PRIVATE ${Catch2_INCLUDE_DIRS} )
target_sources( ${TEST_TARGET} PRIVATE ${TEST_SOURCES} ${MAIN_SOURCES} )
target_link_libraries( ${TEST_TARGET} PRIVATE Catch2::Catch2 )
set_property( TARGET ${TEST_TARGET} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
