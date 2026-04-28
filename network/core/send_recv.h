#ifndef SEND_RECV_H
#define SEND_RECV_H

void settings_udp_pub();
void settings_udp_sub(int sub);

void send_int(int pub, int &data);
void send_float(int pub, float &data);
void send_double(int pub, double &data);

int recv_int(int sub, int &data);
int recv_float(int sub, float &data);
int recv_double(int sub, double &data);



#endif