#include "lib_Socket.h"
namespace lib_Socket
{
#pragma region TCP�ͻ�����
	static WSADATA WsaData, UdpWsaData;
	static SOCKETERROR ErrorInfo, UDPErrorInfo;
	//�Ƿ��ѳ�ʼ��
	static bool IsTCPStatupWSA = false, IsUDPStatupWSA = false;
	//TCP��Ϣ����
	static std::vector<TCPSOCKET> TcpInfo;
	//UDP��Ϣ����
	static std::vector<UDPSOCKET> UdpInfo;
	//�Ƿ��˳������߳�;
	static bool IsTCPThread = false, IsUDPThread = false;
	static fd_set TCPReadfds, UDPReadfds;
	static HANDLE hTCPThread = NULL, hUDPThread = NULL;
	TCPClient::TCPClient(){}
	TCPClient::~TCPClient() {}
	bool __fastcall TCPClient::Initialize()
	{
		if (!IsTCPStatupWSA) {
			WSAStartup(MAKEWORD(2, 2), &WsaData) == 0 ? IsTCPStatupWSA = true : IsTCPStatupWSA = false;
		}
		else {
			ErrorInfo.ErrorCode = 0xFF;
			ErrorInfo.ErrorInfo = "�Ѿ���ʼ����һ����,�����ظ���ʼ��!";
			ErrorInfo.ErrorInfoW = L"�Ѿ���ʼ����һ����,�����ظ���ʼ��!";
		}
		if (!IsTCPThread) {
			IsTCPThread = true;
			hTCPThread = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)TCPThread, NULL, NULL, NULL);
		}
		return IsTCPStatupWSA;
	}
	void __fastcall TCPClient::Release()
	{
		IsTCPThread = false;
		WaitForSingleObject(hTCPThread, INFINITE);
		for (unsigned int i = 0; i < TcpInfo.size(); i++) {
			closesocket(TcpInfo[i].Socket);
		}
		WSACleanup();
		TcpInfo.clear();
		std::vector<TCPSOCKET>().swap(TcpInfo);
	}
	bool __fastcall TCPClient::CreateTCP(std::string ServerAddr, unsigned int Port)
	{
		SOCKADDR_IN Addr = { 0 };
		Addr.sin_family = AF_INET;
		Addr.sin_port = htons(Port);
		hostent *pHost = gethostbyname(ServerAddr.c_str());
		if (!pHost)return false;
		Addr.sin_addr = *(in_addr*)pHost->h_addr;
		SOCKET Socket = socket(AF_INET, SOCK_STREAM, NULL);
		if (Socket == INVALID_SOCKET) {
			ErrorInfo.ErrorCode = GetLastError();
			ErrorInfo.ErrorInfo = "�����׽���ʧ��!";
			ErrorInfo.ErrorInfoW = L"�����׽���ʧ��!";
			return false;
		}
		if (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) == INVALID_SOCKET) {
			ErrorInfo.ErrorCode = GetLastError();
			ErrorInfo.ErrorInfo = "���ӷ�����ʧ��!";
			ErrorInfo.ErrorInfoW = L"���ӷ�����ʧ��!";
			return false;
		}
		TCPSOCKET tmp = { 0 };
		tmp.Socket = Socket;
		tmp.SocketAddr = Addr;
		tmp.Port = Port;
		tmp.ServerAddr = ServerAddr;
		wchar_t *pwchar = nullptr;
		size_t len = ServerAddr.size() * sizeof(wchar_t) + sizeof(wchar_t);
		try { pwchar = new wchar_t[len]; }
		catch (...) { return false; }
		ZeroMemory(pwchar, len);
		MultiByteToWideChar(CP_ACP, NULL, ServerAddr.c_str(), ServerAddr.length(), pwchar, len);
		tmp.ServerAddrW = pwchar;
		tmp.EventFunc = nullptr;
		delete[]pwchar;
		TcpInfo.push_back(tmp);
		return true;
	}
	bool __fastcall TCPClient::CreateTCP(std::wstring ServerAddr, unsigned int Port)
	{
		SOCKADDR_IN Addr = { 0 };
		Addr.sin_family = AF_INET;
		Addr.sin_port = htons(Port);
		char *pchar = nullptr;
		size_t len = ServerAddr.size() + sizeof(char);
		try { pchar = new char[len]; }
		catch (...) { return false; }
		ZeroMemory(pchar, len);
		WideCharToMultiByte(CP_ACP, NULL, ServerAddr.c_str(), ServerAddr.length(), pchar, len, NULL, NULL);
		hostent *pHost = gethostbyname(pchar);
		if (!pHost) { delete[]pchar; return false; }
		Addr.sin_addr = *(in_addr*)pHost->h_addr;
		SOCKET Socket = socket(AF_INET, SOCK_STREAM, NULL);
		if (Socket == INVALID_SOCKET) {
			ErrorInfo.ErrorCode = GetLastError();
			ErrorInfo.ErrorInfo = "�����׽���ʧ��!";
			ErrorInfo.ErrorInfoW = L"�����׽���ʧ��!";
			delete[]pchar;
			return false;
		}
		if (connect(Socket, (sockaddr*)&Addr, sizeof(Addr)) == INVALID_SOCKET) {
			ErrorInfo.ErrorCode = GetLastError();
			ErrorInfo.ErrorInfo = "���ӷ�����ʧ��!";
			ErrorInfo.ErrorInfoW = L"���ӷ�����ʧ��!";
			delete[]pchar;
			return false;
		}
		TCPSOCKET tmp = { 0 };
		tmp.Socket = Socket;
		tmp.SocketAddr = Addr;
		tmp.Port = Port;
		tmp.ServerAddrW = ServerAddr;
		tmp.ServerAddr = pchar;
		tmp.EventFunc = nullptr;
		TcpInfo.push_back(tmp);
		delete[]pchar;
		return true;
	}
	bool __fastcall TCPClient::DisConnect(SOCKET ClientSocket)
	{
		bool IsSucceed;
		closesocket(ClientSocket) == 0 ? IsSucceed = true : IsSucceed = false;
		return IsSucceed;
	}
	std::vector<TCPSOCKET> __fastcall TCPClient::QueryTCPSocket()
	{
		return TcpInfo;
	}
	void __fastcall TCPClient::SetFunc(std::string ServerAddr, unsigned int Port, pTCPEventFunc EventFunc)
	{
		for (unsigned int i = 0; i < TcpInfo.size(); i++) {
			if (strcmp(TcpInfo[i].ServerAddr.c_str(), ServerAddr.c_str()) == 0 && TcpInfo[i].Port == Port) {
				TcpInfo[i].EventFunc = EventFunc;
			}
		}
	}
	void __fastcall TCPClient::SetFunc(std::wstring ServerAddr, unsigned int Port, pTCPEventFunc EventFunc)
	{
		for (unsigned int i = 0; i < TcpInfo.size(); i++) {
			if (wcscmp(TcpInfo[i].ServerAddrW.c_str(), ServerAddr.c_str()) == 0 && TcpInfo[i].Port == Port) {
				TcpInfo[i].EventFunc = EventFunc;
			}
		}
	}
	void __fastcall TCPClient::SetFunc(SOCKET ClientSocket, pTCPEventFunc EventFunc)
	{
		for (unsigned int i = 0; i < TcpInfo.size(); i++) {
			if (TcpInfo[i].Socket == ClientSocket) {
				TcpInfo[i].EventFunc = EventFunc;
				break;
			}
		}
	}
	void __fastcall TCPClient::SendData(SOCKET ClientSocket, void *pBuf, int BufLen)
	{
		send(ClientSocket, (char*)pBuf, BufLen, NULL);
	}
	void __fastcall TCPClient::SendData(std::string ServerAddr, unsigned int Port, void *pBuf, int BufLen)
	{
		for (unsigned int i = 0; i < TcpInfo.size(); i++) {
			if (strcmp(TcpInfo[i].ServerAddr.c_str(), ServerAddr.c_str()) == 0 && TcpInfo[i].Port == Port) {
				send(TcpInfo[i].Socket, (char*)pBuf, BufLen, NULL);
			}
		}
	}
	void __fastcall TCPClient::SendData(std::wstring ServerAddr, unsigned int Port, void *pBuf, int BufLen)
	{
		for (unsigned int i = 0; i < TcpInfo.size(); i++) {
			if (wcscmp(TcpInfo[i].ServerAddrW.c_str(), ServerAddr.c_str()) == 0 && TcpInfo[i].Port == Port) {
				send(TcpInfo[i].Socket, (char*)pBuf, BufLen, NULL);
			}
		}
	}
	SOCKETERROR __fastcall TCPClient::GetError()
	{
		return ErrorInfo;
	}
	static void __stdcall TCPThread()
	{
		//������ջ������ڴ�;
		char *pBuf = nullptr;
		try { pBuf = new char[4096]; }
		catch (...) { MessageBoxA(NULL, "�ڴ�����ʧ��,TCP�����߳�����ʧ��!", NULL, NULL); }
		while (IsTCPThread) {
			//select��ѯ��������
			FD_ZERO(&TCPReadfds);
			//FD_ZERO(&Writefds);
			//FD_ZERO(&exceptfds);
			//��Socket����select��ѯ����������;
			for (unsigned int i = 0; i < TcpInfo.size(); i++) {
				FD_SET(TcpInfo[i].Socket, &TCPReadfds);
				//FD_SET(TcpInfo[i].Socket, &Writefds);
				//FD_SET(TcpInfo[i].Socket, &exceptfds);
			}
			timeval t = { 1,0 };
			//select ����ֻ���пɶ���ѯ,��Ϊֻ���н��մ���;
			int nRetAll = select(0, &TCPReadfds, NULL, NULL, &t);
			if (nRetAll > 0) {
				for (unsigned int i = 0; i < TcpInfo.size(); i++) {
					//�жϷ������Ƿ���TCP��Ϣ����.
					if (!FD_ISSET(TcpInfo[i].Socket, &TCPReadfds))continue;
					else {
						ZeroMemory(pBuf, 4096);
						int RetLen = recv(TcpInfo[i].Socket, pBuf, 4095, NULL);
						if (TcpInfo[i].EventFunc) {
							TcpInfo[i].EventFunc(TcpInfo[i].Socket, i, pBuf, RetLen, WSAGetLastError(), NULL);
						}
					}
				}
			}
			Sleep(5);
		}
		delete[]pBuf;
		_endthreadex(NULL);
	}
