
#include "pch.h"
#include "Send.h"

Sender::Sender() : init(FALSE)
{

}
void Sender::initialization(char* ip= "192.168.2.103") {
	if (init) return;
	WORD w_req = MAKEWORD(2, 2);//�汾��
	WSADATA wsadata;
	err = WSAStartup(w_req, &wsadata);
	send_x = 0;
	send_speed = send_brake = 500;
	//���汾��
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wHighVersion) != 2) {
		cout << "�׽��ֿ�汾�Ų�����" << endl;
		WSACleanup();
	}
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "192.168.2.103", &server_addr.sin_addr.S_un.S_addr);
	server_addr.sin_port = htons(82);
	s_server = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(s_server, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		cout << "����������ʧ�ܣ�" << endl;
		WSACleanup();
	}
	else {
		cout << "���������ӳɹ���" << endl;
		init = TRUE;
	}
}

void Sender::SendtoC(int x, int speed, int brake){
	if (err || !init) {
		cout << "��ʼ�������޷����ͣ�";
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
		cout << "����ʧ�ܣ�" << endl;
	}
}

void Sender::SendtoC_PWM(long x, int speed, int brake) {
	if (err || !init) {
		cout << "��ʼ�������޷����ͣ�";
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
		cout << "����ʧ�ܣ�" << endl;
	}
}

void Sender::close() {
	//�ر��׽���
	closesocket(s_server);
	//�ͷ�DLL��Դ
	WSACleanup();
	init = FALSE;
	cout << "�������Ͽ����ӣ�" << endl;
}
