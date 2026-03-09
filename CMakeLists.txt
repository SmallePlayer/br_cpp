cmake_minimum_required(VERSION 3.10)
project(br_cpp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(main
    main.cpp
    logick/search_path.cpp
    logick/brezenhem.cpp
    logick/a_star.cpp
)
add_executable(tcp_server
    network/tcp_server.cpp
)
add_executable(tcp_client
    network/tcp_client.cpp
)