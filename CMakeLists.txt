cmake_minimum_required(VERSION 3.10)
project(br_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED) 

include_directories(camera logick network)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(broker
    network/broker.cpp
    network/net.cpp
    network/time.cpp
)
add_executable(pub
    network/pub.cpp
    network/net.cpp
    network/time.cpp
)
add_executable(sub
    network/sub.cpp
    network/net.cpp
    network/time.cpp
)

add_executable(algoritm
    logick/search_path.cpp
    logick/brezenhem.cpp
    logick/a_star.cpp
)
add_executable(tcp_server
    network/tcp_server.cpp
    network/net.cpp
    network/time.cpp
)
add_executable(tcp_client
    network/tcp_client.cpp
    network/net.cpp
    network/time.cpp
)

add_executable(capture
    camera/capture.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
)

add_executable(send_frame
    network/send_frame.cpp
    network/net.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp

)

add_executable(reciv_frame
    network/reciv_frame.cpp
    network/net.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
)


target_link_libraries(tcp_server PRIVATE Threads::Threads)
target_link_libraries(tcp_client PRIVATE Threads::Threads)

target_link_libraries(broker PRIVATE Threads::Threads)
target_link_libraries(pub PRIVATE Threads::Threads)
target_link_libraries(sub PRIVATE Threads::Threads)

target_link_libraries(send_frame PRIVATE ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(reciv_frame PRIVATE ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(capture PRIVATE ${OpenCV_LIBS})