cmake_minimum_required(VERSION 3.10)

project(sandProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
                    src/libs/bmpLib
                    src/libs/argsParser
                    src/libs/sandModel
                    src/libs/customStringBuf
)

add_executable(sand
    src/main.cpp
    src/libs/bmpLib/methods.cpp
    src/libs/argsParser/methods.cpp
    src/libs/sandModel/methods.cpp
    src/libs/customStringBuf/methods.cpp
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(sand PRIVATE __DEBUG__)
endif()

if(CMAKE_BUILD_TYPE MATCHES MaxOpt)
    add_compile_options(-O3 -flto -funroll-loops)
endif()

if(CMAKE_BUILD_TYPE MATCHES MaxOpt+Debug)
    target_compile_definitions(sand PRIVATE __DEBUG__)
    add_compile_options(-O3 -flto -funroll-loops)
endif()