#pragma endregion

#pragma region UDP�ͻ�����

	UDPClient::UDPClient(){}
	UDPClient::~UDPClient(){}
	bool __fastcall UDPClient::Initialize()
	{
		if (!IsUDPStatupWSA) {
			WSAStartup(MAKEWORD(2, 2), &UdpWsaData) == 0 ? IsUDPStatupWSA = true : IsUDPStatupWSA = false;
		}
		else {
			UDPErrorInfo.ErrorCode = 0xFF;
			UDPErrorInfo.ErrorInfo = "�Ѿ���ʼ����һ����,�����ظ���ʼ��!";
			UDPErrorInfo.ErrorInfoW = L"�Ѿ���ʼ����һ����,�����ظ���ʼ��!";
		}
		if (!IsUDPThread) {
			IsUDPThread = true;
			hUDPThread = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)UDPThread, NULL, NULL, NULL);
		}
		return IsUDPStatupWSA;
	}
	void __fastcall UDPClient::Release()
	{
		IsUDPThread = false;
		WaitForSingleObject(hUDPThread, INFINITE);
		for (unsigned int i = 0; i < UdpInfo.size(); i++) {
			closesocket(UdpInfo[i].Socket);
		}
		WSACleanup();
		UdpInfo.clear();
		std::vector<UDPSOCKET>().swap(UdpInfo);
	}
	SOCKETERROR __fastcall UDPClient::GetError()
	{
		return UDPErrorInfo;
	}
	bool __fastcall UDPClient::CreateUDP(std::string ServerAddr, unsigned int Port)
	{
		SOCKADDR_IN Addr = { 0 };
		Addr.sin_family = AF_INET;
		Addr.sin_port = htons(Port);
		hostent *pHost = gethostbyname(ServerAddr.c_str());
		if (!pHost)return false;
		Addr.sin_addr = *(in_addr*)pHost->h_addr;
		SOCKET Socket = socket(AF_INET, SOCK_DGRAM, NULL);
		if (Socket == INVALID_SOCKET) {
			UDPErrorInfo.ErrorCode = GetLastError();
			UDPErrorInfo.ErrorInfo = "����UDPʧ��!";
			UDPErrorInfo.ErrorInfoW = L"����UDPʧ��!";
			return false;
		}
		UDPSOCKET tmp = { 0 };
		tmp.Socket = Socket;
		tmp.SocketAddr = Addr;
		tmp.Port = Port;
		tmp.ServerAddr = ServerAddr;
		wchar_t *pwchar = nullptr;
		size_t len = ServerAddr.size() * sizeof(wchar_t) + sizeof(wchar_t);
		try { pwchar = new wchar_t[len]; }
		catch (...) { return false; }
		ZeroMemory(pwchar, len);
		MultiByteToWideChar(CP_ACP, NULL, ServerAddr.c_str(), ServerAddr.length(), pwchar, len);
		tmp.ServerAddrW = pwchar;
		tmp.EventFunc = nullptr;
		delete[]pwchar;
		UdpInfo.push_back(tmp);
		return true;
	}
	bool __fastcall UDPClient::CreateUDP(std::wstring ServerAddr, unsigned int Port)
	{
		SOCKADDR_IN Addr = { 0 };
		Addr.sin_family = AF_INET;
		Addr.sin_port = htons(Port);
		char *pchar = nullptr;
		size_t len = ServerAddr.size() + sizeof(char);
		try { pchar = new char[len]; }
		catch (...) { return false; }
		ZeroMemory(pchar, len);
		WideCharToMultiByte(CP_ACP, NULL, ServerAddr.c_str(), ServerAddr.length(), pchar, len, NULL, NULL);
		hostent *pHost = gethostbyname(pchar);
		if (!pHost) { delete[]pchar; return false; }
		Addr.sin_addr = *(in_addr*)pHost->h_addr;
		SOCKET Socket = socket(AF_INET, SOCK_DGRAM, NULL);
		if (Socket == INVALID_SOCKET) {
			UDPErrorInfo.ErrorCode = GetLastError();
			UDPErrorInfo.ErrorInfo = "����UDPʧ��!";
			UDPErrorInfo.ErrorInfoW = L"����UDPʧ��!";
			delete[]pchar;
			return false;
		}
		UDPSOCKET tmp = { 0 };
		tmp.Socket = Socket;
		tmp.SocketAddr = Addr;
		tmp.Port = Port;
		tmp.ServerAddrW = ServerAddr;
		tmp.ServerAddr = pchar;
		tmp.EventFunc = nullptr;
		UdpInfo.push_back(tmp);
		delete[]pchar;
		return true;
	}
	void __fastcall UDPClient::SendData(SOCKET ClientSocket, void *pBuf, int BufLen)
	{
		for (unsigned int i = 0; i < UdpInfo.size(); i++) {
			if (UdpInfo[i].Socket == ClientSocket) {
				sendto(ClientSocket, (char*)pBuf, BufLen, NULL, (sockaddr*)&UdpInfo[i].SocketAddr, BufLen);
				break;
			}
		}
	}
	void __fastcall UDPClient::SendData(std::string ServerAddr, unsigned int Port, void *pBuf, int BufLen)
	{
		for (unsigned int i = 0; i < UdpInfo.size(); i++) {
			if (strcmp(ServerAddr.c_str(), UdpInfo[i].ServerAddr.c_str()) == 0 && UdpInfo[i].Port == Port) {
				sendto(UdpInfo[i].Socket, (char*)pBuf, BufLen, NULL, (sockaddr*)&UdpInfo[i].SocketAddr, BufLen);
			}
		}
	}
	void __fastcall UDPClient::SendData(std::wstring ServerAddr, unsigned int Port, void *pBuf, int BufLen)
	{
		for (unsigned int i = 0; i < UdpInfo.size(); i++) {
			if (wcscmp(ServerAddr.c_str(), UdpInfo[i].ServerAddrW.c_str()) == 0 && UdpInfo[i].Port == Port) {
				sendto(UdpInfo[i].Socket, (char*)pBuf, BufLen, NULL, (sockaddr*)&UdpInfo[i].SocketAddr, BufLen);
			}
		}
	}
	void __fastcall UDPClient::SetFunc(std::string ServerAddr, unsigned int Port, pTCPEventFunc EventFunc)
	{
		for (unsigned int i = 0; i < UdpInfo.size(); i++) {
			if (strcmp(UdpInfo[i].ServerAddr.c_str(), ServerAddr.c_str()) == 0 && UdpInfo[i].Port == Port) {
				UdpInfo[i].EventFunc = EventFunc;
			}
		}
	}
	void __fastcall UDPClient::SetFunc(std::wstring ServerAddr, unsigned int Port, pTCPEventFunc EventFunc)
	{
		for (unsigned int i = 0; i < UdpInfo.size(); i++) {
			if (wcscmp(UdpInfo[i].ServerAddrW.c_str(), ServerAddr.c_str()) == 0 && UdpInfo[i].Port == Port) {
				UdpInfo[i].EventFunc = EventFunc;
			}
		}
	}
	void __fastcall UDPClient::SetFunc(SOCKET ClientSocket, pTCPEventFunc EventFunc)
	{
		for (unsigned int i = 0; i < UdpInfo.size(); i++) {
			if (UdpInfo[i].Socket == ClientSocket) {
				UdpInfo[i].EventFunc = EventFunc;
				break;
			}
		}
	}
	std::vector<UDPSOCKET> __fastcall UDPClient::QueryUDPSocket()
	{
		return UdpInfo;
	}
	static void __stdcall UDPThread()
	{
		//������ջ������ڴ�;
		char *pBuf = nullptr;
		try { pBuf = new char[4096]; }
		catch (...) { MessageBoxA(NULL, "�ڴ�����ʧ��,UDP�����߳�����ʧ��!", NULL, NULL); }
		while (IsUDPThread) {
			//select��ѯ��������
			FD_ZERO(&UDPReadfds);
			//FD_ZERO(&Writefds);
			//FD_ZERO(&exceptfds);
			//��Socket����select��ѯ����������;
			for (unsigned int i = 0; i < UdpInfo.size(); i++) {
				FD_SET(UdpInfo[i].Socket, &UDPReadfds);
				//FD_SET(TcpInfo[i].Socket, &Writefds);
				//FD_SET(TcpInfo[i].Socket, &exceptfds);
			}
			timeval t = { 1,0 };
			//select ����ֻ���пɶ���ѯ,��Ϊֻ���н��մ���;
			int nRetAll = select(0, &UDPReadfds, NULL, NULL, &t);
			if (nRetAll > 0) {
				for (unsigned int i = 0; i < UdpInfo.size(); i++) {
					//�жϷ������Ƿ���TCP��Ϣ����.
					if (!FD_ISSET(UdpInfo[i].Socket, &UDPReadfds))continue;
					else {
						ZeroMemory(pBuf, 4096);
						int FromLen = sizeof(UdpInfo[i].SocketAddr);
						int RetLen = recvfrom(UdpInfo[i].Socket, pBuf, 4095, NULL, (sockaddr*)&UdpInfo[i].SocketAddr, &FromLen);
						if (UdpInfo[i].EventFunc) {
							UdpInfo[i].EventFunc(UdpInfo[i].Socket, i, pBuf, RetLen, WSAGetLastError(), NULL);
						}
					}
				}
			}
			Sleep(5);
		}
		delete[]pBuf;
		_endthreadex(NULL);
	}
#pragma endregion
}