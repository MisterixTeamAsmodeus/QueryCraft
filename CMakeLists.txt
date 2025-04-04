cmake_minimum_required(VERSION 3.10)

set(query_craft_project_name QueryCraft)
option(QUERY_CRAFT_EXAMPLE "Add examples file as executable (on|off)" OFF )

project(${query_craft_project_name} LANGUAGES CXX)

set(HEADER_FILES
        include/QueryCraft/querycraft.h
        include/QueryCraft/sqltable.h
        include/QueryCraft/table.h
        include/QueryCraft/conditiongroup.h
        include/QueryCraft/joincolumn.h
        include/QueryCraft/sortcolumn.h

        include/QueryCraft/enum/conditionviewtype.h
        include/QueryCraft/enum/logicaloperator.h

        include/QueryCraft/helper/tuplehelper.h
        include/QueryCraft/helper/typeconverter.h

        include/QueryCraft/operator/equalsoperator.h
        include/QueryCraft/operator/inoperator.h
        include/QueryCraft/operator/notinoperator.h
        include/QueryCraft/operator/ioperator.h
        include/QueryCraft/operator/lessoperator.h
        include/QueryCraft/operator/lessorequalsoperator.h
        include/QueryCraft/operator/moreoperator.h
        include/QueryCraft/operator/moreorequalsoperator.h
        include/QueryCraft/operator/notequalsoperator.h
        include/QueryCraft/operator/isnotoperator.h
        include/QueryCraft/operator/isoperator.h
)

set(SOURCE_FILES
        src/sqltable.cpp
        src/table.cpp
        src/conditiongroup.cpp
        src/joincolumn.cpp
        src/sortcolumn.cpp

        src/operator/equalsoperator.cpp
        src/operator/inoperator.cpp
        src/operator/notinoperator.cpp
        src/operator/lessoperator.cpp
        src/operator/lessorequalsoperator.cpp
        src/operator/moreoperator.cpp
        src/operator/moreorequalsoperator.cpp
        src/operator/notequalsoperator.cpp
        src/operator/isnotoperator.cpp
        src/operator/isoperator.cpp
)

add_definitions(-DQUERYCRAFT_LIBRARY)

add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_compile_features(${query_craft_project_name} PUBLIC cxx_std_14)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
)

if(${WIN32})
    set_target_properties(${PROJECT_NAME}
            PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif ()

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

if(${QUERY_CRAFT_EXAMPLE})
    add_subdirectory(example)

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/example)
endif ()