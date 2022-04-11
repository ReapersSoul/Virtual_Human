#include "WINNETLIB.h"
 

std::string Network::Socket::GetName(){
    return Name;
}

void Network::Socket::SetName(std::string name)
{
    Name = name;
}

Network::Socket::Socket(SOCKET _sock, std::string YourName, bool SendFirst) {
    sock = _sock;
    if (SendFirst) {
        SendDataT<std::string>(YourName);
        RecvDataT<std::string>(&Name);
    }
    else {
        RecvDataT<std::string>(&Name);
        SendDataT<std::string>(YourName);
    }
}
Network::Socket::Socket() {
    sock = INVALID_SOCKET;
    Name = "";
}

Network::Socket::~Socket()
{
}

bool Network::Socket::Setup(SOCKET _sock)
{
    sock = _sock;
    return sock != INVALID_SOCKET;
}

bool Network::Socket::ShutDown()
{
    // shutdown the send half of the connection since no more data will be sent
    int iResult = shutdown(sock, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(sock);
        return false;
    }

    // cleanup
    closesocket(sock);
    return true;
}

char* Network::Socket::addToEnd(char* first, int firstsize, char* second, int secondsize)
{
    char* ret;
    ret = new char[firstsize + secondsize];
    for (int i = 0; i < firstsize; i++)
    {
        ret[i] = first[i];
    }
    for (int i = 0; i < secondsize; i++)
    {
        ret[firstsize + i] = second[i];
    }
    return ret;
}

char* Network::Socket::addNullToEnd(char* src, int size)
{
    char* ret = src;

    ret[size] = (char)'\0';

    return ret;
}

char* Network::Socket::PullPart(char* src, int start, int bytesToPull)
{
    char* ret;
    ret = new char[bytesToPull];
    for (int i = 0; i < bytesToPull; i++)
    {
        ret[i] = src[i + start];
    }
    return ret;
}

bool Network::Socket::SendData(char* data, int size)
{
    //send size
    int iResult = send(sock, reinterpret_cast<char*>(&size), sizeof(int), 0);
    if (iResult == SOCKET_ERROR) {
        //printf("send failed: %d\n", WSAGetLastError());
        return false;
    }

    //send data
    iResult = send(sock, data, size, 0);
    if (iResult == SOCKET_ERROR) {
        //printf("send failed: %d\n", WSAGetLastError());
        return false;
    }
    Sleep(100);
    return true;
}

bool Network::Socket::RecvData(char** data)

{


    int size = 0;

    int iResult = recv(sock, reinterpret_cast<char*>(&size), sizeof(int), 0);
    if (iResult > 0) {
        //printf("Bytes received: %d\n", iResult);
        //std::cout << "size of packet: " << size << std::endl;
    }
    else if (iResult == 0) {
        //printf("Connection closed\n");
        return false;
    }
    else {
        //printf("recv failed: %d\n", WSAGetLastError());
        return false;
    }

    char* ret = new char[size];

    iResult = recv(sock, ret, size, 0);
    if (iResult > 0) {
        //printf("Bytes received: %d\n", iResult);
    }
    else if (iResult == 0) {
        //printf("Connection closed\n");
        return false;
    }
    else {
        //printf("recv failed: %d\n", WSAGetLastError());
        return false;
    }

    *data = ret;

    Sleep(300);
    return true;
}

bool Network::Socket::SendBatchData(char* data, int size, int packetSize)
{
    int numOfPackets = size / packetSize;
    int lastPackSize = size % packetSize;


    SendDataT<int>(size);
    SendDataT<int>(numOfPackets);
    SendDataT<int>(packetSize);
    SendDataT<int>(lastPackSize);

    int cursor = 0;
    for (int i = 0; i < numOfPackets; i++)
    {
        //send porion of data
        char* tmp = PullPart(data, cursor, packetSize);
        SendData(tmp, packetSize);
        cursor += packetSize;
        printf("Send Progress: %%%4.2f\n", ((double)cursor / (double)size) * 100);
    }
    if (lastPackSize > 0) {
        //if extra info send here
        char* tmp = PullPart(data, cursor, lastPackSize);
        SendData(tmp, lastPackSize);
        cursor += lastPackSize;
        printf("Send Progress: %%%4.2f\n", ((double)cursor / (double)size) * 100);
    }
    return true;
}

bool Network::Socket::RecvBatchData(char** data)
{

    int size;
    if (!RecvDataT<int>(&size)) {
        return false;
    }
    int numOfPackets;
    if (!RecvDataT<int>(&numOfPackets)) {
        return false;
    }
    int packetSize;
    if (!RecvDataT<int>(&packetSize)) {
        return false;
    }
    int lastPackSize;
    if (!RecvDataT<int>(&lastPackSize)) {
        return false;
    }

    char* ret;
    ret = new char[(numOfPackets * packetSize) + lastPackSize];

    int currentSize = 0;

    for (int i = 0; i < numOfPackets; i++)
    {
        //recv portion of data
        char* tmp;
        if (RecvData(&tmp)) {
            ret = addToEnd(ret, currentSize, tmp, packetSize);
            currentSize += packetSize;
            printf("Recive Progress: %%%4.2f\n", ((double)currentSize / (double)size) * 100);
        }
        else {
            return false;
        }

    }
    if (lastPackSize > 0) {
        //if extra info recv here
        char* tmp;
        tmp = new char[lastPackSize];
        if (RecvData(&tmp)) {
            ret = addToEnd(ret, currentSize, tmp, lastPackSize);
            currentSize += lastPackSize;
            printf("Recive Progress: %%%4.2f\n", ((double)currentSize / (double)size) * 100);
        }
        else {
            return false;
        }
    }
    ret = addNullToEnd(ret, currentSize);
    *data = ret;
    return true;
}

