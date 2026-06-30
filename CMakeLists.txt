cmake_minimum_required(VERSION 3.20)

project(TextEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

add_executable(TextEditorServer
    server.cpp
)

target_include_directories(TextEditorServer PRIVATE include)

target_link_libraries(TextEditorServer PRIVATE
    $<IF:$<TARGET_EXISTS:mongo::bsoncxx_shared>,mongo::bsoncxx_shared,mongo::bsoncxx_static>
    $<IF:$<TARGET_EXISTS:mongo::mongocxx_shared>,mongo::mongocxx_shared,mongo::mongocxx_static>
    Crow::Crow
)