# Multi-Client File Transfer System

This project implements a multi-client file transfer system using **Socket Programming** and **OpenMP** for parallel processing. The system consists of a server that can accept multiple clients simultaneously and receive files from them, along with a client that can send files to the server efficiently in parallel.

## Features

- **Parallel File Transfer**: Client-side file chunks are sent in parallel using OpenMP for faster transfer.
- **Multi-Client Support**: Server handles multiple clients using multi-threading.
- **System Hardware Information**: Client retrieves and displays hardware information (CPU, memory, cores, and threads).
- **Efficient Buffering**: Buffering is implemented to optimize file reading and sending.
- **Error Handling**: Basic error handling for network and file-related issues.

## Technologies Used

- **C++**: Core programming language.
- **Socket Programming**: For client-server communication.
- **OpenMP**: For parallel processing on the client side.
- **Windows API**: To retrieve system information on the client side.

## Prerequisites

- **Windows**: This project is designed to run on Windows, as it uses Winsock and Windows-specific system calls.
- **Visual Studio**: Recommended IDE for setting up and running the project.
- **OpenMP**: Ensure OpenMP is enabled in your Visual Studio project properties.
- **Winsock Library**: Include `ws2_32.lib` in your project to use Windows socket programming.
- **File to Transfer**: Ensure you have a file ready that you wish to send from the client to the server.

## How to Run the Project

### Step 1: Clone the Repository

```bash
git clone https://github.com/SMujahidH/Multi-Client-File-Transfer.git
cd Multi-Client-File-Transfer
```

### Step 2: Compile and Build

Make sure OpenMP and Winsock libraries are properly set up in your Visual Studio environment:

1. Open the project in Visual Studio.
2. Go to **Project Properties > C/C++ > Language** and set **OpenMP Support** to **Yes**.
3. Link the **ws2_32.lib** library in **Project Properties > Linker > Input**.

### Step 3: Run the Server

Compile and run the `server.cpp` file on the server machine. The server listens for client connections and receives files.

### Step 4: Run the Client

Compile and run the `client.cpp` file on the client machine. The client establishes a connection with the server, displays system hardware information, and sends a specified file to the server.

### Example Usage

1. Start the server by running:

   ```bash
   server.exe
   ```

2. Start the client by running:

   ```bash
   client.exe
   ```

3. Enter the file name you want to send when prompted. Make sure the file already exists in the same directory as `client.exe` or specify the full path if it's located elsewhere.

## File Descriptions

### Server

- **`server.cpp`**: Manages incoming client connections and receives files.
    - Uses `std::thread` to handle multiple clients simultaneously.
    - Stores received files on the server.

### Client

- **`client.cpp`**: Connects to the server, retrieves system information, and sends a file.
    - Uses OpenMP to send file chunks in parallel.
    - Displays hardware details before initiating the file transfer.

### Code Breakdown

- **`handleClient` (Server)**: A function to receive file data from a connected client and save it on the server.
- **`sendFile` (Client)**: Sends a file from the client to the server in chunks using OpenMP for parallel processing.
- **System Information Functions** (Client): Collects CPU, memory, and core/thread information for the client device.

## Example Output

When the client connects to the server, it retrieves and displays the system hardware information:

```
Hardware Information:
CPU: Intel(R) Core(TM) i7-9700K CPU @ 3.60GHz
RAM: 16 GB
Physical Cores: 4, Logical Processors (Threads): 8
```

After sending the file, the client reports the time taken to complete the transfer:

```
Enter the file name to send: example.txt
File sent successfully: example.txt
Time taken to send file: 2.341 seconds.
```

On the server side, a message confirms successful file reception:

```
Server running on port 8080
File received successfully: example.txt
```

## Known Issues

- **Multi-Platform Support**: Currently, the project is configured for Windows OS and may need adjustments to run on Linux or macOS.
- **Firewall/Security Permissions**: Make sure to allow firewall access for both client and server executables if prompted.

## Future Improvements

- Add support for Linux and macOS.
- Implement more advanced error handling and logging.
- Support for encrypted file transfer.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

--- 
