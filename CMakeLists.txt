cmake_minimum_required( VERSION 3.17.0 )

project( WhipseeySaveManager LANGUAGES CXX )

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
set( CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE )

option( USE_CLANG_TIDY "USE_CLANG_TIDY" OFF )
if( USE_CLANG_TIDY )
	set( CMAKE_CXX_CLANG_TIDY clang-tidy )
endif( )

option( USE_CPPCHECK "USE_CPPCHECK" OFF )
if( USE_CPPCHECK )
	set( CMAKE_CXX_CPPCHECK cppcheck --suppress=missingInclude --enable=all ${CPPCHECK_USER_ARGS} )
endif( )

find_package( nana REQUIRED CONFIG NAMES nana unofficial-nana )
find_package( Catch2 CONFIG REQUIRED )
find_path( WINREG_INCLUDE_DIRS "winreg/WinReg.hpp" REQUIRED )
find_path( SIMPLEINI_INCLUDE_DIRS "ConvertUTF.c" REQUIRED )

set( WSM_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( WSM_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
set( WSM_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test )

set( LIB_TARGET ${CMAKE_PROJECT_NAME}-Lib )
set( MAIN_TARGET ${CMAKE_PROJECT_NAME} )
set( TEST_TARGET ${CMAKE_PROJECT_NAME}-Tests )

set( LIB_SOURCES
	${WSM_SRC_DIR}/system.cpp
	${WSM_SRC_DIR}/ini.cpp
)

add_library( ${LIB_TARGET} STATIC ${LIB_SOURCES} )
target_include_directories( ${LIB_TARGET} PUBLIC ${WSM_INC_DIR} )
target_include_directories( ${LIB_TARGET} SYSTEM PRIVATE ${WINREG_INCLUDE_DIRS} ${SIMPLEINI_INCLUDE_DIRS} )

set( MAIN_SOURCES
	${WSM_SRC_DIR}/${MAIN_TARGET}.cpp
	${WSM_SRC_DIR}/gui.cpp
	${WSM_SRC_DIR}/gui-impl-common.cpp
	${WSM_SRC_DIR}/gui-impl-files.cpp
	${WSM_SRC_DIR}/gui-impl-options.cpp
	${WSM_SRC_DIR}/gui-impl-cheats.cpp
)

add_executable( ${MAIN_TARGET} WIN32 ${MAIN_SOURCES} )
target_link_libraries( ${MAIN_TARGET} PRIVATE ${LIB_TARGET} unofficial::nana::nana )

set( TEST_SOURCES
	${WSM_TEST_DIR}/${TEST_TARGET}.cpp
	${WSM_TEST_DIR}/system-test.cpp
	${WSM_TEST_DIR}/ini-test.cpp
	${WSM_TEST_DIR}/ini-test-b.cpp
	${WSM_TEST_DIR}/testhelper.cpp
	${WSM_TEST_DIR}/types-test.cpp
)

add_executable( ${TEST_TARGET} ${TEST_SOURCES} )
target_link_libraries( ${TEST_TARGET} PRIVATE ${LIB_TARGET} Catch2::Catch2 )
add_custom_command( TARGET ${TEST_TARGET} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${WSM_TEST_DIR}/data/ $<TARGET_FILE_DIR:${TEST_TARGET}>/data
)
set_target_properties( ${TEST_TARGET} PROPERTIES ADDITIONAL_CLEAN_FILES $<TARGET_FILE_DIR:${TEST_TARGET}>/data/ )
