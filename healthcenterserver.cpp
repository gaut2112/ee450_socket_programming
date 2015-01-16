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
#define host "nunki.usc.edu"
using namespace std;


void updateDoc(char d[150], char * sel)//this function updates the doctor information eg "doc1 41851" in the database when patient selects a available slot
{
int s = atoi(sel);
char d_new[150]="";
char d_temp[150]="";
strcpy(d_temp,d);
int i=1;
char *p=strtok(d_temp,",");

if(s!=i)
{
strcat(d_new,p);
strcat(d_new,",");
}

while(p!=NULL)
{
i++;
char *p=strtok(NULL,",");
if (p==NULL)
 break;
if(s!=i)
{
strcat(d_new,p);
strcat(d_new,",");
}
}
strcpy(d,"");
strcpy(d,d_new);
}


void checkAppointmentDetails(char *p,char *slot_info,char a_slot[20])//function to check appointment details
{
 int s = atoi(p);
char *ctr=NULL;
ctr=strtok(slot_info,",");
if (s==1)
 {
  strcpy(a_slot,ctr);
 }
for(int i=2;i<=6;i++)
 {
  ctr=strtok(NULL,",");
  if (s==i)
   {
	strcpy(a_slot,ctr);
   }
 }
}


void updateAvailable(char a[150], char * sel)//this function updates the appointment information eg "1 Tue 1pm2pm" in the database when patient selects a available slot
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
strcpy(a,"");
strcpy(a,a_new);
}

int doctorInfo(char *sel, char d[150], char k[50])//this function returns the doctor information corresponding to a selection made by patient
{
int s = atoi(sel);
char *ctr=NULL;
ctr=strtok(d,",");
if (s==1)
 {
  strcpy(k,ctr);
  return s;
 }
for(int i=2;i<=6;i++)
 {
  ctr=strtok(NULL,",");
  if (s==i)
   {
	strcpy(k,ctr);
	return s;
   }
 }
}




void readAvailableSlot(char *s, char *d)//this function reads the appointment information into memory when server loads up
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
   //strcat(s, ctr);
   //strcat(s, " ");
   for(int j=1;j<=4;j++)
    { 
	  ctr = strtok(NULL," ");
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



void readCredentials(char *p1_cre, char *p2_cre)//this function reads the credentials of the patient when server loads up
{
fstream user_data("users.txt", ios::in);
if(user_data.is_open())
  {
   user_data.getline(p1_cre,50); 
   user_data.getline(p2_cre,50);
  }
else
  {
   perror("File Error");
   exit(1);
  }
}



int authenticate(char * user, char * pass, char p1[50], char p2[50])// this function returns 1 on successful authentication and 0 on failure
{
cout<<"Phase 1: THE HEALTH CENTER SERVER HAS RECEIEVED REQUEST FROM A PATIENT WITH USERNAME:  "<<user<<" AND PASSWORD: "<<pass<<endl;
int ctr = 0;
char u1[20]="";
char u2[20]="";
char ps1[30]="";
char ps2[30]="";
strcpy(u1,strtok(p1," "));
strcpy(ps1,strtok(NULL," "));
strcpy(u2,strtok(p2," "));
strcpy(ps2,strtok(NULL," "));
if((strcmp(user,u1)==0 && strcmp(pass,ps1)==0))
  ctr=1;
if((strcmp(user,u2)==0 && strcmp(pass,ps2)==0))
  ctr=1; 
return ctr;
}  




int main()//main function
{
// startup print IP address and Hostname//follwing code is from beej tutorials
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
        cout<<"Phase 1: Health Center Server has port number "<<PORT<<" and IP address "<<inet_ntoa(*addr_list[i])<<endl;
    }
   
//initialize global variables
int socketdes, child_socket, ctr=3, auth, k=1, free_slot[6]={0,0,0,0,0,0},yes =1,ls,rec,sen,child_port;
struct sockaddr_in healthserver,cli_addr;
char buffer[1000]="",avail_slot[150]="", doc_info[150]="",doc[150]="", app_info[50]="", p1_cre[50]="",p2_cre[50]="",a_slot[20]="",slot_info[150]="";
char *a = avail_slot;
char *d = doc_info;
char *p_1= p1_cre;
char *p_2= p2_cre;
//read user credentials
readCredentials(p_1,p_2);
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

socklen_t len = sizeof(cli_addr);

//listening to client connection
ls = listen(socketdes, 2);
if(ls <0)
 {
   perror("Error in Listen:");
   exit(1);
  }
  
//Accepting calls
while(1)// infinite while loop to accept client connection
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
	char u1[20],p1[50]="",p2[50]="";
	strcpy(p1, p1_cre);
	strcpy(a_slot,"");
	strcpy(slot_info,avail_slot);
	strcpy(p2, p2_cre);
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
    char * str;
    str = strtok(buffer," ");
    if (strcmp(str,"authenticate")==0)
     ctr=1;
    if (strcmp(str,"available")==0) 
     ctr=2;
    switch(ctr)// switch case to check which type of message is received from server//
	   {
	   case 1:strcpy(u1,strtok(NULL," "));
	          auth = authenticate(u1,strtok(NULL," "),p1,p2);
		      if (auth == 1)
			    {
			    sen=send (child_socket, "success",8,0);
				if(sen<0)
				{
				 perror("Error in Send");
				 exit(1);
				} 
				cout<<"Phase 1: HEALTH CENTER SERVER SENDS THE RESPONSE OF success TO PATIENT WITH USERNAME: "<<u1<<endl;
				}
		      else
		       {
		        sen=send (child_socket, "failure",8,0);
				if(sen<0)
				{
				 perror("Error in Send");
				 exit(1);
				} 
				cout<<"Phase 1: HEALTH CENTER SERVER SENDS THE RESPONSE OF failure TO PATIENT WITH USERNAME: "<<u1<<endl;
			    close(child_socket);
				k=0;
			   }
			   break;
			   
	   case 2: int chk;
	           char * p;
			   cout<<"Phase 2:Health Center Server, receives a request for available time slots from patients with port number "<<ntohs(cli_addr.sin_port)<<" and IP address "<<inet_ntoa(*addr_list[0])<<endl;
			   sen=send(child_socket,avail_slot,strlen(avail_slot)+1,0);
			   cout<<"Phase 2:  Health Center Server sends available time slots to patient with user name "<<u1<<endl;
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
			   checkAppointmentDetails(p,slot_info,a_slot);
			   cout<<"Phase 2: Health Center Server receives a request for appointment "<<p<<" "<<a_slot<<" from patient with port number "<<ntohs(cli_addr.sin_port)<<" and username "<<u1<<endl;
			   if (strcmp(str,"selection")==0)
			    {
				 chk=doctorInfo(p,doc,app_info);
				 updateAvailable(avail_slot,p);
				 updateDoc(doc_info,p);
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
				    sen=send(child_socket,app_info,strlen(app_info)+1,0);
					cout<<"Phase 2: Health Center Server confirms the following appointment "<<p<<" "<<a_slot<<" to the patient with username "<<u1<<endl;
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
