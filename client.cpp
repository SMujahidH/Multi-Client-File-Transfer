#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <omp.h>
#include <windows.h>  
#include <intrin.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")
#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024



// Function to get CPU info
std::string GetCPUInfo() {
    char CPUBrandString[0x40];
    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 0x80000002);
    memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000003);
    memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000004);
    memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
    return std::string(CPUBrandString);
}


// Function to get system memory info
std::string GetMemoryInfo() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    return std::to_string(totalPhysMem / (1024 * 1024 * 1024))+" GB";
}

// Function to get CPU core and thread count
std::string GetCoreAndThreadInfo() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    int physicalCores = std::thread::hardware_concurrency() / sysInfo.dwNumberOfProcessors;
    int logicalProcessors = sysInfo.dwNumberOfProcessors;

    return "Physical Cores: " + std::to_string(physicalCores) + 
           ", Logical Processors (Threads): " + std::to_string(logicalProcessors);
}

void sendFile(SOCKET clientSocket, const std::string& fileName) {
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    // Send file name
    send(clientSocket, fileName.c_str(), fileName.size(), 0);

    // Calculate file size
    inFile.seekg(0, std::ios::end);
    std::streamsize fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    // Number of chunks to send
    const int chunkSize = BUFFER_SIZE; // Adjust as needed
    int numChunks = (fileSize + chunkSize - 1) / chunkSize;

    // Create a buffer for reading the file
    char* buffer = new char[chunkSize];

    // Parallel file sending using OpenMP
    #pragma omp parallel for
    for (int i = 0; i < numChunks; ++i) {
        // Seek to the position in the file
        inFile.seekg(i * chunkSize);
        std::streamsize bytesRead = inFile.read(buffer, chunkSize).gcount();

        // Send the chunk
        send(clientSocket, buffer, bytesRead, 0);
    }

    std::cout << "File sent successfully: " << fileName << std::endl;
    delete[] buffer;
    inFile.close();
}

int main() {


    std::cout << "Hardware Information:" << std::endl;
    std::cout << "CPU: " << GetCPUInfo() << std::endl;
    std::cout << "RAM: " << GetMemoryInfo() << std::endl;
    std::cout << GetCoreAndThreadInfo() << std::endl;
    
    // Initialize Winsock

    WSADATA wsaData;
    SOCKET clientSocket;
    struct sockaddr_in serverAddr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error initializing Winsock." << std::endl;
        return 1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket." << std::endl;
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection to server failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::string fileName;
    std::cout << "Enter the file name to send: ";
    std::getline(std::cin, fileName);

    auto start = std::chrono::high_resolution_clock::now();
    sendFile(clientSocket, fileName);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Time taken to send file: " << elapsed.count() << " seconds." << std::endl;

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
