cmake_minimum_required( VERSION 3.15.0 )

project( WhipseeySaveManager VERSION 0.7.0 )

set_property( GLOBAL PROPERTY CXX_STANDARD 17 )
set_property( GLOBAL PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON )

set( WSM_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc )
set( WSM_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
set( WSM_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test )

# find_path( VDF_PARSER_INCLUDE_DIRS "vdf_parser.hpp" )
# find_path( SIMPLEINI_INCLUDE_DIRS "SimpleIni.h" )
# find_path( WINREG_INCLUDE_DIRS "winreg/WinReg.hpp" )

add_subdirectory( ${WSM_SRC_DIR} )

add_subdirectory( ${WSM_TEST_DIR} )
