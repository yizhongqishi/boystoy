#pragma once
#include<iostream>
#include <thread>
#include <stdio.h>
#include <iostream>
#include <winsock.h>
#include <WS2tcpip.h>

using namespace std;





class Sender {
public:
	Sender();
	void SendtoC(int x, int y, int z);
	void initialization(char* ip);
	void close();
	void SendtoC_PWM(long x, int y, int z);

private:
	int send_len = 0;
	char send_buf[100];
	int err;
	SOCKET s_server;
	SOCKADDR_IN server_addr;
	int send_x;
	int send_speed;
	int send_brake;
	bool init;
};