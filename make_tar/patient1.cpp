#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <iostream>
#include <strings.h>
#include <stdlib.h>
# define SERVER_PORT 21851
#define AUTH_MAX_LEN 1000
#define host "nunki.usc.edu"
using namespace std;


void extractCredentials(char *, char *);
int authenticate(int , char *, char *,int);

int main()
{
//string user, pass;
char user[20],*sel,selection[]="selection ",app_info[50];
char pass[30];
char avail[150];
char auth[10];
char * s=avail;
extractCredentials(user,pass);
int authe;
int socketdes,udp_sock;
struct sockaddr_in healthserver_addr, local_addr;
char buffer[2048];
//create Socket
socketdes = socket(PF_INET, SOCK_STREAM,0);
if (socketdes == -1)
 {
  perror("Socket Creation Failed");
  exit(1);
 }
 
 //bind patient socket to get scoket address//
bzero((char *)&local_addr, sizeof(local_addr));
local_addr.sin_family = AF_INET;
local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
local_addr.sin_port = htons(0);

	if (bind(socketdes, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
		perror("bind failed");
		return 0;
	}
 // print our own socket and IP address : code has been taken from beej
 int i;
 bool hst;
 struct hostent *he;
 struct in_addr **addr_list;
 socklen_t alen = sizeof(local_addr);
 if (getsockname(socketdes, (struct sockaddr *)&local_addr, &alen) < 0) {
                perror("getsockname failed");
                return 0;
        }
		
if ((he = gethostbyname(host)) == NULL) {  // get the host info
        herror("gethostbyname");
        return 2;
    }
addr_list = (struct in_addr **)he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) 
	{
        cout<<"Phase 1:Patient 1 has TCP port Number "<<ntohs(local_addr.sin_port)<<" and IP address "<<inet_ntoa(*addr_list[i])<<endl;
    }
//initialize
bzero((char *)&healthserver_addr, sizeof(healthserver_addr));
healthserver_addr.sin_family = AF_INET;
healthserver_addr.sin_addr.s_addr = INADDR_ANY;
healthserver_addr.sin_port = htons(SERVER_PORT);
//connect to server
if (connect(socketdes, (struct sockaddr *)&healthserver_addr, sizeof(healthserver_addr)) <0)
 {
  perror("Connection Failed");
  close(socketdes);
  exit(1);
 }
//send authentication message and selection Phase 1 and Phase 2

authe=authenticate(socketdes, user, pass, 0);
cout<<"End of Phase 1 for Patient 1"<<endl;
if(authe == 1)
 {
  cout<<"Phase 2: Following Appointments are available for Patient 1"<<endl;
  send(socketdes, "available", 11, 0);
  int len = recv(socketdes,(char *)&avail,150, 0);
  //cout<<avail<<endl;
  char * p;
  int l=1;
  //int sel;
  char b[2]="";
  cout<<"No Day Time"<<endl;
  p = strtok(avail,",");
  cout<<l<<" "<<p<<endl;
  while(p!=NULL)
   {
    l++;
	p = strtok(NULL,",");
	if (p==NULL)
	 break;
    cout<<l<<" "<<p<<endl;
   }
  l--;
  cout<<"Please Enter the preferred appointment index and press enter \t";
  cout<<endl;
  cin>>b;
  if (atoi(b)>l | atoi(b) <1)
  {
   while(atoi(b)>l | atoi(b) <1)
   {
    cout<<"Please Enter Correct Choice:\t";
	cin>>b;
   }
  }
  
  //cout<<sel;
  strcat(selection,b);
  //selection[12]='\0';
  send(socketdes,selection, 12, 0);
  recv(socketdes,(char *)&app_info,50, 0);
  char a_i[50];
  strcpy(a_i,app_info);
  strtok(a_i," ");
  //cout<<app_info<<endl;
  cout<<"Phase 2:The requested appointment is available and reserved for patient1. The assigned doctor port number is "<<(atoi(strtok(NULL," "))+851)<<endl;
  if (strcmp(app_info,"notavailable")==0)
   {
     cout<<"Phase 2:The requested appointment from Patient 1 is	not available.Exiting…"<<endl;
	 exit(1);
   }
 

close (socketdes);
// open connection to doctor
struct sockaddr_in myaddr, docaddr;
char *portno,ins[20],cost[10], *doc_name;
int prt;
socklen_t si = sizeof(docaddr);
doc_name = strtok(app_info," ");
portno = strtok(NULL," ");
prt=atoi(portno);
prt=prt+851;
//cout<<p<<endl;
udp_sock=socket(PF_INET, SOCK_DGRAM,0);
if (udp_sock == -1)
 {
  perror("Socket Creation Failed");
  exit(1);
 }
 
//Initialize Own socket
int n = sizeof(myaddr);
bzero(&myaddr,n);
myaddr.sin_family = AF_INET;
myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
myaddr.sin_port = htons(0);
//Bind for bidirectional communication following code has been is modified version of Bidirectional UDP code from https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html  //
if (bind(udp_sock, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
 socklen_t alen_u = sizeof(local_addr);
 if (getsockname(udp_sock, (struct sockaddr *)&myaddr, &alen_u) < 0) {
                perror("getsockname failed");
                return 0;
        }
cout<<"Phase 3:Patient1 has dynamic UDP port number "<<ntohs(myaddr.sin_port)<<" and IP address "<<inet_ntoa(*addr_list[0])<<endl;
//initialize doctor socket
bzero(&docaddr,sizeof(docaddr));
docaddr.sin_family = AF_INET;
docaddr.sin_port = htons(prt);
docaddr.sin_addr.s_addr = htonl(INADDR_ANY);

//send doctor data
fstream f("patient1insurance.txt",ios::in);
f.getline(ins,20);
f.close();
cout<<"Phase 3: The cost estimation request from Patient1 with insurance plan "<<ins<<" has been sent to doctor with port number "<<prt<<" and IP address "<<inet_ntoa(*addr_list[0])<<endl;
sendto(udp_sock,ins,strlen(ins)+1,0,(sockaddr *)&docaddr,sizeof(docaddr));
//receive from server
recvfrom(udp_sock,cost,3,0,(sockaddr *)&docaddr,&si);
cout<<"Phase 3:Patient 1 receives cost estimation of "<<cost<<"$ from doctor with port number"<<p<<" and name"<<doc_name<<endl;
cout<<"End of phase 3 for patient 1";
close(udp_sock);
}
else
 {
  exit(1);
 }
return 0;
}

//Function To authenticate Client//

int authenticate( int sock, char *user, char *pass, int flag)
{
cout<<"Phase 1:Authentication Request from Patient1 with username "<<user<<" and password "<<pass<<" has been sent to Health Centre Server"<<endl;
char authenticate[]="authenticate ";
char rec[8];
strcat(user, " ");
strcat(user, pass);
strcat(authenticate, user);
//cout<<authenticate<<endl;
int s = send(sock, authenticate, strlen(authenticate)+1, flag);
if (s == -1)
 {
  perror("error sending data");
  close (sock);
  exit (1);
 }
int r = recv(sock, (char *)&rec,8, flag);
cout<<"Phase 1:Patient 1 authentication result "<<rec<<endl;
if (strcmp(rec,"success") == 0)
 return 1;
else 
 return 0;
}


void extractCredentials(char *user, char *pass)// function to read credentials from file
{
fstream credentials ("patient1.txt", ios::in);
if(credentials.is_open())
 {
  credentials >> user;
  credentials >> pass;
 }
credentials.close();
}
