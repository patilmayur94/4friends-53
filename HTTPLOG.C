// HTTPLOG developed by 4friends
// It works better on 'Trubo c'
#include<stdio.h>
#include<conio.h>
#define MAX 40
#define MAXIP 170

//Structure for Browser
typedef struct
{
	int count;
	int cnt[MAX];
	char method[MAX][MAX];
}bro;
bro b;

//Structure for IP Address
typedef struct
{
	int count;
	int cnt[MAXIP];
	char method[MAXIP][20];
}ipadd;
ipadd ip;

//Structure for Methods
typedef struct
{
	int count;
	int cnt[4];
	long int byte[4];
	char method[4][10];
}cntmeth;
cntmeth c;

//Structure for Hours
typedef struct
{
	int hrs;
	int hits;
}array;
array arr[24];

//Function for Adding new Method or Incrementing exisiting count
void addcnt(char *p)
{
	int i;
	for(i=0;i<c.count;i++)
		if(strcmp(p,c.method[i])==0)
			break;
	if(i==c.count)
	{
		c.cnt[c.count]=1;
		strcpy(c.method[c.count++],p);
	}
	else
		c.cnt[i]++;
}

//Function for Adding new IP or Incrementing existing count
void addip(char *p)
{
	int i;
	for(i=0;i<ip.count;i++)
		if(strcmp(p,ip.method[i])==0)
			break;
	if(i==ip.count)
	{
		ip.cnt[ip.count]=1;
		strcpy(ip.method[ip.count++],p);
	}
	else
		ip.cnt[i]++;
}

//Function for Bytes
void addbyte(char *p,int num)
{
	int i;
	for(i=0;i<c.count;i++)
		if(strcmp(p,c.method[i])==0)
			break;
	c.byte[i]+=num;
}

//Function for Adding new Browser or Incrementing existing count
void browser(char p[])
{
	int i;
	for(i=0;i<b.count && (strcmp(p,b.method[i])!=0);i++);
	if(i==b.count)
	{
		b.cnt[i]=1;
		strcpy(b.method[i],p);
		b.count++;
	}
	else
		b.cnt[i]++;
}

