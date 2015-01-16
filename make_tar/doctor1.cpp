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
#include <strings.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#define PORT 41851
#define MAX_BUFFER 2048
#define host "nunki.usc.edu"

using namespace std;

void loadInsuranceDetails(char ins_d[100])//function to read insurance details into the memory when doctor starts up//
{
fstream f("doc1.txt",ios::in);
char buf[100]="";
char *ch;
for(int i=0;i<3;i++)
{
f.getline(buf,20);
strcat(ins_d,strtok(buf," "));
strcat(ins_d,",");
strcat(ins_d,strtok(NULL," "));
strcat(ins_d,":");
}
f.close();
}

void calculateInsurance(char *c, char ins_d[100], char ins_s[3])//function to calculate insurance details upon receiving insurance details
{
char *ch[3],*s;

ch[0] = strtok(ins_d,":");
ch[1] = strtok(NULL,":");
ch[2] = strtok(NULL,":");
for(int i=0;i<3;i++)
 {
   s = strtok(ch[i],",");
   if(strcmp(c,s)==0)
    {
	 strcpy(ins_s,strtok(NULL," "));
	}
 }
ins_s[2]='\0';
}


int main()
{
int sockdes, n;
struct sockaddr_in myserver,remote;
socklen_t len = sizeof(remote);
ssize_t k;
char buffer[MAX_BUFFER], ins[100]="", ins_send[3],ins_m[100]="";
//create socket
 sockdes=socket(PF_INET, SOCK_DGRAM, 0);
 if (sockdes < 0) perror("Opening socket");
 n = sizeof(myserver);
 //Initialize
 bzero(&myserver,n);
 myserver.sin_family=AF_INET;
 myserver.sin_addr.s_addr=INADDR_ANY;
 myserver.sin_port=htons(PORT);
 //get IP address of host: following code has been taken from beej
  int i;
 bool hst;
 struct hostent *he;
 struct in_addr **addr_list;
if ((he = gethostbyname(host)) == NULL) {  // get the host info
        herror("gethostbyname");
        return 2;
    }
addr_list = (struct in_addr **)he->h_addr_list;
    for(i = 0; addr_list[i] != NULL; i++) 
	{
        cout<<"Phase3:Doctor 1 has static UDP port "<<PORT<<" and IP address "<<inet_ntoa(*addr_list[i])<<endl;
    }
 
 //load insurance data
 loadInsuranceDetails(ins);
//bind
 if (bind(sockdes,(struct sockaddr *)&myserver,n)<0) 
       perror("binding");
 
//start receiving from client : following code has been is modified version of Bidirectional UDP code from https://www.cs.rutgers.edu/~pxk/417/notes/sockets/udp.html  //

 while(1)
 { 
   strcpy(ins_m,ins);
   k=recvfrom(sockdes,buffer,MAX_BUFFER+1,0,(sockaddr *)&remote,&len);
   if (k==-1)
   {
    perror("Error In receiving");
	exit(1);
   }
   cout<<"Phase 3:Doctor 1 receives a request from patient with port number "<<ntohs(remote.sin_port)<<" and the insurance plan "<<buffer<<endl;
   calculateInsurance(strtok(buffer," "),ins_m,ins_send);
   
   sendto(sockdes,ins_send,strlen(ins_send)+1,0,(sockaddr *)&remote,len);
   cout<<"Phase 3:Doctor 1 has sent estimated price of $"<<ins_send<<" to patient with port number "<<ntohs(remote.sin_port)<<endl;
  }
  return 0;
 }
 
   
   
   


