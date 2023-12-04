
#include "pch.h"
#include "Send.h"

Sender::Sender() : init(FALSE)
{

}
void Sender::initialization(char* ip= "192.168.2.103") {
	if (init) return;
	WORD w_req = MAKEWORD(2, 2);//版本号
	WSADATA wsadata;
	err = WSAStartup(w_req, &wsadata);
	send_x = 0;
	send_speed = send_brake = 500;
	//检测版本号
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "套接字库版本号不符！" << endl;
		WSACleanup();
	}
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "192.168.2.103", &server_addr.sin_addr.S_un.S_addr);
	server_addr.sin_port = htons(82);
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "服务器连接失败！" << endl;
		WSACleanup();
	}
	else {
		cout << "服务器连接成功！" << endl;
		init = TRUE;
	}
}

void Sender::SendtoC(int x, int speed, int brake){
	if (err || !init) {
		cout << "初始化错误，无法发送！";
		return;
	}
	if (send_x == x && send_speed == speed && send_brake == brake) return;
	if (x > 0) x = 1;
	else if (x < 0) x = -1;
	if (brake > 0) {
		speed = -1;
		brake = 1;
	}
	else speed = speed > 0;
	if (send_x == x && send_speed == speed && send_brake == brake) return;
	send_x = x;
	send_speed = speed;
	send_brake = brake;
	sprintf_s(send_buf, "%d,%d\n", x, speed);
	cout << send_buf;
	send_len = send(s_server, send_buf, strlen(send_buf), 0);
	if (send_len < 0) {
		cout << "发送失败！" << endl;
	}
}

void Sender::SendtoC_PWM(long x, int speed, int brake) {
	if (err || !init) {
		cout << "初始化错误，无法发送！";
		return;
	}
	brake = (brake > 0);
	if (send_x == x && send_speed == speed && send_brake == brake) return;
	if (x > 1500) x = 2000;
	else if (x < -1500) x = -2000;
	else x = 0;
	if (brake > 0) {
		speed *= -1;
	};
	if (send_x == x && send_speed == speed && send_brake == brake) return;
	send_x = x;
	send_speed = speed;
	send_brake = brake; 
	sprintf_s(send_buf, "%d,%d\n", x, speed);
	cout << send_buf;
	send_len = send(s_server, send_buf, strlen(send_buf), 0);
	if (send_len < 0) {
		cout << "发送失败！" << endl;
	}
}

void Sender::close() {
	//关闭套接字
	closesocket(s_server);
	//释放DLL资源
	WSACleanup();
	init = FALSE;
	cout << "服务器断开连接！" << endl;
}
