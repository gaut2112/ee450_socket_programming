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

using namespace std;

int main()
{
int sel=2,i=1;
char a[150]="1 Tue 01pm,2 Mon 03pm,3 Thu 02pm,4 Wed 10am,";
char a_temp[150]="", d_temp[150];
char * p, *t;
//strcpy(a_temp,a);
//strcpy(d_temp,d);
p= strtok(a,",");
cout<<p<<endl;
if (sel!=i)
 {
  strcat(a_temp,p);
  strcat(a_temp,",");
 }
 
while(p!=NULL)
 {
  ++i;
  p= strtok(NULL,",");
  cout<<p<<endl;
  if (p == NULL)
  {
  break;
  }
  if (sel!=i)
 {
  cout<<i<<" ENter if"<<endl;
  strcat(a_temp,p);
  strcat(a_temp,",");
 }
//cout<<a_temp<<endl;
 }
 printf("%s",a_temp);
  printf("%d",strlen(a_temp));

 printf(" hhhhhhhhhhhhhh");
//cout<<"Hi";
//cout<<strlen(a_temp)<<endl;
//cout<<"output:"<<a_temp<<endl;
//char ch;
//cin>>ch;
return 0;
}
 
 