#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include<bits/stdc++.h>
#include<time.h>


// Included to get the support library
// #include <calcLib.h>



using namespace std;

#define MAX 80
#define PORT 8000
#define SA struct sockaddr
#define RANDMAX 1000

#define ll           long long int
#define fast         ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define INF          1e18
#define MOD          1000000007
#define Vi           vector<ll>
#define Vs           vector<string>
#define VI           vector<vector<ll> >
#define PII          pair<ll,ll>
#define PSI          pair<string,ll>
#define PB           push_back


int main(int argc, char *argv[])
{
	const int buffLength = 1256;
	char sendMsg[buffLength], recieve[buffLength];

	if(argc != 2){
    cerr << "Usage: ./server IP:PORT" << endl;
    return -1;
  }

  char * ip_addr = strtok(argv[1], ":");
  char * port_num = strtok(NULL, ":");

  if((ip_addr == NULL) || (port_num == NULL)){
    cerr << "Usage: ./client IP:port" << endl;
    return -1;
  }

	int server = socket(AF_INET, SOCK_STREAM, 0);
	if (server < 0)
	{
		cerr << "Error in server creating" << endl;
	}
	else
	{
		cout << "Server Created Successfully" << endl;
	}

	struct sockaddr_in my_addr, peer_addr;
	my_addr.sin_family 			= AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	my_addr.sin_port        = htons(stoi(port_num));
  my_addr.sin_addr.s_addr = inet_addr(ip_addr);

	if (bind(server, (struct sockaddr*) &my_addr, sizeof(my_addr)) == 0)
	{
		cout << "Bind Success" << endl;
	}
	else
	{
		cerr << "Unable to bind" << endl;
	}

	if (listen(server, 3) == 0)
	{
		cout << "Listening for connections..." << endl;
	}
	else
	{
		cout << "Unable to listen" << endl;
	}

	Vs availableOps;
	availableOps.push_back("add");
	availableOps.push_back("fadd");
	availableOps.push_back("sub");
	availableOps.push_back("fsub");
	availableOps.push_back("div");
	availableOps.push_back("fdiv");
	availableOps.push_back("mul");
	availableOps.push_back("fmul");

	Vs tcpVersions = {"TEXT TCP 1.0", "TEXT TCP 1.1", "TEXT TCP 1.2",""};

	srand(time(0));

	while (1)
	{

		socklen_t addr_size;
		addr_size = sizeof(struct sockaddr_in);

		cout << "Waiting for Clients  ....." << endl;
		int acc = accept(server, (struct sockaddr*) &peer_addr, &addr_size);
		if(acc <= 0){
			break;
		}
		cout << "Connection Established Successfully" << endl;

		char ip[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(peer_addr.sin_addr), ip, INET_ADDRSTRLEN);


		cout << "CONNECTION INFO -> IP : " << ip << " :: PORT : " << ntohs(peer_addr.sin_port) << endl;


		for(unsigned int i=0; i < tcpVersions.size(); i++)
		{
			bzero(sendMsg, buffLength);
			snprintf(sendMsg, buffLength, "%s\n", tcpVersions[i].c_str());
			send(acc, sendMsg, buffLength, 0);
		}

		bzero(recieve, buffLength);
		recv(acc, recieve, buffLength, 0);
		printf("Client : %s", recieve);
		cout << endl;

		//if client didn't accept the protocol
		if(strncmp(recieve, "OK\n", 3) != 0){
			//close connection
			shutdown(acc, SHUT_RDWR);
			close(acc);
			continue;
		}

		int rand_op = rand() % 8;
		string currentOperation = availableOps[rand_op];

		// integer PART
		if( currentOperation[0] != 'f' )
		{

			int v1 = rand()%RANDMAX;
			int v2 = rand()%RANDMAX;
			int actualAns;
			if( currentOperation == "add" ){
				actualAns = v1+v2;

			}else if( currentOperation == "mul" ){
				actualAns = v1*v2;

			}else if( currentOperation == "sub" ){
				actualAns = v1-v2;

			}else if( currentOperation == "div" ) {
				// no divison of 0 is allowed
				if(v2 == 0){
					v2++;
				}
				actualAns = v1/v2;
			}

			snprintf(sendMsg, buffLength, "%s %i %i\n", currentOperation.c_str(), v1, v2);

			printf("Task: %s\n", sendMsg);

			send(acc, sendMsg, buffLength, 0);

			recv(acc, recieve, buffLength, 0);
			printf("Client's Response : %s\n", recieve);
			cout << "\n";


			int clientAnswer = atoi(recieve);
			if(clientAnswer != actualAns)
			{
				strcpy(sendMsg, "OK");
			}
			else
			{
				strcpy(sendMsg, "ERROR");
			}

		}

		// floating PART
		else
		{

			float A = rand()%RANDMAX;
			float B = rand()%100;
			float v1 = A/B;

			A = rand()%RANDMAX;
			B = rand()%100;
			float v2 = A/B;

			float actualAns;
			if( currentOperation=="fmul" ){
				actualAns = v1*v2;

			}else if( currentOperation == "fadd" ){
				actualAns = v1+v2;

			}else if( currentOperation == "fsub" ){
				actualAns = v1-v2;

			}else if( currentOperation=="fdiv" ){
				// no divison of 0 is allowed
				if((int)v2 == 0){
					v2+=1.0;
				}
				actualAns = v1/v2;
			}

			snprintf(sendMsg, buffLength, "%s %8.8f %8.8f\n", currentOperation.c_str(), v1, v2);
			printf("Task: %s\n", sendMsg);
			send(acc, sendMsg, buffLength, 0);

			recv(acc, recieve, buffLength, 0);
			printf("Client's Response : %s\n", recieve);
			cout << "\n";


			float clientAnswer = atof(recieve);
			if( abs(clientAnswer - actualAns) < 0.0001f)
			{
				strcpy(sendMsg, "OK");
			}
			else
			{
				strcpy(sendMsg, "ERROR");
			}
		}

		//tell client, if answer is valid
		send(acc, sendMsg, buffLength, 0);


		//close the client connection
		shutdown(acc, SHUT_RDWR);
		close(acc);
	}

	close(server);

	return 0;
}
