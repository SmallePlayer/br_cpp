cmake_minimum_required(VERSION 3.10)
project(br_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED) 

include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(algoritm
    logick/search_path.cpp
    logick/brezenhem.cpp
    logick/a_star.cpp
)
add_executable(tcp_server
    network/tcp_server.cpp
    network/settings_socket.cpp
)
add_executable(tcp_client
    network/tcp_client.cpp
    network/settings_socket.cpp
)

add_executable(capture
    camera/capture.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
)

add_executable(send_frame
    network/send_frame.cpp
    network/settings_socket.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp

)

add_executable(reciv_frame
    network/reciv_frame.cpp
    network/settings_socket.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
)



target_link_libraries(send_frame PRIVATE ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(reciv_frame PRIVATE ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(capture PRIVATE ${OpenCV_LIBS})