# Src

# Main library (everything except main.cpp)
file(GLOB_RECURSE HEADERS *.h)
file(GLOB_RECURSE SOURCES *.cpp)
list(REMOVE_ITEM SOURCES "main.cpp")

add_library(MAINLIB ${HEADERS} ${SOURCES})

target_link_libraries(MAINLIB IMGUILIB)
target_link_libraries(MAINLIB TBB::tbb)

# Main executable
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} MAINLIB)

# Make sure we compile with CPP 17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Compile definition to avoid autolink with tbb on windows
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC __TBB_NO_IMPLICIT_LINKAGE)
endif()

foreach(binary_file ${GL_DLLS})
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        ARGS -E copy ${binary_file} ${CMAKE_CURRENT_BINARY_DIR}/Release
    )
endforeach()