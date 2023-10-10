#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include<sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PORT 8080

int db;

struct db
{
    int acc_no;
    long int name;
    int pwd;
    long int ph_no;
    int acc_type;
    int balance;
	int nou;
};

long int chartoint(char* name)
{
	long int a = 0;
	for(int i = 0;i<100;i++)
	{
		if(*(name+i) == '\0')
		{
			return a;
		}
		a = a * 26 + *(name+i) - 'a';
	}
	return a;
}

void int2char(long int n, char *name)
{
	int k = n;
	int s = 0;
	while(k>0)
	{
		k = k / 26;
		s += 1;
	}
	
	int k1 = n;
	for(int i = s-1;i>=0;i--)
	{
		*(name+i) = k1%26 + 'a';
		k1 = k1/26;
	}
}

void int2char1(int n,char *bal)
{
	int k = n;
	int s = 0;
	while(k>0)
	{
		k = k / 10;
		s += 1;
	}
	
	int k1 = n;
	*(bal + s) = '\0';
	for(int i = s-1;i>=0;i--)
	{
		*(bal+i) = k1%10 + '0';
		k1 = k1/10;
	}
}

int pwdcmp(char *pwd,char* pwde)
{
	for(int i = 0;i<4;i++)
	{
		if(*(pwd+i) != *(pwde+i))
		{
			return 0;
		}
	}
	return 1;
}

long int Convint(char *buffer)
{
	int i = 0;
	int val = 0;
	while(*(buffer+i) != 0)
	{
		val *= 10;
		val += *(buffer+i) - 48;
		i++;
	}
	return val;
}

char* inttochar(int k)
{
	static char Arr1[6];
	int i = 5;
	while(k>0)
	{
		Arr1[i--] = k%10 + '0';
		k /= 10;
	}

	printf("%s\n",Arr1);

	return Arr1;

}

int Acno_Generator() // Apply Lock Here.
{
	int db1 = dup(db);
	lseek(db1,0,SEEK_SET);
	char ACC[6];
	read(db1,ACC,6);
	lseek(db1,0,SEEK_SET);
	write(db1,inttochar(Convint(ACC)+1),6);
	printf("%s\n",inttochar(Convint(ACC)+1));
	printf("%ld\n",Convint(ACC));
	return Convint(ACC);
}

void Createaccount(int soc)
{
	char type[] = "\nEnter the no for selecting the type of the Account\n1) Normal User Account\n2) Joint User Account\n3) Administrative Account\n\n";
	write(soc,type,sizeof(type));
	char b[1] = {0};
	read(soc,b,1);
	int typ = Convint(b);
	printf("%d\n",typ);
	long int mobileno;
	int no_of_users;
	char pwd[10];
	// char Name[100];
	long int name;
	if(Convint(b) < 0 || Convint(b) > 3)
	{
		write(soc,"Invalid Selection",sizeof("Invalid Selection"));
	}
	if(Convint(b) == 1)
	{
		char n1[] = "\nEnter the name of the Account holder : ";
		write(soc,n1,sizeof(n1));
		char b2[100] = {0};
		read(soc,b2,100);
		// for(int i= 0;i<100;i++)
		// {
		// 	*(Name+i) = b2[i];
		// } 
		name = chartoint(b2);
		printf("name %d\n",name);

		char phno[] = "\nEnter the Mobile no of the Account holder : ";
		write(soc,phno,sizeof(phno));
		char b3[10] = {0};
		read(soc,b3,10); 
		mobileno = Convint(b3);
		no_of_users = 1;

		char pass[] = "\nEnter the Password for the Bank Account: ";
		write(soc,pass,sizeof(pass));
		char b4[4] = {0};
		read(soc,b4,4);

		// char pwd[4] = {b4[0],b4[1],b4[2],b4[3]};
		int pwd = Convint(b4);

		printf("The password set is %s %ld\n",b4,pwd);
		
		// pwd = Convint(b4);
		int p = Acno_Generator();
		// printf(" p = %d\n",p);
		// printf("conv int from %s to %d \n",Name,chartoint(Name));
		// char name[100];
		// int2char(chartoint(Name),name);
		// printf("%s\n",name);
		struct db Account = {p,name, pwd ,mobileno,typ,0,no_of_users};
		lseek(db,6,SEEK_SET);
		write(db,&Account,sizeof(Account));
	}
	else if(Convint(b) == 2)
	{
		char n1[] = "\nEnter the no of Users will be holding this account : ";
		write(soc,n1,sizeof(n1));
		char b1[10] = {0};
		read(soc,b1,10);
		no_of_users = Convint(b1); 
		printf("%d\n",no_of_users);

		for(int k = 0; k< no_of_users; k++ )
		{
			printf("%d %d\n",k,no_of_users);
			char n1[] = {'\n','E','n','t','e','r',' ','t','h','e',' ','n','a','m','e',' ','o','f',' ','t','h','e',' ',k+'1',' ','t','h',' ','A','c','c','o','u','n','t',' ','h','o','l','d','e','r',' ',':',' '};
			write(soc,n1,sizeof(n1));
			char b2[100] = {0};
			read(soc,b2,100);
			// for(int i= 0;i<100;i++)
			// {
			// 	*(*(Name+k)+i) = b2[i];
			// } 

			char phno[] = {'\n','E','n','t','e','r',' ','t','h','e',' ','m','o','b','i','l','e',' ','n','u','m','b','e','r',' ','o','f',' ','t','h','e',' ',k+'1',' ','t','h',' ','A','c','c','o','u','n','t',' ','h','o','l','d','e','r',' ',':',' '};
			write(soc,phno,sizeof(phno));
			char b3[10] = {0};
			read(soc,b3,10); 
			mobileno = Convint(b3);
			printf("%d %d\n",k,no_of_users);
		}

		char pass[] = "\nEnter the Password for the Bank Account: ";
		write(soc,pass,sizeof(pass));
		char b4[4] = {0};
		read(soc,b4,4);
		
		int pwd = Convint(b4);
		//pwd = Convint(b4);
		// strcpy(pwd ,b4);

		struct db Account = {(Acno_Generator()),0,pwd,mobileno,typ,0,no_of_users};
				// struct db Account = {(Acno_Generator()),*Name,pwd,*mobileno,typ,0,no_of_users};
		lseek(db,0,SEEK_END);
		write(db,&Account,sizeof(Account));

		// for(int i = 0;i<100;i++)
		// {
		// 	printf("%s",*(Name+i));
		// }
	}
	else
	{

	}

}

