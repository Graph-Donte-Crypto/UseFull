#ifndef UF_CP_Socket_H
#define UF_CP_Socket_H

#include <immintrin.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <initializer_list>

#include "../Utils/Bytes.hpp"

//UseFull Cross Platfrom Socket module
//Version 1.0 alfa
//Make by Graph Don'te-Crypto

using namespace utils;

struct SocketConfig;
struct SocketData;
struct SystemSocket;
struct SocketSender;
struct SocketReceiver;
struct SocketGeneral;

enum class Protocol {
	TCP,
	UDP
};

enum class SocketType {
	Receiver,
	Sender
};

struct SocketConfig {
	Protocol protocol = Protocol::TCP;
	const char * port = nullptr;
	const char * ip   = nullptr;
};

struct SocketData {
	const char * value = nullptr;
	size_t command[8];
	size_t argc = 0;

	void set(std::initializer_list<size_t> ilist, const char * value) {
		this->value = value;
		argc = ilist.size();
		for (size_t i = 0; i < argc; i++) command[i] = *(ilist.begin() + i);
	}
	void flush() {
		value = nullptr;
		argc  = 0;
	}
	bool compare(size_t k, size_t v) {
		return k < argc && v == command[k];
	}
	bool compare(std::initializer_list<size_t> ilist) {
		if (ilist.size() > argc) return false;
		for (size_t i = 0; i < ilist.size(); i++) if (command[i] != *(ilist.begin() + i)) return false;
		return true;
	}
};

#ifdef WIN32
//WINSOCK2
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
struct SystemSocket {
	SocketConfig * config = nullptr;
	SocketType   * type   = nullptr;
	SocketData   * data   = nullptr;

	WSADATA wsaData;
	struct addrinfo * result = nullptr, * ptr = nullptr, hints;

	size_t buflen = 1024;
	char * buffer = nullptr;

	int iResult;

	SOCKET ListenSocket  = INVALID_SOCKET;
	SOCKET paired_socket = INVALID_SOCKET;

	int iSendResult;
	bool active    = false;
	bool connected = false;

	void set(SocketConfig * config, SocketType * type, SocketData * data) {
		this->config = config;
		this->type   = type;
		this->data   = data;
	}

	void checkSize(size_t size) {
		if (size >= buflen) {
			buflen *= 2;
			while (size >= buflen) buflen *= 2;
			delete[] buffer;
			buffer = new char[buflen];
		}
	}

	SystemSocket() {
		buffer = new char[buflen];
	}
	~SystemSocket() {
		close();
		delete[] buffer;
	}

	void accept() {
		if (active) {
			if (!connected) {
				//Ожидать подключение
				iResult = listen(ListenSocket, SOMAXCONN);
				if (iResult == SOCKET_ERROR) {
					printf("listen failed with error: %d\n", WSAGetLastError());
					closesocket(ListenSocket);
					WSACleanup();
					active = false;
					return;
				}
				// Accept a client socket
				paired_socket = ::accept(ListenSocket, nullptr, nullptr);
				if (paired_socket == INVALID_SOCKET) {
					printf("accept failed with error: %d\n", WSAGetLastError());
					closesocket(ListenSocket);
					WSACleanup();
					active = false;
					return;
				}
				connected = true;
			}
			else printf("ReceiverSocket already connected\n");
		}
		else printf("ReceiverSocket !active\n");
	}

	void init() {

		if (type == nullptr) {
			printf("Socket Type Fatal Error\n");
			return;
		}
		if (config == nullptr) {
			printf("Ip/port configuration isn't set\n");
			return;
		}

		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return;
		}

		ZeroMemory(&hints, sizeof(hints));

		if (config->protocol == Protocol::TCP) {
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
		}
		else {
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_protocol = IPPROTO_UDP;
		}

		if (*type == SocketType::Receiver) {
			hints.ai_family = AF_INET;
			hints.ai_flags  = AI_PASSIVE;
		}
		else {
			hints.ai_family = AF_UNSPEC;
		}

