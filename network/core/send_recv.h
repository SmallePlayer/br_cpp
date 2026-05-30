#ifndef SEND_RECV_H
#define SEND_RECV_H

void settings_udp_pub();
void settings_udp_sub(int sub);

void send_int(int pub, int &data);
void send_float(int pub, float &data);
void send_double(int pub, double &data);

void send_int_m(int sock, int value, struct sockaddr_in group_addr);
void send_float_m(int sock, float value, struct sockaddr_in group_addr);
void send_double_m(int sock, double value, struct sockaddr_in group_addr);

int recv_int(int sub, int &data);
float recv_float(int sub, float &data);
double recv_double(int sub, double &data);

int recv_int_m(int sock, int &data);
int recv_float_m(int sock, float &data);
int recv_double_m(int sock, double &data);




#endif