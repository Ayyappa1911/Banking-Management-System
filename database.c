#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

struct db
{
    int acc_no;
    char name[100][100];
    int pwd;
    int ph_no;
    int acc_type;
    int balance;
};


int main()
{
    int fd = open("Bank_db",O_CREAT|O_RDWR,0744);
    char Sano[] = "202001";
    write(fd,Sano,sizeof(Sano));
    return 0;
}