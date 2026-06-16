#ifndef SEND_RECV_H
#define SEND_RECV_H

#include "json.hpp"

using json = nlohmann::json;

void settings_udp_pub();
void settings_udp_sub(int sub);

void send_json(int pub, const json& data);

void send_udp_int(int pub, int &data);
void send_udp_float(int pub, float &data);
void send_udp_double(int pub, double &data);

void send_tcp_int(int pub, int &data);
void send_tcp_float(int pub, float &data);
void send_tcp_double(int pub, double &data);

void send_int_m(int sock, int value, struct sockaddr_in group_addr);
void send_float_m(int sock, float value, struct sockaddr_in group_addr);
void send_double_m(int sock, double value, struct sockaddr_in group_addr);

json recv_json(int sub);
int recv_tcp_int(int fd, int &data);
float recv_tcp_float(int fd, float &data);
double recv_tcp_double(int fd, double &data);

int recv_udp_int(int fd, int &data);
float recv_udp_float(int fd, float &data);
double recv_udp_double(int fd, double &data);

int recv_int_m(int sock, int &data);
int recv_float_m(int sock, float &data);
int recv_double_m(int sock, double &data);




#endif