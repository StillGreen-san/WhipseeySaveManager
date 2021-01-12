cmake_minimum_required(VERSION 3.12.0)

set(TARGET_MAIN WhipseeySaveManager)
set(SOURCES_ADDITIONAL
	#conversionfunctions.cpp
)

add_executable(${TARGET_MAIN} ${TARGET_MAIN}.cpp)
target_include_directories(${TARGET_MAIN} PRIVATE ${WSM_INC_DIR})
target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${VDF_PARSER_INCLUDE_DIRS})
target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${SIMPLEINI_INCLUDE_DIRS})
# target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${WINREG_INCLUDE_DIRS})
target_sources(${TARGET_MAIN} PRIVATE ${SOURCES_ADDITIONAL})
