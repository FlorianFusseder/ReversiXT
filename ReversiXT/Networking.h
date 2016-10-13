#pragma once

#ifdef __unix__ //maybe use __linux__ (?)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <iostream>
#include <vector>
#include <stdint.h>
#include <cstring>

#include "clock.h"
#include "Spieler.h"


namespace repu{



	class netwBuf
	{
		uint8_t m_msgtype;
		uint32_t m_len;
		std::vector<char> m_buf;
		void setLen(){
			for (int i = sizeof(uint32_t) * 8 - 8, k = 1; i >= 0; i -= 8, k++){
				m_buf[k] = (m_len >> i);
			}
		}
	public:
		netwBuf(){
			m_len = 0;
			m_msgtype = 0;
			for (int i = 0; i < 5; i++){
				m_buf.push_back(0);
			}
		}
		netwBuf(uint8_t Msgtype){
			m_msgtype = Msgtype;
			m_len = 0;
			m_buf.push_back(m_msgtype);
			for (int i = sizeof(uint32_t) * 8 - 8; i >= 0; i -= 8){
				m_buf.push_back(m_len >> i);
			}
		}
		~netwBuf(){}
		void operator << (uint32_t m_int) {
			for (int i = sizeof(uint32_t) * 8 - 8; i >= 0; i -= 8){
				m_buf.push_back(m_int >> i);
				m_len++;
			}
			setLen();
		}
		void operator << (uint16_t m_int) {
			for (int i = sizeof(uint16_t) * 8 - 8; i >= 0; i -= 8){
				m_buf.push_back(m_int >> i);
				m_len++;
			}
			setLen();
		}
		void operator << (char m_char) {
			m_buf.push_back(m_char);
			m_len++;
			setLen();
		}
		void operator << (char* m_char) {
			for (int i = 0; m_char[i] != NULL; i++){
				m_buf.push_back(m_char[i]);
				m_len++;
			}
			setLen();
		}
		void setMsgtype(uint8_t Msgtype){ m_msgtype = Msgtype; 	m_buf[0] = m_msgtype; };
		const char * buf(){
			return &m_buf[0];
		}
		std::vector<char> bufV(){
			return m_buf;
		}
		unsigned int size(){ return m_buf.size(); }
		void clear(){
			m_buf.clear();
			m_buf.push_back(m_msgtype);
			for (int i = sizeof(uint32_t) * 8 - 8; i >= 0; i -= 8){
				m_buf.push_back(m_len >> i);
			}
			m_len = 0;
			setLen();
		}

	};

class Networking
	{
#ifdef __unix__
		int sock;
#elif _WIN32
		SOCKET sock;

		WORD wVersionRequested = MAKEWORD(2, 2);
		WSADATA wsaData;

#endif

	public:
		Networking();
		~Networking();

		enum msgtype{
			groupno = 1,
			map = 2,
			playerno,
			turn_prompt,
			turn_answer,
			turn,
			disquali,
			end_phase1,
			end_phase2
		};

		int mConnect(std::string ip, std::string port);
		int mSend(netwBuf buf);
		int mRecieve(netwBuf& out);

		int sendTurn(ff::turnPos turn, int special);
		////////////////////////////////////////////////////////////////////////////////
		/// \brief RecieveMethod with multiple options
		///
		/// \param		buf				Reference of a netwBuf
		/// \param		type			Reference of Messagetype, REQUIRED
		/// \param		timelimit		Reference of timelimit
		/// \param		depthlimit		Reference of depthlimit
		/// \param		turnPos			Reference of Coords for a turn;
		/// \param		specialfield	Reference of char of specialfield
		/// \param		playerno		Reference of number of player executing turn
		///
		////////////////////////////////////////////////////////////////////////////////
		int Recieve2(netwBuf& buf,int& type, uint32_t& timelimit, int& depthlimit, ff::turnPos& turnPos, int& specialfield, int& playerno);

	};

}
