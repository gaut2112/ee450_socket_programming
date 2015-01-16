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
#define PORT 21851

using namespace std;


void updateAvailable(char a[150], char * sel)
{
int s = atoi(sel);
char a_temp[150]="";
char a_new[150]="";
strcpy(a_temp,a);
int i=1;
char *p=strtok(a_temp,",");

if(s!=i)
{
strcat(a_new,p);
strcat(a_new,",");
}

while(p!=NULL)
{
i++;
char *p=strtok(NULL,",");
if (p==NULL)
 break;
if(s!=i)
{
strcat(a_new,p);
strcat(a_new,",");
}
}
cout<<a_new<<endl;
strcpy(a,"");
strcpy(a,a_new);
}
int doctorInfo(char *sel, char d[150], char k[50], int free[6])
{
int s = atoi(sel);
if (free[s]==1)
 { 
  return 0;
 }
char *ctr=NULL;
ctr=strtok(d,",");
if (s==1)
 {
  strcpy(k,ctr);
  //cout<<ctr<<endl;
  free[s]=1;
  cout<<k;
  return s;
 }
for(int i=2;i<=6;i++)
 {
  ctr=strtok(NULL,",");
  if (s==i)
   {
    //cout<<ctr<<endl;
	strcpy(k,ctr);
	free[s]=1;
	cout<<k;
	return s;
   }
 }
}
void readAvailableSlot(char *s, char *d)
{
char curr[22]="";
char *ctr;
fstream slot("availabilities.txt",ios::in);
if (slot.is_open())
 {
  for(int i=0;i<=5;++i)
  {
   slot.getline(curr,50);
   ctr = strtok(curr," ");
   strcat(s, ctr);
   strcat(s, " ");
   for(int j=1;j<=4;j++)
    { 
	  ctr = strtok(NULL," .");
	 //cout<<ctr<<endl;
	 if(j==1 | j==2)
	 strcat(s, ctr);
	 if(j==3|j==4)
	 strcat(d, ctr);
	 if(j==1)
	  strcat(s," ");
	 if(j==3)
	  strcat(d," ");
	}
   strcat(s, ",");
   strcat(d,",");
   //cout<<s<<endl<<d<<endl;
  }
 }
else
 {
  perror("Error Opening availablities.txt");
  exit(1);
 }
}
int authenticate(char * user, char * pass)
{
cout<<"------------THE HEALTH CENTER SERVER HAS RECEIEVED REQUEST FROM A PATIENT WITH USERNAME:  "<<user<<" AND PASSWORD: "<<pass<<endl;
int ctr = 0;
char u1[20]="";
char u2[20]="";
char p1[30]="";
char p2[30]="";
char patient1_cre[50];
char patient2_cre[50];
//char authenticate1[] = "authenticate ";
//char authenticate2[] = "authenticate ";
fstream user_data("users.txt", ios::in);
if(user_data.is_open())
  {
   user_data.getline(patient1_cre,50); 
   user_data.getline(patient2_cre,50);
  }
else
  {
   perror("File Error");
   exit(1);
  }

strcpy(u1,strtok(patient1_cre," "));
strcpy(p1,strtok(NULL," "));
strcpy(u2,strtok(patient2_cre," "));
strcpy(p2,strtok(NULL," "));
if((strcmp(user,u1)==0 && strcmp(pass,p1)==0))
  ctr=1;
if((strcmp(user,u2)==0 && strcmp(pass,p2)==0))
  ctr=1; 
return ctr;
}  


