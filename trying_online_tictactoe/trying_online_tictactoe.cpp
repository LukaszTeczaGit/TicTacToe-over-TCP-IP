#include <iostream>
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
													//TODO dla klienta zrobic oddzielne zmienne
#pragma warning(disable:4996) 

using namespace std;

#define X 1
constexpr auto O = 2;

int TAB[9] = { 0 };
int turn = 1;
int place;
int counter = 0;
string ipToSomeonesServer;


//server ~ client
WSADATA Winsockdata;
int iWsaStartup;
int iWsaCleanup;

SOCKET TCPServerSocket;
int iCloseSocket;

SOCKET TCPClientSocket;

struct sockaddr_in TCPServerAdd;
struct sockaddr_in TCPClientAdd;
int iTCPClientAdd = sizeof(TCPClientAdd);

int iBind;

int iListen;

int iConnect;

SOCKET sAcceptSocket;

int iSend;
char SenderBufferServer[512] = "Hello from server!\n";
int iSenderBufferServer = strlen(SenderBufferServer) + 1;

char SenderBufferClient[512] = "Hello from CLIENT!\n";
int iSenderBufferClient = strlen(SenderBufferClient) + 1;

int iRecv;
char RecvBuffer[512];
int iRecvBuffer = strlen(RecvBuffer) + 1;

int iRecvC;
char RecvBufferC[512];
int iRecvBufferC = strlen(RecvBufferC) + 1;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


int check_if_win();
void printTable();
void game();
void menu();
void serverStartup();
void clientStartup();


int main()
{
	cout << "len(SenderBufferClient) = " << iSenderBufferClient << endl << "len(SenderBufferServer) = " << iSenderBufferServer << endl;
	menu();

	return 0; //end of program
}