		if (*type == SocketType::Receiver) {
			//Возможно, для UDP нужно что-то другое
			iResult = getaddrinfo(config->ip, config->port, &hints, &result);
			if (iResult != 0) {
				printf("getaddrinfo failed with error: %d\n", iResult);
				WSACleanup();
				return;
			}

			// Create a SOCKET for connecting to server
			ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (ListenSocket == INVALID_SOCKET) {
				printf("socket failed with error: %i\n", WSAGetLastError());
				freeaddrinfo(result);
				WSACleanup();
				return;
			}

			// Setup the TCP listening socket
			iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				printf("bind failed with error: %d\n", WSAGetLastError());
				freeaddrinfo(result);
				closesocket(ListenSocket);
				WSACleanup();
				return;
			}

			freeaddrinfo(result);
		}
		active = true;
	}

	void receive() {
		if (active) {
			if (connected) {
				//Receive message with length
				iResult = recv(paired_socket, buffer, sizeof(size_t), 0);
				if (iResult <= 0) {
					if (iResult == 0) printf("Connection closed\n");
					else              printf("Failed receive length message: Error: %d\n", WSAGetLastError());
					disconnect();
					return;
				}

				//Read length
				size_t current_size = 0;
				char * ptr = fromBytes(&current_size, buffer, sizeof(size_t));

				//Read main message
				checkSize(current_size);
				iResult = recv(paired_socket, buffer, current_size, 0);
				if (iResult == SOCKET_ERROR) {
					printf("Failed receive main message: Error: %d\n", WSAGetLastError());
					disconnect();
					return;
				}
				printf("Was");
				//Get argc, commands and value
				ptr = fromBytes(&(data->argc), buffer, sizeof(size_t));
				for (size_t i = 0; i < data->argc; i++) {
					ptr = fromBytes(&(data->command[i]), ptr, sizeof(size_t));
				}
				buffer[current_size] = 0;
				data->value = ptr;
			}
			else printf("Socket isn't connected\n");
		}
		else printf("Socket isn't active\n");
	}

	void send() {
		if (active) {
			if (connected) {

				//make main message
				size_t current_size = sizeof(size_t) * (1 + data->argc) + strlen(data->value);
				checkSize(current_size);
				char * ptr = buffer;
				ptr = toBytes(ptr, &(data->argc), sizeof(size_t));
				for (size_t i = 0; i < data->argc; i++) {
					ptr = toBytes(ptr, &(data->command[i]), sizeof(size_t));
				}
				ptr = toBytes(ptr, data->value, strlen(data->value));

				//make message with length
				char temp[sizeof(size_t)];
				toBytes(temp, &current_size, sizeof(size_t));

				//send message with length
				printf("%lu", (unsigned long)current_size);
				iResult = ::send(paired_socket, temp, sizeof(size_t), 0);
				if (iResult == SOCKET_ERROR) {
					printf("Failed send length message: Error: %d\n", WSAGetLastError());
					disconnect();
					return;
				}

				//send main message
				iResult = ::send(paired_socket, buffer, current_size, 0);
				if (iResult == SOCKET_ERROR) {
					printf("Failed send main message: Error: %d\n", WSAGetLastError());
					disconnect();
					return;
				}
			}
			else printf("Socket isn't connected\n");
		}
		else printf("Socket isn't active\n");
	}

	void disconnect() {
		if (active) {
			if (connected) {
				if (config->protocol == Protocol::TCP) {
					iResult = shutdown(paired_socket, SD_SEND);
					if (iResult == SOCKET_ERROR) {
						printf("shutdown failed with error: %d\n", WSAGetLastError());
					}
				}
				closesocket(paired_socket);
				WSACleanup();
				connected = false;
			}
		}
	}

	void close() {
		if (active) {
			if (*type == SocketType::Receiver) closesocket(ListenSocket);
			disconnect();
		}
	}

	void connect() {
		if (active) {
			if (!connected) {
				iResult = getaddrinfo(config->ip, config->port, &hints, &result);

				if (iResult != 0) {
					printf("getaddrinfo failed with error: %d\n", iResult);
					WSACleanup();
					active = false;
					return;
				}

				for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
					paired_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
					if (paired_socket == INVALID_SOCKET) {
						printf("socket connection failed with error: %d\n", WSAGetLastError());
						WSACleanup();
						return;
					}

					iResult = ::connect(paired_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
					if (iResult == SOCKET_ERROR) {
						closesocket(paired_socket);
						paired_socket = INVALID_SOCKET;
						continue;
					}
					break;
				}

				freeaddrinfo(result);

				if (paired_socket == INVALID_SOCKET) {
					printf("Unable to connect to server!\n");
					WSACleanup();
					return;
				}
				connected = true;
				Sleep(5);
			}
			else printf("SenderSocket already connected!\n");
		}
		else printf("SenderSocket isn't active\n");
	}

};

