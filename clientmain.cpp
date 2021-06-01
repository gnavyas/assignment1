#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>




// Included to get the support library
// #include <calcLib.h>
using namespace std;

#define MAX 80
#define PORT 8000
#define SA struct sockaddr

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

  int len;
  const int buffLength = 1256;
  char recieve[buffLength], sendMsg[buffLength];
  struct sockaddr_in my_addr;

  if(argc != 2){
    cerr << "Usage: ./client IP:port" << endl;
    return -1;
  }

  char * ip_addr = strtok(argv[1], ":");
  char * port_num = strtok(NULL, ":");

  if((ip_addr == NULL) || (port_num == NULL)){
    cerr << "Usage: ./client IP:PORT" << endl;
    return -1;
  }

  int client = socket(AF_INET, SOCK_STREAM, 0);
  if (client < 0){
    cerr << "Error in client creating" << endl;
  }else{
    cout << "Client Created" << endl;
  }

  my_addr.sin_family      = AF_INET;
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_port        = htons(stoi(port_num));
  my_addr.sin_addr.s_addr = inet_addr(ip_addr);

  int con = connect(client, (struct sockaddr*) &my_addr, sizeof(my_addr));
  if (con == 0)
  {
    cout << "Connection Successful" << endl;
  } else {
    cerr << "Error in Connection" << endl;
  }


  // start recieveing TCP version from the client
  cout << endl << "TCP Versions at Server" << endl;
  while(1)
  {
    len = recv(client, recieve, buffLength, 0);
    if(len <= 0){
      perror("recv");
      return -1;
    }
    recieve[len] = '\0';

    cout << "Server []: " << recieve << endl;
    if(recieve[0] == '\n'){
      break;
    }
  }

  len = snprintf(sendMsg, buffLength, "OK\n");
  send(client, sendMsg, len, 0);
  cout << endl;

  len = recv(client, recieve, buffLength, 0);
  char showRecieve[100];
  for(int i=0; i < len; i++)
  {
    if(recieve[i]=='\0')
    {
      showRecieve[i] = '\0';
      break;
    }
    else if(recieve[i]>='a' and recieve[i]<='z'){
      showRecieve[i] = recieve[i]-32;
    }else{
      showRecieve[i] = recieve[i];
    }
  }
  cout << "Server Task [] : " << showRecieve << endl;

  vector<string> recievedOP;
  char* token = strtok(recieve, " ");
  while (token != NULL) {
    recievedOP.push_back(token);
    token = strtok(NULL, " ");
  }


  if( recievedOP[0][0] == 'f' )
  {

    float v1 = stof(recievedOP[1]);
    float v2 = stof(recievedOP[2]);
    float myAns;

    if( recievedOP[0] == "fsub"){
      myAns = v1-v2;

    }else if( recievedOP[0] == "fadd"){
      myAns = v1+v2;

    }else if( recievedOP[0] == "fdiv"){
      myAns = v1/v2;

    }else if( recievedOP[0] == "fmul"){
      myAns = v1*v2;
    } else {
      cerr << "Error: Invalid operation " << recievedOP[0] << endl;
    }

    len = snprintf(sendMsg, buffLength, "%8.8f\n", myAns);

  } else {

    int v1 = stoi(recievedOP[1]);
    int v2 = stoi(recievedOP[2]);
    int myAns;

    if( recievedOP[0] == "div"){
      myAns = v1/v2;

    }else if( recievedOP[0] == "add"){
      myAns = v1+v2;

    }else if( recievedOP[0] == "sub"){
      myAns = v1-v2;

    }else if( recievedOP[0] == "mul"){
      myAns = v1*v2;

    } else {
      cerr << "Error: Invalid operation " << recievedOP[0] << endl;
    }

    len = snprintf(sendMsg, buffLength, "%d\n", myAns);
  }

  send(client, sendMsg, buffLength, 0);

  //receive server response
  len = recv(client, recieve, buffLength, 0);
  recieve[len] = '\0';  //terminate the string
  cout << "Server's RESPONSE []: " <<  recieve << endl;

  //close the connection
  shutdown(client, SHUT_RDWR);
  close(client);

  return 0;
}
