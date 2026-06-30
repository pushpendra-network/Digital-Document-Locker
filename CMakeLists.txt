cmake_minimum_required(VERSION 3.20)

project(TextEditor)

set(CMAKE_CXX_STANDARD 17)

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

add_executable(TextEditorServer
    server.cpp
)

target_include_directories(TextEditorServer PRIVATE include)

target_link_libraries(TextEditorServer PRIVATE
    mongo::bsoncxx_shared
    mongo::mongocxx_shared
    Crow::Crow
)