#else
//Linux Socket
#endif

struct SocketReceiver {
	SocketType   type = SocketType::Receiver;
	SocketData   data;
	SocketConfig config;
	SystemSocket sys;

	size_t (*protocol) (SocketReceiver *, size_t) = nullptr;

	void _printInfo() {
		printf("SocketReceiver %lu\n    port = %s\n    SocketData = %s\n    Active & Connect = %i %i\n", (unsigned long)(unsigned long long)this, config.port, data.value, sys.active, sys.connected);
		for(size_t i = 0; i < data.argc; i++) printf("    command[%lu] = %lu\n", (unsigned long)i, (unsigned long)data.command[i]);
	}

	SocketReceiver(const char * port, Protocol protocol = Protocol::TCP) {
		config.port     = port;
		config.protocol = protocol;
		sys.set(&config, &type, &data);
		sys.init();
	}
	~SocketReceiver() {close();}

	void close()      {sys.close     ();}
	void accept()     {sys.accept    ();}
	void receive()    {sys.receive   ();}
	void disconnect() {sys.disconnect();}

};

struct SocketSender {
	SocketType   type = SocketType::Sender;
	SocketData   data;
	SocketConfig config;
	SystemSocket sys;

	void _printInfo() {
		printf("SocketSender %lu\n    ip = %s\n    port = %s\n    SocketData = %s\n    Active & Connect = %i %i\n", (unsigned long)(unsigned long long)this, config.ip, config.port, data.value, sys.active, sys.connected);
		for(size_t i = 0; i < data.argc; i++) printf("    command[%lu] = %lu\n", (unsigned long)i, (unsigned long)data.command[i]);
	}

	SocketSender(const char * ip, const char * port, Protocol protocol = Protocol::TCP) {
		config.ip       = ip;
		config.port     = port;
		config.protocol = protocol;
		sys.set(&config, &type, &data);
		sys.init();
	}
	~SocketSender() {close();}

	void send()       {sys.send      ();}
	void close()      {sys.close     ();}
	void connect()    {sys.connect   ();}
	void disconnect() {sys.disconnect();}

	void send(size_t command, const char * data){}
};

struct GenericSocket {
	SocketType   type;
	SocketData   data;
	SocketConfig config;
	SystemSocket sys;

	void _printInfo() {
		if (type == SocketType::Receiver) printf("GenericSocket %lu type = Receiver\n    ip = %s\n    port = %s\n    SocketData = %s\n    Active & Connect = %i %i\n", (unsigned long)(unsigned long long)this, config.ip, config.port, data.value, sys.active, sys.connected);
		else                              printf("GenericSocket %lu type = Sender  \n    ip = %s\n    port = %s\n    SocketData = %s\n    Active & Connect = %i %i\n", (unsigned long)(unsigned long long)this, config.ip, config.port, data.value, sys.active, sys.connected);
		for(size_t i = 0; i < data.argc; i++) printf("    command[%lu] = %lu\n", (unsigned long)i, (unsigned long)data.command[i]);
	}

	GenericSocket(const char * ip, const char * port, SocketType type, Protocol protocol = Protocol::TCP) {
		this->type      = type;
		config.ip       = ip;
		config.port     = port;
		config.protocol = protocol;
		sys.set(&config, &type, &data);
		sys.init();
	}
	~GenericSocket()  {close();}

	void send()       {sys.send      ();}
	void close()      {sys.close     ();}
	void accept()     {sys.accept    ();}
	void connect()    {sys.connect   ();}
	void receive()    {sys.receive   ();}
	void disconnect() {sys.disconnect();}

	void send(size_t command, const char * data){}
};

#endif
