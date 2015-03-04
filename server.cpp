#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <time.h>

// adds wsock32.lib to the linker
#pragma comment(lib, "wsock32.lib")

using namespace std;

void main() {
	std::ifstream infile("C:/Users/sjaveed.bscs12seecs/Desktop/Lab3/myfile.txt", std::ifstream::binary);
	std::ofstream outfile("new.txt", std::ofstream::binary);
	long size = infile.tellg();
	WSADATA wsa_data; // this variable will contain the details of the winsock connection
	// specifies winsock version 2.0
	if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
		cout << "Error initializing winsock version 2.0" << endl;
		WSACleanup(); // terminates the use of the winsock 2 DLL
		return;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "Error creating socket" << endl;
		WSACleanup(); // terminates the use of the winsock 2 DLL
		return;
	}


	SOCKADDR_IN server_info;
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = INADDR_ANY;
	server_info.sin_port = htons(6789);


	if (bind(sock, (SOCKADDR *)(&server_info), sizeof(server_info)) == SOCKET_ERROR) {
		cout << "Error binding socket" << endl;
		WSACleanup(); // terminates the use of the winsock 2 DLL
		return;
	}


	listen(sock, 1);

	cout << "Waiting for connection . . ." << endl;

	SOCKET client_socket = accept(sock, NULL, NULL);

	cout << "Client connected" << endl;

	/*+++++++++++++++++++++++
	Download file from client
	++++++++++++++++++++++++*/

	char* rbuffer;
	rbuffer = new char[5000]; //will write message to this buffer
	char* rbuffer1;
	rbuffer = new char[5000];
	clock_t ustart, uend;
	ustart = clock();
	int k = recv(client_socket, rbuffer, sizeof(rbuffer), NULL);
	if (k < 0){
		cout << "Error uploading file" << endl;
	}
	outfile.write(rbuffer, sizeof(rbuffer));
	outfile.close();
	cout << "File is being uploaded..." << endl;

	cout << "File received!" << endl;

	//send acknowledgement
	/*char* ackmsg;
	ackmsg = new char[500];
	ackmsg = "File successfully received!";

	int a = send(client_socket, ackmsg, 500, NULL);*/

	uend = clock() - ustart;

	cout << "Time to upload file was " << (double)uend / ((double)CLOCKS_PER_SEC) << " seconds." << endl;

	/*+++++++++++++++++++++
	Next, send the file back
	++++++++++++++++++++++*/


	clock_t cstart, cend;
	cstart = clock();
	int s = send(client_socket, rbuffer, sizeof(rbuffer), NULL); //send message back to client
	if (s == SOCKET_ERROR){
		cout << "Error sending file back to client :(" << endl;
	}
	cout << "Sending file back to client..." << endl;

	cend = clock() - cstart;

	cout << "Time to send file back to client was " << (double)cend / ((double)CLOCKS_PER_SEC) << " seconds." << endl;

	shutdown(client_socket, SD_SEND);

	closesocket(client_socket);

	WSACleanup();
}
