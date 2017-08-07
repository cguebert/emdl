cmake_minimum_required(VERSION 3.5)

# External dependencies
find_package(Boost REQUIRED COMPONENTS filesystem system date_time)
find_package(fmt REQUIRED)
find_package(odil REQUIRED)

# Target name
set(target emdl)

# Set API export file and macro
string(TOUPPER ${target} target_upper)
set(export_file  "include/${target}/${target}_api.h")
set(export_macro "${target_upper}_API")

# Sources
file(GLOB_RECURSE Header_Files "*.h")
file(GLOB_RECURSE Source_Files "*.cpp")

# Group source files
GroupFiles(Header_Files)
GroupFiles(Source_Files)

# Create library
add_library(${target} ${Header_Files} ${Source_Files})

 # Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})

# Export library for downstream projects
export(TARGETS ${target} NAMESPACE ${META_PROJECT_NAME}:: FILE ${PROJECT_BINARY_DIR}/cmake/${target}/${target}-export.cmake)

# Create API export header
generate_export_header(${target} EXPORT_FILE_NAME ${export_file} EXPORT_MACRO_NAME ${export_macro})

# Include directories
target_include_directories(${target} 
	PRIVATE
	${CMAKE_SOURCE_DIR}/libs
	${CMAKE_CURRENT_BINARY_DIR}/include
	INTERFACE
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libs> 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> 
	$<INSTALL_INTERFACE:include>)

# Libraries
target_link_libraries(${target}
	PUBLIC
	libodil
	Boost::date_time
	Boost::system
	fmt::fmt)

# Compile definitions
target_compile_definitions(${target} PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})

# Compile options
target_compile_options(${target} PRIVATE ${DEFAULT_COMPILE_OPTIONS})

# Linker options
target_link_libraries(${target} PRIVATE ${DEFAULT_LINKER_OPTIONS})

# Project options
set_target_properties(${target} PROPERTIES FOLDER "Libraries")

if(WIN32)
	# We have to set _WIN32_WINNT for Asio
	if(${CMAKE_SYSTEM_VERSION} EQUAL 10) # Windows 10
		target_compile_definitions(${target} PRIVATE _WIN32_WINNT=0x0A00)
	elseif(${CMAKE_SYSTEM_VERSION} EQUAL 6.3 OR ${CMAKE_SYSTEM_VERSION} EQUAL 8.1) # Windows 8.1
		target_compile_definitions(${target} PRIVATE _WIN32_WINNT=0x0603)
	elseif(${CMAKE_SYSTEM_VERSION} EQUAL 6.2) # Windows 8
		target_compile_definitions(${target} PRIVATE _WIN32_WINNT=0x0602)
	elseif(${CMAKE_SYSTEM_VERSION} EQUAL 6.1) # Windows 7
		target_compile_definitions(${target} PRIVATE _WIN32_WINNT=0x0601)
	elseif(${CMAKE_SYSTEM_VERSION} EQUAL 6.0) # Windows Vista
		target_compile_definitions(${target} PRIVATE _WIN32_WINNT=0x0600)
	else() # Windows XP (5.1)
		target_compile_definitions(${target} PRIVATE _WIN32_WINNT=0x0501)
	endif()
endif()

# 
# Deployment
# 

# Library
install(TARGETS ${target}
	EXPORT  "${target}-export"            COMPONENT dev
	RUNTIME DESTINATION ${INSTALL_BIN}    COMPONENT runtime
	LIBRARY DESTINATION ${INSTALL_SHARED} COMPONENT runtime
	ARCHIVE DESTINATION ${INSTALL_LIB}    COMPONENT dev
)

# Header files
install(DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${INSTALL_INCLUDE} 
	COMPONENT dev
	FILES_MATCHING PATTERN "*.h"
)

# Generated header files
install(DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/include/${target} DESTINATION ${INSTALL_INCLUDE}
	COMPONENT dev
)

# CMake config
install(EXPORT ${target}-export
	NAMESPACE   ${META_PROJECT_NAME}::
	DESTINATION ${INSTALL_CMAKE}/${target}
	COMPONENT   dev
)

# Dlls for the package
if(WIN32 AND BUILD_SHARED_LIBS)
	install(TARGETS ${target} RUNTIME DESTINATION debug   CONFIGURATIONS Debug   COMPONENT runtime)
	install(TARGETS ${target} RUNTIME DESTINATION release CONFIGURATIONS Release COMPONENT runtime)
endif()
