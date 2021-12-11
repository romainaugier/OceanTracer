# ImGui

file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)

add_library(IMGUILIB ${HEADERS} ${SOURCES})

target_link_libraries(IMGUILIB GLEW::GLEW)

# Link against glfw/glew
if(WIN32)
	if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
		target_link_libraries(IMGUILIB ${CMAKE_SOURCE_DIR}/dependencies/glfw/lib-vc2019-x64/glfw3.lib)
	endif()
elseif(UNIX)
	if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
		target_link_libraries(IMGUILIB ${CMAKE_SOURCE_DIR}/dependencies/glfw/lib-linux/libglfw.so)
		# target_link_libraries(IMGUILIB glfw)
	endif()
endif()