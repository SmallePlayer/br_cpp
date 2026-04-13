cmake_minimum_required(VERSION 3.10)
project(br_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED) 

include_directories(camera logick network)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(broker
    network/core/broker.cpp
    network/core/net.cpp
    network/core/time.cpp
    network/core/signals.cpp
)
add_executable(pub
    network/pub.cpp
    network/core/net.cpp
    network/core/send_recv.cpp
    network/core/time.cpp
    network/core/signals.cpp
)
add_executable(sub
    network/sub.cpp
    network/core/net.cpp
    network/core/send_recv.cpp
    network/core/time.cpp
    network/core/signals.cpp
)


add_executable(pub_frame
    network/pub_frame.cpp
    network/core/net.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
    network/core/optimization.cpp
    network/core/signals.cpp
)


add_executable(tcp_server
    network/tcp_server.cpp
    network/core/net.cpp
    network/core/time.cpp
    network/core/signals.cpp
)
add_executable(tcp_client
    network/tcp_client.cpp
    network/core/net.cpp
    network/core/time.cpp
    network/core/signals.cpp
)





add_executable(send_frame
    network/send_frame.cpp
    network/core/net.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
    network/core/signals.cpp

)

add_executable(reciv_frame
    network/reciv_frame.cpp
    network/core/net.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
    network/core/signals.cpp
)


add_executable(capture
    camera/capture.cpp
    camera/frames.cpp
    camera/jpeg_work.cpp
)

add_executable(algoritm
    logick/search_path.cpp
    logick/brezenhem.cpp
    logick/a_star.cpp
)


target_link_libraries(tcp_server PRIVATE Threads::Threads)
target_link_libraries(tcp_client PRIVATE Threads::Threads)

target_link_libraries(broker PRIVATE Threads::Threads)
target_link_libraries(pub PRIVATE Threads::Threads)
target_link_libraries(sub PRIVATE Threads::Threads)

target_link_libraries(pub_frame PRIVATE ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(send_frame PRIVATE ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(reciv_frame PRIVATE ${OpenCV_LIBS} Threads::Threads)
target_link_libraries(capture PRIVATE ${OpenCV_LIBS})