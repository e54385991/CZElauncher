#pragma once
#ifndef _LIB_SOCKET_H
#define _LIB_SOCKET_H
#define FD_SETSIZE 1024
#include <windows.h>
#include <iostream>
#include <vector>
#include <process.h>

#pragma comment(lib,"ws2_32.lib")
namespace lib_Socket
{
#pragma region TCP�ͻ�����
	/*
	TCP������ָ��;
	@parameter ����Socket
	@parameter Socket����ֵ
	@parameter ���ջ�����
	@parameter ���ճ���(���4095)
	@parameter �������
	@parameter Ԥ�����
	*/
	typedef void(*pTCPEventFunc)(SOCKET nSock, unsigned int nIndex, void *pBuf, unsigned int BufLen, unsigned int ErrorCode, LPARAM lParam);
	//TCP��Ϣ��������
	struct TCPSOCKET
	{
		SOCKADDR_IN SocketAddr;
		SOCKET Socket;
		unsigned int Port;
		std::string ServerAddr;
		std::wstring ServerAddrW;
		pTCPEventFunc EventFunc;
	};
	//UDP��Ϣ��������
	struct UDPSOCKET
	{
		SOCKADDR_IN SocketAddr;
		SOCKET Socket;
		unsigned int Port;
		std::string ServerAddr;
		std::wstring ServerAddrW;
		pTCPEventFunc EventFunc;
	};
	//������Ϣ��������
	struct SOCKETERROR
	{
		std::string ErrorInfo;
		std::wstring ErrorInfoW;
		int ErrorCode;
	};
	//TCPȫ�ִ����߳�;
	static void __stdcall TCPThread();
	class TCPClient
	{
	public:
		TCPClient();
		~TCPClient();
		//TCP��ʼ��
		bool __fastcall Initialize();
		//TCP�ͷ�
		void __fastcall Release();
		/*
		����TCP����
		@parameter ��������ַ
		@parameter �������˿�
		*/
		bool __fastcall CreateTCP(std::string ServerAddr, unsigned int Port);
		/*
		����TCP����
		@parameter ��������ַ
		@parameter �������˿�
		*/
		bool __fastcall CreateTCP(std::wstring ServerAddr, unsigned int Port);
		/*
		�Ͽ�TCP����
		@parameter TCP�����׽���
		*/
		bool __fastcall DisConnect(SOCKET ClientSocket);
		/*
		��ѯTCP Socket
		*/
		std::vector<TCPSOCKET> __fastcall QueryTCPSocket();
		/*
		�˶Զ˷���TCP����
		@parameter TCP�����׽���
		@parameter TCP����
		*/
		void __fastcall SendData(SOCKET ClientSocket, void *pBuf, int BufLen);
		/*
		��������TCP����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter TCP����
		*/
		void __fastcall SendData(std::string ServerAddr, unsigned int Port, void *pBuf, int BufLen);
		/*
		��������TCP����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter TCP����
		*/
		void __fastcall SendData(std::wstring ServerAddr, unsigned int Port, void *pBuf, int BufLen);
		/*
		����TCP���մ�����
		@parameter TCP�����׽���
		@parameter ������
		*/
		void __fastcall SetFunc(SOCKET ClientSocket, pTCPEventFunc EventFunc);
		/*
		����TCP���մ�����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter ������
		*/
		void __fastcall SetFunc(std::string ServerAddr, unsigned int Port, pTCPEventFunc EventFunc);
		/*
		����TCP���մ�����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter ������
		*/
		void __fastcall SetFunc(std::wstring ServerAddr, unsigned int Port, pTCPEventFunc EventFunc);
		/*
		��ȡ������Ϣ
		*/
		SOCKETERROR __fastcall GetError();
	private:
		
	};
#pragma endregion

#pragma region UDP�ͻ�����
	static void __stdcall UDPThread();
	class UDPClient
	{
	public:
		UDPClient();
		~UDPClient();
		//��ʼ��UDP
		bool __fastcall Initialize();
		//�ͷ�UDP
		void __fastcall Release();
		//��ȡ������Ϣ
		SOCKETERROR __fastcall GetError();
		/*
		����TCP����
		@parameter ��������ַ
		@parameter �������˿�
		*/
		bool __fastcall CreateUDP(std::string ServerAddr, unsigned int Port);
		/*
		����TCP����
		@parameter ��������ַ
		@parameter �������˿�
		*/
		bool __fastcall CreateUDP(std::wstring ServerAddr, unsigned int Port);
		/*
		��ѯUDP Socket
		*/
		std::vector<UDPSOCKET> __fastcall QueryUDPSocket();
		/*
		�˶Զ˷���UDP����
		@parameter UDP�����׽���
		@parameter UDP����
		*/
		void __fastcall SendData(SOCKET ClientSocket, void *pBuf, int BufLen);
		/*
		��������UDP����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter UDP����
		*/
		void __fastcall SendData(std::string ServerAddr, unsigned int Port, void *pBuf, int BufLen);
		/*
		��������UDP����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter UDP����
		*/
		void __fastcall SendData(std::wstring ServerAddr, unsigned int Port, void *pBuf, int BufLen);
		/*
		����UDP���մ�����
		@parameter UDP�����׽���
		@parameter ������
		*/
		void __fastcall SetFunc(SOCKET ClientSocket, pTCPEventFunc EventFunc);
		/*
		����UDP���մ�����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter ������
		*/
		void __fastcall SetFunc(std::string ServerAddr, unsigned int Port, pTCPEventFunc EventFunc);
		/*
		����UDP���մ�����
		@parameter ��������ַ
		@parameter �������˿�
		@parameter ������
		*/
		void __fastcall SetFunc(std::wstring ServerAddr, unsigned int Port, pTCPEventFunc EventFunc);
	private:

	};
#pragma endregion
}

#endif