cmake_minimum_required(VERSION 3.12.0)

find_package(Catch2 CONFIG REQUIRED)

set(TARGET_MAIN testmain)
set(SOURCES_TEST
	#${WSM_TEST_DIR}/converter_test.cpp
)
set(SOURCES_ADDITIONAL
	#${WSM_SRC_DIR}/conversionfunctions.cpp
)

add_executable(${TARGET_MAIN} ${TARGET_MAIN}.cpp)
target_include_directories(${TARGET_MAIN} PRIVATE ${WSM_INC_DIR})
target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${Catch2_INCLUDE_DIRS})
#target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${VDF_PARSER_INCLUDE_DIRS})
#target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${SIMPLEINI_INCLUDE_DIRS})
#target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${WINREG_INCLUDE_DIRS})
target_sources(${TARGET_MAIN} PRIVATE ${SOURCES_TEST} ${SOURCES_ADDITIONAL})
target_link_libraries(${TARGET_MAIN} PRIVATE Catch2::Catch2)
