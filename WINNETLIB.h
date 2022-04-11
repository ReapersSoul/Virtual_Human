#pragma once
#define DllExport   __declspec( dllexport )
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")


#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <fstream>
#include <filesystem>
#include <map>



class DllExport Network {
public:
	class DllExport Socket {
		std::string Name;
		SOCKET sock;
	public:

		std::string GetName();
		void SetName(std::string name);

		Socket(SOCKET _sock, std::string YourName, bool SendFirst);
		Socket();
		~Socket();

		//gen functs
		bool Setup(SOCKET _sock);
		bool ShutDown();

		//utility functs
		char* addToEnd(char* first, int firstsize, char* second, int secondsize);

		char* addNullToEnd(char* src, int size);

		char* PullPart(char* src, int start, int bytesToPull);

		//send recv functs
		bool  SendData(char* data, int size);

		template<typename T>
		bool SendDataT(T t) {
			return SendData(reinterpret_cast<char*>(&t), sizeof(T));
		}

		template<>
		bool SendDataT<std::string>(std::string s);

		template<>
		bool SendDataT<std::wstring>(std::wstring s);


		bool RecvData(char** data);

		template<typename T>
		bool RecvDataT(T* item) {
			char* data = nullptr;

			if (!RecvData(&data)) {
				return false;
			}

			T* ret = reinterpret_cast<T*>(data);

			*item = *ret;
			return true;
		}

		template<>
		bool RecvDataT<std::string>(std::string* s);

		template<>
		bool RecvDataT<std::wstring>(std::wstring* s);

		/*bool SendDataComp(char* my_data, int size_of_data, int compLevel) {
			size_t estimated_size_of_compressed_data = ZSTD_compressBound(size_of_data);
			char* compressed_data = (char*)malloc(estimated_size_of_compressed_data);
			size_t actual_size_of_compressed_data = ZSTD_compress(compressed_data, estimated_size_of_compressed_data, my_data, size_of_data, compLevel);
			if (SendDataT<int>(size_of_data)) {
				if (SendDataT<int>(actual_size_of_compressed_data)) {
					if (SendData(compressed_data, actual_size_of_compressed_data)) {
						return true;
					}
				}
			}

			return true;
		}

		bool RecvDataComp(char** obj) {
			int OrigSize;
			if (!RecvDataT<int>(&OrigSize)) {
				return false;
			}
			int CompSize;
			if (!RecvDataT<int>(&CompSize)) {
				return false;
			}
			char* data = nullptr;

			if (!RecvData(&data)) {
				return false;
			}

			void* decompdata = malloc(OrigSize);

			int DecompressedSize = ZSTD_decompress(decompdata, OrigSize, (void*)data, CompSize);

			int imgtype;
			if (!RecvDataT<int>(&imgtype)) {
				return false;
			}

			*obj = (char*)decompdata;

			return true;
		}
		template<typename T>
		bool SendDataTComp(T obj) {
			return SendDataComp(reinterpret_cast<char*>(&obj), sizeof(T));
		}
		template<typename T>
		bool RecvDataTComp(T* obj) {
			char* data = nullptr;

			if (!RecvDataComp(&data)) {
				return NULL;
			}

			T* ret = reinterpret_cast<T*>(data);

			*obj = *ret;
		}*/

		bool SendBatchData(char* data, int size, int packetSize);

		bool RecvBatchData(char** data);

		bool SendFile(std::string path, int PackSize);

		bool RecvFile(std::string path);

		//bool SendCVMat(cv::Mat img) {
		//	if (SendDataT<int>(img.rows)) {
		//		if (SendDataT<int>(img.cols)) {

		//			char* k = (char*)img.data;

		//			if (SendData(k, img.total() * img.elemSize())) {

		//				if (SendDataT<int>(img.type())) {

		//					return true;
		//				}
		//			}
		//		}
		//	}
		//	return false;
		//}

		//bool RecvCVMat(cv::Mat* img) {
		//	int rows;
		//	if (!RecvDataT<int>(&rows)) {
		//		return false;
		//	}
		//	int cols;
		//	if (!RecvDataT<int>(&cols)) {
		//		return false;
		//	}
		//	char* data = nullptr;

		//	if (!RecvData(&data)) {
		//		return false;
		//	}

		//	int imgtype;
		//	if (!RecvDataT<int>(&imgtype)) {
		//		return false;
		//	}

		//	*img = cv::Mat(rows, cols, imgtype, data);

		//	return true;
		//}

		////compress image data then send
		//bool SendCVMatComp(cv::Mat img) {
		//	if (SendDataT<int>(img.rows)) {
		//		if (SendDataT<int>(img.cols)) {

		//			char* my_data = (char*)img.data;
		//			int size_of_data = img.total() * img.elemSize();
		//			size_t estimated_size_of_compressed_data = ZSTD_compressBound(size_of_data);
		//			void* compressed_data = malloc(estimated_size_of_compressed_data);
		//			size_t actual_size_of_compressed_data = ZSTD_compress(compressed_data, estimated_size_of_compressed_data, my_data, size_of_data, 19);
		//			if (SendDataT<int>(size_of_data)) {
		//				if (SendDataT<int>(actual_size_of_compressed_data)) {
		//					if (SendData((char*)compressed_data, actual_size_of_compressed_data)) {

		//						if (SendDataT<int>(img.type())) {

		//							return true;
		//						}
		//					}
		//				}
		//			}
		//		}
		//	}
		//	return false;
		//}
		////decompress image data when recv
		//bool RecvCVMatComp(cv::Mat* img) {
		//	int rows;
		//	if (!RecvDataT<int>(&rows)) {
		//		return false;
		//	}
		//	int cols;
		//	if (!RecvDataT<int>(&cols)) {
		//		return false;
		//	}
		//	int OrigSize;
		//	if (!RecvDataT<int>(&OrigSize)) {
		//		return false;
		//	}
		//	int CompSize;
		//	if (!RecvDataT<int>(&CompSize)) {
		//		return false;
		//	}
		//	char* data = nullptr;

		//	if (!RecvData(&data)) {
		//		return false;
		//	}

		//	void* decompdata = malloc(OrigSize);

		//	int DecompressedSize = ZSTD_decompress(decompdata, OrigSize, (void*)data, CompSize);

		//	int imgtype;
		//	if (!RecvDataT<int>(&imgtype)) {
		//		return false;
		//	}

		//	*img = cv::Mat(rows, cols, imgtype, decompdata);

		//	return true;
		//}

	};

	Socket* GetSocket(int i);

	int GetNumConnections();

protected:
	std::string Name;

	std::vector<Socket*> Connections;

	WSADATA wsaData;
	int iResult;

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	SOCKET ListenSocket = INVALID_SOCKET;
};

class DllExport Server : public Network {
	std::vector<std::thread *> threads;

public:

	bool Init(PCSTR Port, std::string _Name);

	bool Listen();

	bool AcceptConn(void (*RecvFunct)(Socket* sock));

	bool DisConnect(Socket* ns);
};

class DllExport Client :public Network {
public:
	bool Init(std::string _Name);


	bool Connect(PCSTR ip, PCSTR Port, Socket* destSocket);

	bool DisConnect(Socket* ns);
};


