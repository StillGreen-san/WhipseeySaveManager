cmake_minimum_required(VERSION 3.15.0)

find_package(Catch2 CONFIG REQUIRED)

set( TEST_MAIN ${CMAKE_PROJECT_NAME}-Tests )
set( SOURCES_TEST
	#${WSM_TEST_DIR}/converter_test.cpp
)
set( SOURCES_ADDITIONAL
	#${WSM_SRC_DIR}/conversionfunctions.cpp
)

add_executable( ${TEST_MAIN} ${TEST_MAIN}.cpp )
target_include_directories( ${TEST_MAIN} PRIVATE ${WSM_INC_DIR} )
target_include_directories( ${TEST_MAIN} SYSTEM PRIVATE ${Catch2_INCLUDE_DIRS} )
#target_include_directories( ${TEST_MAIN} SYSTEM PRIVATE ${VDF_PARSER_INCLUDE_DIRS} )
#target_include_directories( ${TEST_MAIN} SYSTEM PRIVATE ${SIMPLEINI_INCLUDE_DIRS} )
#target_include_directories( ${TEST_MAIN} SYSTEM PRIVATE ${WINREG_INCLUDE_DIRS} )
target_sources( ${TEST_MAIN} PRIVATE ${SOURCES_TEST} ${SOURCES_ADDITIONAL} )
target_link_libraries( ${TEST_MAIN} PRIVATE Catch2::Catch2 )
set_property( TARGET ${TEST_MAIN} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
