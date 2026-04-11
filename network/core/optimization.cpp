#include "core/net.h"
#include <netinet/tcp.h>
#include "optimization.h"




void frame_optimization(int pub){
    int flag = 1;
    setsockopt(pub, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag)); 
    int buf = 4 * 1024 * 1024;
    setsockopt(pub, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf)); 
}