bool Network::Socket::SendFile(std::string path, int PackSize)
{
    std::ifstream in(path, std::ios::binary);
    if (in.is_open()) {

        in.seekg(0, in.end);
        int length = in.tellg();
        in.seekg(0, in.beg);

        if (length == 0) {
            printf("empty file");
            in.close();
            return false;
        }

        int numOfPackets = length / PackSize;
        int lastPackSize = length % PackSize;


        SendDataT<int>(length);
        SendDataT<int>(PackSize);
        SendDataT<int>(numOfPackets);
        SendDataT<int>(lastPackSize);

        int bytesSent = 0;

        char* packet;
        packet = new char[PackSize];
        for (int i = 0; i < numOfPackets; i++)
        {
            in.read(packet, PackSize);
            SendData(packet, PackSize);
            bytesSent += PackSize;
            printf("Send Progress: %%%4.2f\n", ((double)bytesSent / (double)length) * 100);
        }
        if (lastPackSize > 0) {
            in.read(packet, PackSize);
            SendData(packet, lastPackSize);
            bytesSent += lastPackSize;
            printf("Send Progress: %%%4.2f\n", ((double)bytesSent / (double)length) * 100);
        }
        return true;
    }
    return false;
}

bool Network::Socket::RecvFile(std::string path)
{
    std::ofstream out(path, std::ios::binary);

    if (out.is_open() && out.good()) {
        int length;
        if (!RecvDataT<int>(&length)) {
            return false;
        }
        int PackSize;
        if (!RecvDataT<int>(&PackSize)) {
            return false;
        }
        int numOfPackets;
        if (!RecvDataT<int>(&numOfPackets)) {
            return false;
        }
        int lastPackSize;
        if (!RecvDataT<int>(&lastPackSize)) {
            return false;
        }

        int bytesRecived = 0;

        for (int i = 0; i < numOfPackets; i++)
        {
            char* packet;
            packet = new char[PackSize];
            RecvData(&packet);
            out.write(packet, PackSize);
            bytesRecived += PackSize;
            printf("Recive Progress: %%%4.2f\n", ((double)bytesRecived / (double)length) * 100);
        }
        if (lastPackSize > 0) {
            char* packet;
            packet = new char[lastPackSize];
            RecvData(&packet);
            out.write(packet, lastPackSize);
            bytesRecived += lastPackSize;
            printf("Recive Progress: %%%4.2f\n", ((double)bytesRecived / (double)length) * 100);
        }
        out.close();
        return true;
    }
    return false;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

Network::Socket* Network::GetSocket(int i)
{
    return Connections[i];
}

int Network::GetNumConnections()
{
    return Connections.size();
}

bool Server::Init(PCSTR Port, std::string _Name)
{
    Name = _Name;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, Port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return false;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }
    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }
    freeaddrinfo(result);
    return true;
}

bool Server::Listen()
{
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %ld\n", WSAGetLastError());
        return false;
    }

    return true;
}

bool Server::AcceptConn(void(*RecvFunct)(Socket* sock))
{
    SOCKET ConnectedSocket = INVALID_SOCKET;

    // Accept a client socket
    ConnectedSocket = accept(ListenSocket, NULL, NULL);
    if (ConnectedSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        return false;
    }
    Connections.push_back(new Socket(ConnectedSocket, Name, true));
    threads.push_back(new std::thread(RecvFunct, Connections[Connections.size() - 1]));
    return true;
}

bool Server::DisConnect(Socket* ns)
{
    return ns->ShutDown();
}

bool Client::Init(std::string _Name)
{
    Name = _Name;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    return true;
}

bool Client::Connect(PCSTR ip, PCSTR Port, Socket* destSocket)
{
    // Resolve the server address and port
    iResult = getaddrinfo(ip, Port, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return false;
    }

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr = result;

    // Create a SOCKET for connecting to server
    SOCKET sock = socket(ptr->ai_family, ptr->ai_socktype,
        ptr->ai_protocol);

    if (sock == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    // Connect to server.
    iResult = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if (sock == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return false;
    }
    *destSocket = Socket(sock, Name, false);
    return true;
}

bool Client::DisConnect(Socket* ns)
{
    return ns->ShutDown();
}


template<>
inline bool Network::Socket::SendDataT(std::string s)
{
    return SendData((char*)s.c_str(), s.size() + 1);
}

template<>
inline bool Network::Socket::SendDataT(std::wstring s)
{
    return SendData((char*)s.c_str(), s.capacity() * sizeof(wchar_t));
}

template<>
inline bool Network::Socket::RecvDataT(std::string* s)
{
    char* data = nullptr;

    if (RecvData(&data)) {

        *s = data;
        return true;
    }
    return false;
}

template<>
inline bool Network::Socket::RecvDataT(std::wstring* s)
{
    char* data = nullptr;

    if (RecvData(&data)) {

        *s = (wchar_t*)data;
        return true;
    }
    return false;
}