void services(int soc,int accno)
{
	int service = 1;
	while(service)
	{
		char buf[1024] = {0};
		char select[] ="\n\n\nEnter the no for the following Services:\n\n1) Deposit\n2) Withdraw\n3) Balance Enquiry\n4) Password Change\n5) View details\n6) Exit \n\n\n";
		write(soc,select,sizeof(select));
		char b1[1] = {0};
		read(soc,b1,1);

		if(Convint(b1) == 1)
		{
			char A1[] = "\n\n Enter the Amount to be deposited: ";
			write(soc,A1,sizeof(A1));
			char amount[10] = {0};
			read(soc,amount,1024);


			int db1 = dup(db);
			lseek(db1,0,SEEK_SET);
			char ACC[6];
			read(db1,ACC,6);

			struct db tmp;
			while(1)
			{
				read(db1,&tmp,sizeof(tmp));
				if(tmp.acc_no == accno)
				{
					break;
				}
			}
			tmp.balance += Convint(amount);
			lseek(db1,-sizeof(tmp),SEEK_CUR);

			write(db1,&tmp,sizeof(tmp));

		}
		else if(Convint(b1) == 2)
		{
			char A1[] = "\n\n Enter the Amount to be withdrawn: ";
			write(soc,A1,sizeof(A1));
			char amount[10] = {0};
			read(soc,amount,1024);


			int db1 = dup(db);
			lseek(db1,0,SEEK_SET);
			char ACC[6];
			read(db1,ACC,6);

			struct db tmp;
			while(1)
			{
				read(db1,&tmp,sizeof(tmp));
				if(tmp.acc_no == accno)
				{
					break;
				}
			}
			tmp.balance -= Convint(amount);
			lseek(db1,-sizeof(tmp),SEEK_CUR);

			write(db1,&tmp,sizeof(tmp));
		}
		else if(Convint(b1) == 3)
		{
			printf("yup\n");
			int db1 = dup(db);
			lseek(db1,0,SEEK_SET);
			char ACC[6];
			read(db1,ACC,6);

			struct db tmp;
			while(1)
			{
				read(db1,&tmp,sizeof(tmp));
				if(tmp.acc_no == accno)
				{
					break;
				}
			}

			// char balance[10] = inttochar(tmp.balance);
			printf("balance %d\n",tmp.balance);
			char bal[10];
			int2char1(tmp.balance,bal);

			printf("bal in char %s\n",bal);

			// write(soc,(tmp.balance),sizeof((tmp.balance)));
			write(soc,bal,sizeof(bal));
		}
		else if(Convint(b1) == 4)
		{
			char p[] = "\nEnter the Old password : ";
			write(soc,p,sizeof(p));
			char oldpwd[4] =  {0};
			read(soc,oldpwd,sizeof(oldpwd));

			int opwd = Convint(oldpwd);

			int db1 = dup(db);
			lseek(db1,0,SEEK_SET);
			char ACC[6];
			read(db1,ACC,6);

			struct db tmp;
			while(1)
			{
				read(db1,&tmp,sizeof(tmp));
				if(tmp.acc_no == accno)
				{
					break;
				}
			}

			// if(pwdcmp(tmp.pwd,(oldpwd)))
			if(tmp.pwd == opwd)
			{
				char p[] = "\nEnter the new password : ";
				write(soc,p,sizeof(p));
				char newpwd[4] =  {0};
				read(soc,newpwd,sizeof(newpwd));
				// strcpy(tmp.pwd,(newpwd));
				char pwd1[5] = {newpwd[0],newpwd[1],newpwd[2],newpwd[3],'\0'};
				int p1 = Convint(pwd1);
				tmp.pwd = p1;
				printf("new pwd %d %s\n",tmp.pwd,pwd1);
			}

			lseek(db1,-sizeof(tmp),SEEK_CUR);

			write(db1,&tmp,sizeof(tmp));
		}
		else if(Convint(b1) == 5)
		{
			
		}
		else if(Convint(b1) == 6)
		{
			service = 0;
		}
	}
}

