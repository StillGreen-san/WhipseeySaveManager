cmake_minimum_required(VERSION 3.12.0)

find_package(raylib CONFIG REQUIRED)
include_directories(SYSTEM ${raylib_INCLUDE_DIRS})

find_path(RAYGUI_INCLUDE_DIRS "raygui.h")
include_directories(SYSTEM ${RAYGUI_INCLUDE_DIRS})

set(TARGET_MAIN WhipseeySaveManager)
set(SOURCES_ADDITIONAL
	#conversionfunctions.cpp
)

add_executable(${TARGET_MAIN} ${TARGET_MAIN}.cpp)
target_include_directories(${TARGET_MAIN} PRIVATE ${WSM_INC_DIR})
target_link_libraries(${TARGET_MAIN} PRIVATE ${raylib_LIBRARIES})
# target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${VDF_PARSER_INCLUDE_DIRS})
# target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${SIMPLEINI_INCLUDE_DIRS})
# target_include_directories(${TARGET_MAIN} SYSTEM PRIVATE ${WINREG_INCLUDE_DIRS})
target_sources(${TARGET_MAIN} PRIVATE ${SOURCES_ADDITIONAL})
