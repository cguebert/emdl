cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
find_package(odil REQUIRED)

set(target emdl)

file(GLOB_RECURSE Header_Files "*.h")
file(GLOB_RECURSE Source_Files "*.cpp")

# Regroup files by folder
GroupFiles(Header_Files)
GroupFiles(Source_Files)

add_library(${target} STATIC ${Header_Files} ${Source_Files})
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target}) # Create namespaced alias

target_include_directories(${target} 
	PUBLIC
	${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(${target}
	PUBLIC
	libodil
	Boost::boost
	PRIVATE
	fmt)

set_target_properties(${target} PROPERTIES FOLDER "Libraries")

if(WIN32)
	target_compile_definitions(${target} PRIVATE BUILDING_EMDL)
endif()
