cmake_minimum_required(VERSION 3.8)

set(query_craft_project_name QueryCraft)
option(QUERY_CRAFT_EXAMPLE "Add examples file as executable (on|off)" OFF)

project(${query_craft_project_name} LANGUAGES CXX)

file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

if(NOT TARGET TypeConverterApi)
    add_subdirectory(include/external/TypeConverterApi)
endif()

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
        include/external
)

target_link_libraries(${PROJECT_NAME} PUBLIC
        TypeConverterApi
)

target_compile_features(${query_craft_project_name} PUBLIC cxx_std_14)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)

install(DIRECTORY include
        DESTINATION include
)

if (${QUERY_CRAFT_EXAMPLE})
    add_subdirectory(example)
endif ()