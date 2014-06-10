//Birthday Paradox by '4friends'
/*
Problem Description : Verification for correctness of birthday paradox using random experiments
Input : No. of persons
Output : Percentage chance of having same birthday for given input
*/

#include<stdio.h>
#include<timeb.h>
#include<stdlib.h>
#include<time.h>
#include<dos.h>

//array for storing days of each month
int month[12]={31,29,31,30,31,30,31,31,30,31,30,31};

void main()
{
	int num,i,j,k,flag,tot=0;
	int *nummon,*numday;
	time_t t;
	clrscr();

	printf("\nEnter the number of people: ");
	scanf("%d",&num);

	nummon=(int *)malloc(num*sizeof(int));
	numday=(int *)malloc(num*sizeof(int));
	srand((unsigned) time(&t));

	//loop k for number of experiments
	for(k=0;k<10000;k++)
	{
		flag=0;
		/*loop for random generation of birthday(month and day)
		for each person*/
		for(i=0;i<num;i++)
		{
			nummon[i]=(rand()%12)+1;
	 //	for(i=0;i<num;i++)
			numday[i]=(rand()%(month[nummon[i]-1]))+1;
		}
		/*loop for comparison of birthdays*/
		for(i=0;i<num-1 && flag==0;i++)
		{
			for(j=i+1;j<num;j++)
			{
				//condition for success
				if(nummon[i]==nummon[j] && numday[i]==numday[j])
				{
					//termination of experiment instance*/
					flag=1;
					tot++;
					break;
				}
			}
		}
	}
	printf("\nPercentage chance is %.4f\n",(float)tot/100);
	getch();
}