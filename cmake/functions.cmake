# Group files by their folder
function(GroupFiles fileGroup)
	if(NOT ${fileGroup} STREQUAL "") # Ensure the files list is not empty
		# Beautify fileGroup (HEADER_FILES -> Header Files)
		string(REPLACE "_" " " fileGroupName ${fileGroup})	# Replace underscores with spaces
		string(TOLOWER ${fileGroupName} fileGroupName)		# To lower
		set(finalFileGroupName)
		string(REGEX MATCHALL "([^ ]+)" fileGroupNameSplit ${fileGroupName}) # Split each word
		foreach(fileGroupNameWord ${fileGroupNameSplit})	# Set to upper the first letter of each word
			string(SUBSTRING ${fileGroupNameWord} 0 1 firstLetter)
			string(SUBSTRING ${fileGroupNameWord} 1 -1 otherLetters)
			string(TOUPPER ${firstLetter} firstLetter)
			if(finalFileGroupName)
				set(finalFileGroupName "${finalFileGroupName} ")
			endif()
			set(finalFileGroupName "${finalFileGroupName}${firstLetter}${otherLetters}")
		endforeach()

		# Put each file in the correct group
		foreach(currentFile ${${fileGroup}})
			set(folder ${currentFile})
			get_filename_component(filename ${folder} NAME) # Get the file name
			string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" folder ${folder})	# Get the folder relative to the current one
			string(REPLACE "${filename}" "" folder ${folder})					# Remove the file name
			set(groupName "${finalFileGroupName}")
			if(NOT folder STREQUAL "") # Parse the remaining directories hierarchy
				string(REGEX REPLACE "/+$" "" baseFolder ${folder}) # Remove trailing slash
				if(NOT baseFolder STREQUAL "")
					string(REPLACE "/" "\\" baseFolder ${baseFolder})	# Forward to backward slashes
					set(groupName "${groupName}\\${baseFolder}")		# Put together the entire group name
				endif()
			endif()
			source_group("${groupName}" FILES ${currentFile})		# Put the file in this group
		endforeach()
	endif()
endfunction()

function(GetRuntime var_name)
	if(${var_name})
		set(${var_name} "" PARENT_SCOPE) # First clear the variable in case we do not find the dll
		string(REPLACE ".lib" ".dll" dll_path ${${var_name}}) # First look for the dll in the same directory
		if(EXISTS ${dll_path})
			set(${var_name} ${dll_path} PARENT_SCOPE)
		else()
		# Else look for it in a sibling directory called bin
			get_filename_component(filename ${dll_path} NAME) # Get the file name
			get_filename_component(dir ${dll_path} PATH) # Directory of the .lib
			get_filename_component(dir ${dir} PATH) # /..
			set(dll_path ${dir}/bin/${filename})
			if(EXISTS ${dll_path})
				set(${var_name} ${dll_path} PARENT_SCOPE)
			endif()
		endif()
	endif()
endfunction()

function(InstallDependency export_lib)
	get_target_property(export_lib_path ${export_lib} IMPORTED_LOCATION_RELEASE)
	GetRuntime(export_lib_path)
	if(export_lib_path)
		install(FILES ${export_lib_path} DESTINATION release CONFIGURATIONS Release)
	endif()
		
	get_target_property(export_lib_path ${export_lib} IMPORTED_LOCATION_DEBUG)
	GetRuntime(export_lib_path)
	if(export_lib_path)
		install(FILES ${export_lib_path} DESTINATION debug CONFIGURATIONS Debug)
	endif()
		
	get_target_property(export_lib_path ${export_lib} IMPORTED_LOCATION)
	GetRuntime(export_lib_path)
	if(export_lib_path)
		install(FILES ${export_lib_path} DESTINATION release CONFIGURATIONS Release)
		install(FILES ${export_lib_path} DESTINATION debug CONFIGURATIONS Debug)
	endif()
endfunction()