void serverStartup() {

	//STEP 1 - WSAStartUp
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);

	if (iWsaStartup != 0) {
		cout << "WSAStartUp failed!" << endl;
	}
	cout << "WSAStartUp Success" << endl;

	//STEP 2 - Fill the structure
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	TCPServerAdd.sin_port = htons(8001);

	//STEP 3 - Socket creation
	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET) {
		cout << "TCP Server socket creation FAILED " << WSAGetLastError() << endl;
	}
	cout << "TCP Server Socket Creation Success" << endl;

	//STEP 4  bind function
	iBind = bind(TCPServerSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iBind == SOCKET_ERROR) {
		cout << "Binding Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Binding Success" << endl;

	//STEP 5 listen function
	iListen = listen(TCPServerSocket, 2);
	if (iListen == SOCKET_ERROR) {
		cout << "Listening Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Listening Success" << endl;

	//STEP 6 Accept
	sAcceptSocket = accept(TCPServerSocket, (SOCKADDR*)&TCPClientAdd, &iTCPClientAdd);
	if (sAcceptSocket == INVALID_SOCKET) {
		cout << "Accept Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Connection Accepted" << endl;

	//STEP 7 Send data to client
	iSend = send(sAcceptSocket, SenderBufferServer, iSenderBufferServer,0);
	if (iSend == SOCKET_ERROR) {
		cout << "Sending Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Data Sending Success" << endl;

	//STEP 8 Recive data from Client
	iRecv = recv(sAcceptSocket, RecvBuffer, 21,0);

	if (iRecv == SOCKET_ERROR) {
		cout << "Recive data Failed & Error No -> " << WSAGetLastError() << endl;
	}
	SetConsoleTextAttribute(hConsole, 10);
	cout << "Data Recived\n";
	printf("%s\n", RecvBuffer);
	SetConsoleTextAttribute(hConsole, 15);

	//STEP 9 Close socket
	iCloseSocket = closesocket(TCPServerSocket);
	if (iCloseSocket == SOCKET_ERROR) {
		cout << "Closing socket Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Closing socket Success" << endl;
	
	//STEP 9 CleanUp from DLL
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR) {
		cout << "Cleaning up from DLL Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "CleanUp from DLL Success" << endl;
}


void clientStartup() {

	//STEP 1 - WSAStartUp
	iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);

	if (iWsaStartup != 0) {
		cout << "WSAStartUp failed!" << endl;
	}
	cout << "WSAStartUp Success" << endl;

	//STEP 2 - Socket creation
	TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPClientSocket == INVALID_SOCKET) {
		cout << "TCP Client socket creation FAILED " << WSAGetLastError() << endl;
	}
	cout << "TCP Client Socket Creation Success" << endl;

	//STEP 3 - Fill the structure
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
	TCPServerAdd.sin_port = htons(8001);
	//InetPton();

	//STEP 4  Connect function
	iConnect = connect(TCPClientSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
	if (iConnect == INVALID_SOCKET) {
		cout << "Connection Failed 7 Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Connection Success" << endl;

	//STEP 5 Recive data from Server
	cout << "B4 len(RECVBUFCLIENT) = " << iRecvBufferC << endl;
	iRecvC = recv(TCPClientSocket, RecvBufferC, 21, 0);
	if (iRecvC == SOCKET_ERROR) {
		cout << "Recive data Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "AFTER len(RECVBUFCLIENT) = " << iRecvBufferC << endl;
	SetConsoleTextAttribute(hConsole, 10);
	cout << "Data Recived -> " << RecvBufferC << endl;
	SetConsoleTextAttribute(hConsole, 15);
	//printf("%s\n", RecvBufferC);

	//STEP 6 Send data to Server
	iSend = send(TCPClientSocket, SenderBufferClient, iSenderBufferClient, 0);
	if (iSend == SOCKET_ERROR) {
		cout << "Sending Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Data Sending Success" << endl;

	//STEP 7 Close socket
	iCloseSocket = closesocket(TCPClientSocket);
	if (iCloseSocket == SOCKET_ERROR) {
		cout << "Closing socket Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "Closing socket Success" << endl;

	//STEP 8 CleanUp from DLL
	iWsaCleanup = WSACleanup();
	if (iWsaCleanup == SOCKET_ERROR) {
		cout << "Cleaning up from DLL Failed & Error No -> " << WSAGetLastError() << endl;
	}
	cout << "CleanUp from DLL Success" << endl;
}

void menu() {
	
	int choice = 5;
	
	while (choice != 4) {
		cout << "\nWybierz opcje:\n\t1.Graj ze znajomym offline\n\t2.Podaj ip do poczekalni znajomego\n\t3.Stworz serwer oraz zapros znajomego do wspolnego grania\n\t4.Wyjscie z gry\n";
		cin >> choice;
		switch (choice)
		{
		case 1:
			game();
			break;
		case 2:
			//cin >> ipToSomeonesServer;
			clientStartup();
			break;
		case 3:
			//TODO: dokonczenie tworzenia wlasnego serwera oraz generowania ip do wyslania do znajomego
			serverStartup();
			break;
		default:
			break;
		}
	}
}

void game() {
	bool playing = true;
	while (playing) {
		printTable();
		if (turn == 1)
			cout << "TURN: X" << endl;
		else
			cout << "TURN: O" << endl;

		cout << "Write 1 - 9 to pick your place" << endl;

		cin >> place;
		place -= 1;

		if (TAB[place] != 0) {
			cout << "This place is taken, pick another one";
			continue;
		}
		counter++;

		TAB[place] = turn;

		turn = turn == 1 ? 2 : 1;

		int sbwin = check_if_win();
		if (sbwin == 1) {
			printTable();
			cout << "O WON\n";
			playing = false;
			break;
		}
		else if (sbwin == 2) {
			printTable();
			cout << "X WON\n";
			playing = false;
			break;
		}

		if (counter == 9) {
			cout << "DRAW\n";
			playing = false;
			break;
		}
	}
}

void printTable() {
	cout << endl;

	/*cout << "\t" << TAB[0] << "\t" << TAB[1] << "\t" << TAB[2] << endl
		<< "\t" << TAB[3] << "\t" << TAB[4] << "\t" << TAB[5] << endl
		<< "\t" << TAB[6] << "\t" << TAB[7] << "\t" << TAB[8] << endl;*/

	for (size_t i = 0; i < 9; i++)
	{
		char printChar;
		switch (TAB[i])
		{
		case 0:
			printChar = '-';
			break;
		case 1:
			printChar = 'X';
			break;
		case 2:
			printChar = 'O';
			break;
		default:
			break;
		}
		cout << "\t" << printChar;
		if (i == 2 || i == 5 || i == 8) cout << endl;
	}

	cout << endl;
}

int check_if_win() {
	if ((TAB[0] == TAB[1] && TAB[1] == TAB[2] && TAB[2] != 0) ||
		(TAB[3] == TAB[4] && TAB[4] == TAB[5] && TAB[5] != 0) ||
		(TAB[6] == TAB[7] && TAB[7] == TAB[8] && TAB[8] != 0) ||
		(TAB[0] == TAB[3] && TAB[3] == TAB[6] && TAB[6] != 0) ||
		(TAB[1] == TAB[4] && TAB[4] == TAB[7] && TAB[7] != 0) ||
		(TAB[2] == TAB[5] && TAB[5] == TAB[8] && TAB[8] != 0) ||
		(TAB[0] == TAB[4] && TAB[4] == TAB[8] && TAB[8] != 0) ||
		(TAB[2] == TAB[4] && TAB[4] == TAB[6] && TAB[6] != 0))
	{
		return turn;
	}
	else return 0;
}


