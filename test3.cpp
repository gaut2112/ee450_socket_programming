#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
using namespace std;


int main()
{
char a[150]="1 Tue 01pm,2 Mon 03pm,3 Thu 02pm,4 Wed 10am,";
char a_temp[150]="";
char a_new[150]="";
int s=3;
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

return 0;

}