int main( int argv, char *argc[])
{
//initialize global variables
int socketdes, child_socket, ctr=3, auth, k=1, free_slot[6]={0,0,0,0,0,0},yes =1,ls,rec,sen;
struct sockaddr_in healthserver,cli_addr;
char buffer[2048]="",avail_slot[150]="", doc_info[150]="",doc[150]="", app_info[50]="";
char *a = avail_slot;
char *d = doc_info;
// initialize struct
bzero((char *)&healthserver, sizeof(healthserver));
healthserver.sin_family = AF_INET;
healthserver.sin_addr.s_addr = INADDR_ANY;
healthserver.sin_port = htons(PORT);
//create socket
socketdes = socket(PF_INET, SOCK_STREAM,0);
if (socketdes == -1)
  {
   perror("Socket Creation Failed");
   exit(1);
  }
//set socket option
setsockopt(socketdes, SOL_SOCKET, SO_REUSEADDR,&yes, sizeof(int));// for bind error: address already in use
//bind socket
int binddesc = bind(socketdes, (struct sockaddr *) &healthserver, sizeof(healthserver));
if (binddesc == -1)
  {
   perror("Bind Issue");
   exit(1);
  }
//read slots from availabities.txt
readAvailableSlot(a,d);
//cout<<avail_slot<<endl;
socklen_t len = sizeof(cli_addr);
cout<<"Server listening to connections Now"<<endl;
//listening
ls = listen(socketdes, 2);
if(ls <0)
 {
   perror("Error in Listen:");
   exit(1);
  }
//Accepting calls
while(1)
 {
    child_socket = accept(socketdes,(struct sockaddr*)&cli_addr, &len);
	if(child_socket <0)
    {
     cout<<child_socket<<endl;
	 perror (" Error in accept\n");
     exit(1);
    }
   
   while(k)
   {
    char u1[20];
	strcpy(doc,doc_info);
	strcpy(buffer,"");
	strcpy(app_info,"");
	ctr=0;
	rec=recv(child_socket,buffer, 100, 0);
	if (rec<0)
	  {
	  perror("Error in Receive:");
	  exit(1);
	  }
    //cout<<buffer<<endl;
    char * str;
    str = strtok(buffer," ");
    //cout<<str<<endl;
    if (strcmp(str,"authenticate")==0)
     ctr=1;
    if (strcmp(str,"available")==0) 
     ctr=2;
    switch(ctr)
	   {
	   case 1:strcpy(u1,strtok(NULL," "));
	          auth = authenticate(u1,strtok(NULL," "));
		      if (auth == 1)
			    {
			    sen=send (child_socket, "success",8,0);
				if(sen<0)
				{
				 perror("Error in Send");
				 exit(1);
				} 
				cout<<"--HEALTH CENTER SERVER SENDS THE RESPONSE OF success TO PATIENT WITH USERNAME: "<<u1<<endl;
				}
		      else
		       {
		        sen=send (child_socket, "failure",8,0);
				if(sen<0)
				{
				 perror("Error in Send");
				 exit(1);
				} 
				cout<<"--HEALTH CENTER SERVER SENDS THE RESPONSE OF failure TO PATIENT WITH USERNAME: "<<u1<<endl;
			    close(child_socket);
				k=0;
			   }
			   break;
			   
	   case 2: int chk;
	           char * p;
	           sen=send(child_socket,avail_slot,strlen(avail_slot)+1,0);
			   if(sen<0)
				{
				 perror("Error in Send");
				 exit(1);
				} 
			   rec=recv(child_socket,buffer,12,0);
			   if (rec<0)
	            {
	              perror("Error in Receive:");
	              exit(1);
	            }
			  
			   str = strtok(buffer," ");
			   p = strtok(NULL," ");
			   //cout<<doc<<endl<<buffer<<endl;
			   if (strcmp(str,"selection")==0)
			    {
				 chk=doctorInfo(p,doc,app_info,free_slot);
				 updateAvailable(avail_slot,p);
				 cout<<app_info<<endl;
				 if (chk==0)
				  {
				   sen=send(child_socket,"notavailable",13,0);
				   if(sen<0)
				    {
				      perror("Error in Send");
				      exit(1);
				    } 
				  }
				  else
				  {
				 //cout<<"appointment"<<app_info<<endl;
				    sen=send(child_socket,app_info,strlen(app_info)+1,0);
				    if(sen<0)
				      {
				        perror("Error in Send");
				        exit(1);
				      } 
				  }
				}
				k=0;
				close(child_socket);
			    break;
			  
	   case 3: cout<<"Wrong Request From Client!! Exiting!!!\t\n";
	           close(child_socket);
	           exit(1);
	   }
	}
	k=1;
 }
close (socketdes);
return 0;
}
