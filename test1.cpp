#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main()
{
fstream f("doc1.txt",ios::in);
char d[100]="";
char buf[100]="";
char *ch;
for(int i=0;i<3;i++)
{
 f.getline(buf,20);
strcat(d,strtok(buf," "));
strcat(d," ");
strcat(d,strtok(NULL," "));
strcat(d,":");
}
cout<<d<<endl;
char *c[3],*s,ins_s[5];
char k[]="insurance1";
c[0] = strtok(d,":");
c[1] = strtok(NULL,":");
c[2] = strtok(NULL,":");
for(int i=0;i<3;i++)
 {
   s = strtok(c[i]," ");
   if(strcmp(k,s)==0)
    {
	 strcpy(ins_s,strtok(NULL," "));
	}
 }
 cout<<ins_s<<endl;
f.close();
char ins[20];
fstream fi("patient1insurance.txt",ios::in);
fi.getline(ins,20);
cout<<"insurance "<<ins;
return 0;
}