int authenticate(int acc,int pwd)
{
	// printf("%d %s",acc,pwd);
	printf("%d %d\n",acc,pwd);
	int db1 = dup(db);
	lseek(db1,0,SEEK_SET);
	char tmp[6];
	read(db1,tmp,6);

	if(acc >= 202001)
	{
		struct db temp;
		int y = 1;
		// lseek(db1,6,SEEK_SET);
		while(y)
		{
			y = read(db1,&temp,sizeof(temp));
			// printf("%d",)
			// printf("\n%d hey\n",temp.pwd);
			printf("%d %d yo",acc,(temp.acc_no));
			if(temp.acc_no == (acc))
			{
				// if(pwdcmp((temp.pwd), pwd))
				if(pwd == temp.pwd)
				{
					return 1;
				}
				else
				{
					printf("\n%d \n%d oops \n",temp.pwd,pwd);
					// for(int i = 0 ; i< 100;i++)
					{
						printf("%ld\n",(temp.name));
					}
					return 0;
				}
			}
		}
	}
	return 0;
}

void *myThreadFun(void *vargp)
{
	int *socket = (int *)vargp;
	int new_socket = *socket;
	char hello[] = "                  ***Welcome to XYZ Bank***\n\n\n\nEnter 1 for Login and 2 for Creating an Account : \0";

	char invld[] = "Invalid Option Please select from given options\0";

	int logflg = 1;

	while(logflg)
	{
		char buffer[1024] = { 0 };
		write(new_socket,hello,sizeof(hello));

		read(new_socket,buffer,1024);
		printf("%s\n",buffer);
		logflg = 0;
		if(buffer[0] == '1')
		{
			char acno[] = "Enter your Account no : \0";
			write(new_socket,acno,sizeof(acno));
			char buffer1[6] = { 0 };
			read(new_socket,buffer1,6);
			printf("%s\n",buffer1);
	
			int accno = Convint(buffer1);
			printf("%d\n",accno);

			char buffer2[4] = { 0 };
			char pd[] = "Enter your Password : \0";
			
			write(new_socket,pd,sizeof(pd));
			read(new_socket,buffer2,4);
			printf("pass = %s\n",buffer2);

			char pwd[5] = {buffer2[0],buffer2[1],buffer2[2],buffer2[3],'\0'};

			printf("pwd %s\n",pwd);
			if(authenticate(accno,Convint(pwd)))
			{
				services(new_socket,accno);
			}
			else
			{
				write(new_socket,"Invalid Credentials!\n",sizeof("Invalid Credentials!\n"));
				char uf[10] = {0};
				read(new_socket,uf,sizeof(uf));
			}
			logflg = 1;
		}
		else if(buffer[0] == '2')
		{
			Createaccount(new_socket);
			logflg = 1;
		}
		else
		{
			write(new_socket,invld,sizeof(invld));
			logflg = 1;
		}
	}

}

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char* hello = "Hello from server";

	db = open("Bank_db",O_RDWR);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
		== 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	while(1) 
	{
		if (listen(server_fd, 3) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
		if ((new_socket
			= accept(server_fd, (struct sockaddr*)&address,
					(socklen_t*)&addrlen))
			< 0) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		printf("Connected Succefully\n");
		pthread_t thread_id;
    	pthread_create(&thread_id, NULL, myThreadFun, (void *)&new_socket);
    
	}
	return 0;
}
