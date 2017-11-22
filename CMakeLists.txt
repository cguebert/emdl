cmake_minimum_required(VERSION 3.5)

# Include cmake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(cmake/functions.cmake)
include(GenerateExportHeader)

# Meta information about the project
set(META_PROJECT_NAME "EMDL")

# Declare project
project(${META_PROJECT_NAME})

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Compiler settings and options
include(cmake/CompileOptions.cmake)

# Installation paths
if(UNIX AND SYSTEM_DIR_INSTALL)
	# Install into the system (/usr/bin or /usr/local/bin)
	set(INSTALL_ROOT      "share/${project}")       # /usr/[local]/share/<project>
	set(INSTALL_CMAKE     "share/${project}/cmake") # /usr/[local]/share/<project>/cmake
	set(INSTALL_EXAMPLES  "share/${project}")       # /usr/[local]/share/<project>
	set(INSTALL_DATA      "share/${project}")       # /usr/[local]/share/<project>
	set(INSTALL_BIN       "bin")                    # /usr/[local]/bin
	set(INSTALL_SHARED    "lib")                    # /usr/[local]/lib
	set(INSTALL_LIB       "lib")                    # /usr/[local]/lib
	set(INSTALL_INCLUDE   "include")                # /usr/[local]/include
	set(INSTALL_DOC       "share/doc/${project}")   # /usr/[local]/share/doc/<project>
	set(INSTALL_SHORTCUTS "share/applications")     # /usr/[local]/share/applications
	set(INSTALL_ICONS     "share/pixmaps")          # /usr/[local]/share/pixmaps
	set(INSTALL_INIT      "/etc/init")              # /etc/init (upstart init scripts)
else()
	# Install into local directory
	set(INSTALL_ROOT      ".")                      # ./
	set(INSTALL_CMAKE     "cmake")                  # ./lib/cmake
	set(INSTALL_EXAMPLES  ".")                      # ./
	set(INSTALL_DATA      ".")                      # ./
	set(INSTALL_BIN       "bin")                    # ./bin
	set(INSTALL_SHARED    "lib")                    # ./lib
	set(INSTALL_LIB       "lib")                    # ./lib
	set(INSTALL_INCLUDE   "include")                # ./include
	set(INSTALL_DOC       "doc")                    # ./doc
	set(INSTALL_SHORTCUTS "misc")                   # ./misc
	set(INSTALL_ICONS     "misc")                   # ./misc
	set(INSTALL_INIT      "misc")                   # ./misc
endif()

# 
# Project modules
# 

add_subdirectory("src")

# Install cmake find script for the project
install(FILES ${META_PROJECT_NAME}-config.cmake DESTINATION ${INSTALL_CMAKE} COMPONENT dev)