//Main function
void main()
{
	//Declarations
	FILE *fpin,*fpout;
	char str[1000],temp1[200],temp2[200],mod[200],t[200];
	int i,j,flag,k,ptr,num,flag1,ch,redir,success,hr;
	int clienterr,servererr,temp;
	long int tot_byte;
	array x;

	clrscr();

	//Initializations
	b.count=0;
	c.count=0;
	ip.count=0;
	redir=0;
	success=0;
	clienterr=0;
	servererr=0;
	ptr=0;

	for(i=0;i<24;i++)
	{
		arr[i].hits=0;
		arr[i].hrs=0;
	}

	//File opening for input and Output
	fpin=fopen("weblog.txt","r");
	fpout=fopen("stats.txt","w");

	//Loop to read log file
	while(!feof(fpin))
	{
		i=0;
		j=0;

		//Read line by line
		fgets(str,1000,fpin);

		while(str[i]!=' ')
		{
			temp1[j++]=str[i];
			i++;
		}

		temp1[j]='\0';
		addip(temp1);
		fputs(temp1,fpout);
		fputc('\t',fpout);
		i++;


		while(str[i++]!='[');
		i--;

		//Timestamp + Zone
		flag=0;
		flag1=0;
		k=0;
		hr=0;

		if(str[i++]=='[')
		{
			while(str[i]!=']')
			{
				if(str[i]==':'&&flag1==0)
				{
					flag1++;
					hr=((str[i+1]-48)*10)+(str[i+2]-48);
					arr[hr].hits++;
					if(arr[hr].hrs==0)
						arr[hr].hrs=hr;
				}
				if(str[i]==' ')
				{
					flag=1;
					temp2[k]='\0';
					fputs(temp2,fpout);
					fputc('\t',fpout);
				}
				else if(flag==0)
					temp2[k++]=str[i];
				i++;
			}
		}
		i+=2;

		//Method+Request URI+Protocol
		k=0;
		flag=0;

		if(str[i++]=='"')
		{
			while(str[i]!='"')
			{
				if(str[i]!=' '&&str[i]!='"')
					temp2[k++]=str[i];
				else
				{
					flag++;
					temp2[k]='\0';
					fputs(temp2,fpout);
					fputc('\t',fpout);
					k=0;
					if(flag==1)
					{
						addcnt(temp2);
						strcpy(mod,temp2);
					}

				}
				i++;
			}
			temp2[k]='\0';
			fputs(temp2,fpout);
			fputc('\t',fpout);
			k=0;
		}
		i+=2;

		//StatusCode
		j=0;
		while(str[i]!=' ')
		{
			temp1[j++]=str[i];
			i++;
		}
		temp1[j]='\0';

		if(strcmp(temp1[0],'4')==0)
			clienterr++;
		else if(strcmp(temp1[0],'3')==0)
			redir++;
		else if(strcmp(temp1[0],'2')==0)
			success++;
		else
			servererr++;

		fputs(temp1,fpout);
		fputc('\t',fpout);
		i++;

		//Bytes sent to client
		j=0;
		while(str[i]!=' ')
		{
			temp1[j++]=str[i];
			i++;
		}
		temp1[j]='\0';
		addbyte(mod,atoi(temp1));
		i++;

		num=0;
		while((num++)<3)
			while(str[i++]!='"');
		i--;

		//User Agent

		flag=0;
		k=0;
		flag1=0;
		if(str[i++]=='"')
		{
			while(str[i]!='"')
			{
				if((str[i]==' '||str[i]=='+')&&flag1==0)
				{
					flag1++;
					flag=1;
					temp2[k]='\0';
					fputs(temp2,fpout);
					fputc('\n',fpout);
					if(flag1==1)
						browser(temp2);
				}
				else if(flag==0)
					temp2[k++]=str[i];
				i++;
			}
			if(flag1==0)
			{
				temp2[k]='\0';
				fputs(temp2,fpout);
				fputc('\n',fpout);
				browser(temp2);
			}
		}
		i+=2;
		ptr++;
	}

	fclose(fpin);
	fclose(fpout);
	//Sorting the Browser name according to the hits
	flag=0;
	for(i=1;i<b.count && flag!=1;i++)
	{
		flag=1;
		for(j=0;j<b.count-i;j++)
		{
			if(b.cnt[j]>b.cnt[j+1])
			{
				flag=0;
				temp=b.cnt[j];
				b.cnt[j]=b.cnt[j+1];
				b.cnt[j+1]=temp;
				strcpy(t,b.method[j]);
				strcpy(b.method[j],b.method[j+1]);
				strcpy(b.method[j+1],t);
			}
		}
	}
	//Sorting the IP Address according to the hits
	flag=0;
	for(i=1;i<ip.count && flag!=1;i++)
	{
		flag=1;
		for(j=0;j<ip.count-i;j++)
		{
			if(ip.cnt[j]>ip.cnt[j+1])
			{
				flag=0;
				temp=ip.cnt[j];
				ip.cnt[j]=ip.cnt[j+1];
				ip.cnt[j+1]=temp;
				strcpy(t,ip.method[j]);
				strcpy(ip.method[j],ip.method[j+1]);
				strcpy(ip.method[j+1],t);
			}
		}
	}
	//Sorting the Hours from Timestamp
	flag=0;
	for(i=1;i<24 && flag!=1;i++)
	{
		flag=1;
		for(j=0;j<24-i;j++)
		{
			if(arr[j].hits>arr[j+1].hits)
			{
				flag=0;
				x=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=x;
			}
		}
	}

	//Menu for report
	while(1)
	{
		printf("\n\t\tMENU\n\n");
		printf("\n\n\t\t1.Methods used");
		printf("\n\n\t\t2.Browsers used");
		printf("\n\n\t\t3.Error and success rates");
		printf("\n\n\t\t4.IP Address Analysis");
		printf("\n\n\t\t5.Hour wise Analysis");
		printf("\n\n\t\t6.Exit");
		printf("\n\n\n\n\t\tEnter your choice: ");
		scanf("%d",&ch);

		switch(ch)
		{
			case 1:	clrscr();
				printf("\n\n");
				j=0;
				tot_byte=0;
				printf("\t\tMETHOD\t\tHITS\t\tBYTES\n\n");
				printf("\t\t******\t\t****\t\t*****\n\n\n");
				for(i=0;i<c.count;i++)
				{
					j+=c.cnt[i];
					tot_byte+=c.byte[i];
					printf("\t\t%s\t\t%d\t\t%ld\n\n",c.method[i],c.cnt[i],c.byte[i]);
				}
				printf("\n\n\t\tTotal Hits: %d",j);
				printf("\n\n\t\tTotal Bytes: %ld",tot_byte);
				printf("\n\n\n\n\n\n\t\tPress Enter to continue");
				break;

			case 2: clrscr();
				printf("\n\n");
				for(i=0;i<b.count;i++)
					printf("\t\t%s:\t\t\t%d\n",b.method[i],b.cnt[i]);
				printf("\n\n\t\tMost popular Browser: %s",b.method[i-1]);
				printf("\n\n\t\tSecond Most popular Browser: %s",b.method[i-2]);
				printf("\n\n\n\n\n\n\t\tPress Enter to continue");
				break;

			case 3: clrscr();
				j=0;
				for(i=0;i<c.count;j+=c.cnt[i],i++);
				printf("\n\n\t\tClient Errors: \t\t%d",clienterr);
				printf("\n\n\t\tServer Errors: \t\t%d",servererr);
				printf("\n\n\t\tSuccessful Response: \t%d",success);
				printf("\n\n\t\tRedirection: \t\t%d",redir);
				printf("\n\n\t\t--------------------------------------");
				printf("\n\n\t\tTotal: \t\t\t%d",j);
				printf("\n\n\n\n\n\n\t\tSuccess Rate: \t\t%f \%",(success)/(float)j*100);
				printf("\n\n\n\t\tRedirection Rate: \t%f \%",(redir)/(float)j*100);
				printf("\n\n\n\t\tError Rate: \t\t%f \%",(clienterr+servererr)/(float)j*100);
				printf("\n\n\n\n\n\n\t\tPress Enter to continue");
				break;

			case 4: clrscr();
				printf("\n\n");
				for(i=0;i<ip.count;i++)
					printf("\t\t%s\t\t\t%d\n",ip.method[i],ip.cnt[i]);
				printf("\n\n\t\tMost popular IP: %s",ip.method[i-1]);
				printf("\n\n\t\tSecond Most popular IP: %s",ip.method[i-2]);
				printf("\n\n\t\tTotal IP Addresses used: %d",ip.count);
				printf("\n\n\n\n\n\n\t\tPress Enter to continue");
				break;

			case 5: clrscr();
				printf("\n\n\t\tHOUR\t\tHITS");
				for(i=0;i<24;i++)
					printf("\n\t\t%d\t\t%d",arr[i].hrs,arr[i].hits);
				printf("\n\n\t\tMost popular HOUR: %d",arr[i-1].hrs);
				printf("\n\n\t\tSecond Most popular HOUR: %d",arr[i-2].hrs);
				printf("\n\n\n\n\n\n\t\tPress Enter to continue");
				break;

			case 6: exit(0);
			default:printf("\n\n\t\tPlease enter correct choice");
		}
		getch();
		clrscr();
	}
}