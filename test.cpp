#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;




int doctorInfo(char *sel, char *d, char k[50], int free[6])
{
int s = atoi(sel);
int m = s;
cout<<"value of s"<<s<<endl;
cout<<free[2]<<endl;
if(s>6)
 {
  return 0;
 }
if (free[--m]==1)
 {
  return 7;
 }  
char *ctr=NULL;
ctr=strtok(d,",");
if (s==1)
 {
  cout<<"Enter 1st if"<<endl<<ctr<<endl;
  k=ctr;
  return s;
 }
for(int i=2;i<=6;i++)
 {
  ctr=strtok(NULL,",");
  cout<<i<<" "<<ctr<<endl;
  if (s==i)
   {
    cout<<ctr<<" "<<i<<" "<<s<<endl;
	strcpy(k,ctr);
	return s;
   }
 }
}
int main()
{
int free_slot[6]={0,1,0,0,0,0};
char s[150]="";
char d[100]="";
char curr[22]="";
char * ctr=NULL;
fstream slot("availabilities.txt",ios::in);
if (slot.is_open())
 {
  for(int i=0;i<=5;++i)
  {
   slot.getline(curr,50);
   cout<<curr<<endl;
   strcat(curr,".");
   cout<<curr<<endl;
   ctr = strtok(curr," .");
   strcat(s, ctr);
   strcat(s, " ");
   //strcat(d, ctr);
   //strcat(d, " ");
   cout<<i<<" "<<d<<endl;
   for(int j=1;j<=4;j++)
    { 
	 ctr = strtok(NULL," .");
	 cout<<ctr<<endl;
	 if(j==1 | j==2)
	 strcat(s, ctr);
	 if(j==3|j==4)
	 strcat(d, ctr);
	 if(j==1)
	  strcat(s," ");
	 if(j==3)
	  strcat(d," ");
	}
   //cout<<"outside"<<strtok(NULL,NULL)<<endl;
   strcat(s,",");
   strcat(d,",");
   cout<<d<<endl;
  }
 }
else
 {
  perror("Error Opening availablities.txt");
  exit(1);
 }
char * print;
cout<<"No Day Time"<<endl;
print = strtok(s,",");
cout<<print<<endl;
for (int i=0;i<=4;i++)
{
print = strtok(NULL,",");
cout<<print<<endl;
}
char ch;
cin>>ch
char * cc = strtok(ch," ");
cc=strtok(NULL," ");
cout<<cc<<endl;
int result;
char ai[50]="";
cout<<atoi(cc);
result = doctorInfo(cc,d,ai,free_slot);
cout<<ai<<endl<<result;
return 0;
}

void updateSlot(char a[150], char d[150], char * sel)
{
int s=atoi(sel),i=1, k=1;
char a_temp[150], d_temp[150];
char * p, *t;
//strcpy(a_temp,a);
//strcpy(d_temp,d);
p= strtok(a,",");
if (s!=i)
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
  if (s!=i)
 {
  cout<<i<<" ENter if"<<endl;
  strcat(a_temp,p);
  strcat(a_temp,",");
 }
 }
 cout<<"Modified :"<<a_temp<<endl;
 strcpy(a,a_temp);
 p= strtok(d,",");
if (s!=k)
 {
  strcat(d_temp,p);
  strcat(d_temp,",");
 }
while(p!=NULL)
 {
  ++k;
  t= strtok(NULL,",");
  cout<<p<<endl;
  if (t == NULL)
  {
  break;
  }
  if (s!=k)
 {
  cout<<k<<" ENter if"<<endl;
  strcat(d_temp,t);
  strcat(d_temp,",");
 }
 }
 cout<<"Modified :"<<d_temp<<endl;
 strcpy(d,d_temp);
}
