cmake_minimum_required(VERSION 3.20)

project(TextEditor)

set(CMAKE_CXX_STANDARD 17)

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

# Console application
add_executable(TextEditor
    main.cpp
)

# REST API server
add_executable(TextEditorServer
    server.cpp
)

target_include_directories(TextEditor PRIVATE include)
target_include_directories(TextEditorServer PRIVATE include)

target_link_libraries(TextEditor PRIVATE
    mongo::bsoncxx_shared
    mongo::mongocxx_shared
    Crow::Crow
)

target_link_libraries(TextEditorServer PRIVATE
    mongo::bsoncxx_shared
    mongo::mongocxx_shared
    Crow::Crow
)