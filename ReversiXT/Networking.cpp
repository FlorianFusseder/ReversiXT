#include "Networking.h"

namespace repu{



Networking::Networking()
{
}


Networking::~Networking()
{
#ifdef __unix__
	close(sock);
#elif _WIN32
	closesocket(sock);
	WSACleanup();
#endif
}


int Networking::mConnect(std::string ip, std::string port)
{
#ifdef _WIN32
	WSAStartup(wVersionRequested, &wsaData);
#endif


	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0) {
		std::cout << "err creating sock";
		return sock;
	}

	struct sockaddr_in server;


	memset(server.sin_zero, '\0', sizeof(server.sin_zero));
	server.sin_family = AF_INET;

	inet_pton(AF_INET, ip.c_str(), &(server.sin_addr));
	server.sin_port = htons(atoi(port.c_str()));

	int res = -1;
	repu::clock time;
	while (res < 0)
	{
		res = connect(sock, (struct sockaddr*)&server, sizeof(server));
		if (time.getAsSecs() > 3)
		{
			std::cout << "err: timeout when trying to connect" << std::endl;
			#ifdef __unix__
				close(sock);
			#elif _WIN32
				closesocket(sock);
				WSACleanup();
			#endif
			return res;
		}
	}
	if (res < 0){
		std::cout << "err: couldn't connect to server"<< std::endl;
		#ifdef __unix__
			close(sock);
		#elif _WIN32
			closesocket(sock);
			WSACleanup();
		#endif
		return res;
	}



	netwBuf groupdata;
	groupdata.setMsgtype(Networking::groupno);
	groupdata << char(10);



	int sendres = mSend(groupdata);
	return sendres;
}


int Networking::mSend(netwBuf buf)
{
	int res = 0;
	unsigned int total = 0;
	int left = buf.size();
	for (total = 0; total < buf.size(); total += res, left-=res)
	{
		res = send(sock, buf.buf()+total, left, 0);
		if (res < 0)
		{
			std::cout << "err: unable to send msg" << std::endl;
			//closesocket and quit
#ifdef __unix__
			close(sock);
#elif _WIN32
			closesocket(sock);
			WSACleanup();
#endif
			return res;
		}
		if (res == 0)
		{
			std::cout << "err: unable to send msg: connection was closed by remote" << std::endl;
			//closesocket and quit
#ifdef __unix__
			close(sock);
#elif _WIN32
			closesocket(sock);
			WSACleanup();
#endif
			return res;
		}
	}
	return res;
}

int Networking::mRecieve(netwBuf& out)
{
	out.clear();
	char *buff = new char[5];
	int res = 1;
		res = recv(sock, buff, 5, MSG_WAITALL);
		if (res < 0)
		{
			std::cout << "err: unable to recv msg head" << std::endl;
			//closesocket and quit
#ifdef __unix__
			close(sock);
#elif _WIN32
			closesocket(sock);
			WSACleanup();
#endif
			return res;
		}
		if (res == 0)
		{
			std::cout << "err: unable to recv msg head: connection was closed by remote" << std::endl;
			//closesocket and quit
#ifdef __unix__
			close(sock);
#elif _WIN32
			closesocket(sock);
			WSACleanup();
#endif
			return res;
		}

	out.setMsgtype(buff[0]);
	uint32_t mSize = 0;
	for (int i = 4, j = 0; i > 0; i--, j++)
	{
		mSize += (buff[i] & 0xFF) << (8 * j);
	}
	char *buff2 = new char[mSize];

	if (mSize > 0)
	{
		res = recv(sock, buff2, mSize, MSG_WAITALL);
		if (res < 0)
		{
			std::cout << "err: unable to recv msg" << std::endl;
			//closesocket and  quit
#ifdef __unix__
			close(sock);
#elif _WIN32
			closesocket(sock);
			WSACleanup();
#endif
			return res;
		}
		if (res == 0)
		{
			std::cout << "err: unable to recv msg: connection was closed by remote" << std::endl;
			//closesocket and  quit
#ifdef __unix__
			close(sock);
#elif _WIN32
			closesocket(sock);
			WSACleanup();
#endif
			return res;
		}

		for (unsigned int i = 0; i < mSize; i++)
		{
			out << buff2[i];
		}
	}


	delete[] buff;
	delete[] buff2;
	return res;
}

int Networking::Recieve2(netwBuf& buf, int& type, uint32_t& timelimit, int& depthlimit, ff::turnPos& turnPos, int& specialfield, int& playerno)
{
	netwBuf netwBuf;
	int res = mRecieve(netwBuf);		
	type = (int)netwBuf.buf()[0];
	switch (type)
	{
	case Networking::turn_prompt:
		timelimit = 0;
		for (int i = 8, j = 0; i > 4; i--, j++)
		{
			timelimit += (netwBuf.bufV()[i] & 0xFF) << (8 * j);
		}
		depthlimit = (int)netwBuf.buf()[9];
		break;
	case Networking::turn:
		turnPos.pos_X = 0;
		turnPos.pos_Y = 0;
		for (int i = 6, j = 0; i > 4; i--, j++)
		{
			turnPos.pos_X += (netwBuf.bufV()[i] & 0xFF) << (8 * j);
		}
		for (int i = 8, j = 0; i > 6; i--, j++)
		{
			turnPos.pos_Y += (netwBuf.bufV()[i] & 0xFF) << (8 * j);
		}
		specialfield = netwBuf.buf()[9];
		playerno = (int)netwBuf.buf()[10];
		break;
	case Networking::disquali:
		playerno = (int)netwBuf.buf()[5];
		break;
	}
	return res;
}

int Networking::sendTurn(ff::turnPos turn, int special)
{
	netwBuf turnbuf;
	turnbuf.setMsgtype(Networking::turn_answer);
	uint16_t x = turn.pos_X;
	uint16_t y = turn.pos_Y;
	turnbuf << x;
	turnbuf << y;
	turnbuf << (char)special;
	return mSend(turnbuf);
}

}