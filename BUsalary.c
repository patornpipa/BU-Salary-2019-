#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
typedef struct{
	int id,dumid,pay,dayw,ot,changeP,dayh,day,proday;
	char fname[20],lname[20],sex,emptype,bin[100],changeT;
	float net,tax,deduction,payot,netot,income;     //payroll deduction
}empinfo;
typedef struct node{
	empinfo data;
	struct node *right,*left;
}bst;
//===================================================[1] INPUT FILE============================================================
int inputmenu()
{
	int slinput=0;
	printf("===============select type of input===============\n");
	printf("	[1]Input by file.txt\n");
	printf("	[2]Input by yourself\n");
	printf("==================================================\n");
	printf("	Select menu : ");
	scanf("%d",&slinput);
	return slinput;
}
void insertbst(bst **root,empinfo newdata,int *cnt)
{
	bst *newnode,*run,*prev;
	newnode = malloc(sizeof(bst));
	newnode->data = newdata;
	newnode->left = NULL;
	newnode->right = NULL;
	if(*root == NULL)
	{
		*root = newnode;
	}
	else
	{
		run = *root;
		while(run != NULL)
		{
			prev = run;
			if(newnode->data.id > run->data.id)
			{
				run = run->right;
			}
			else if(newnode->data.id < run->data.id)
			{
				run = run->left;
			}
		}
		if(newnode->data.id > prev->data.id)
		{
			prev->right = newnode;
		}
		else if(newnode->data.id < prev->data.id)
		{
			prev->left = newnode;
		}
	}
	(*cnt)++;
}

void createbst(bst **root,int *cnt)
{
	FILE *inf;
	inf = fopen("employee.txt","r");
	empinfo newdata;
	fgets(newdata.bin,100,inf);
	newdata.day = 30;
	newdata.proday = 2;
	fscanf(inf,"%d%s%s %c%d %c",&newdata.id,&newdata.fname,&newdata.lname,&newdata.sex,&newdata.pay,&newdata.emptype);
	while(!feof(inf))
	{
		insertbst(&*root,newdata,cnt);
		fscanf(inf,"%d%s%s %c%d %c",&newdata.id,&newdata.fname,&newdata.lname,&newdata.sex,&newdata.pay,&newdata.emptype);	
		
	}
	fclose(inf);
}
int checkid(bst *root,empinfo newdata)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		checkid(run->left,newdata);
		if(newdata.id == run->data.id)
		{
			return 1;
		}
		checkid(run->right,newdata);
	}
}

void inputkeyboard(bst **root,int *cnt)
{
	int check=-1;
	char ans='Y';
	empinfo newdata;
	do
	{
		printf("Enter Your ID : ");
		scanf("%d",&newdata.id);
		check = checkid(*root,newdata);
		if(check != 1)
		{
			printf("Enter Your First Name : ");
			scanf("%s",&newdata.fname);
			printf("Enter Your Lastname : ");
			scanf("%s",&newdata.lname);
			printf("Enter Your sex [F]emale,[M]ale : ");
			scanf(" %c",&newdata.sex);
			printf("Enter Your employee type [M]onth,[D]ay : ");
			scanf(" %c",&newdata.emptype);
			printf("Enter Your salary : ");
			scanf("%d",&newdata.pay);
			printf("Do you want to input data again? [Y]es,[N]o : ");
			scanf(" %c",&ans);
			newdata.day = 30;
			newdata.proday = 2;
			printf("==================================================\n");
			insertbst(&*root,newdata,cnt);
		}
		else
		{
			printf("duplicate ID !!!!\n");
		}
	}while(toupper(ans) == 'Y');                                     
}
//=============================================================================================================================
//===================================================[2] INPUT DAY OF WORK=====================================================
int daywmenu()
{
	int sldayw=0;
	printf("===============select type of input===============\n");
	printf("	[1]Input by file.txt\n");
	printf("	[2]Input by yourself\n");
	printf("==================================================\n");
	printf("	Select menu : ");
	scanf("%d",&sldayw);
	return sldayw;
}
void insertdayw(bst *root,empinfo newdata)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		insertdayw(run->left,newdata);
		if(newdata.dumid == run->data.id)
		{
			run->data.dayw = newdata.dayw;
			run->data.ot = newdata.ot;
		}
		insertdayw(run->right,newdata);
	}
}
void filedayw(bst *root)
{
	FILE *inf;
	inf = fopen("work.txt","r");
	empinfo newdata;
	fgets(newdata.bin,100,inf);
	fscanf(inf,"%d%d%d",&newdata.dumid,&newdata.dayw,&newdata.ot);
	while(!feof(inf))
	{
		insertdayw(root,newdata);
		fscanf(inf,"%d%d%d",&newdata.dumid,&newdata.dayw,&newdata.ot);	
		
	}
	fclose(inf);
}
void inputdayw(bst *root)
{
	int check = 0;
	char ans;
	empinfo newdata;
	do
	{
		printf("Enter Your ID : ");
		scanf("%d",&newdata.dumid);
		newdata.id = newdata.dumid;
		check = checkid(root,newdata);
		if(check == 1)
		{
			printf("Enter Your Day of work (0-30) : ");
			scanf("%d",&newdata.dayw);
			printf("Enter Your Overtime (hrs): ");
			scanf("%d",&newdata.ot);
			insertdayw(root,newdata);
			printf("Do you want to input data again? [Y]es,[N]o : ");
			scanf(" %c",&ans);
		}
		else
		{
			printf("Not found please try again!!!!\n");
			inputdayw(root);
		}
		
	}while(toupper(ans) == 'Y');                                    
}

//=============================================================================================================================
//===================================================[3] Change pay of employee=================================================================
int changemenu()
{
	int slchange=0;
	printf("===============select type of input===============\n");
	printf("	[1]Change pay of employee\n");
	printf("	[2]Delete employee\n");
	printf("==================================================\n");
	printf("	Select menu : ");
	scanf("%d",&slchange);
	return slchange;
}
void printpay(bst *root,empinfo newdata)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		printpay(run->left,newdata);
		if(newdata.dumid == run->data.id)
		{
			printf("----------------------- Before -----------------------\n");
			printf("%s %s\n",run->data.fname,run->data.lname);
			printf("Employeetype: %c\n",run->data.emptype);
			printf("Pay = %d\n",run->data.pay);	
		}
		printpay(run->right,newdata);
	}
}
int insertpay(bst *root,empinfo newdata)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		insertpay(run->left,newdata);
		if(newdata.dumid == run->data.id)
		{
			run->data.emptype = newdata.changeT;
			run->data.pay = newdata.changeP;	
		}
		insertpay(run->right,newdata);
	}
}
bst* changeP(bst *root)
{
	int check = 0;
	empinfo newdata;
	printf("Enter your ID: ");
	scanf("%d",&newdata.dumid);
	newdata.id = newdata.dumid;
	check = checkid(root,newdata);
	if(check == 1)
	{
		printpay(root,newdata);
		printf("----------------------- After -----------------------\n");
		printf("Employee type [D]ay,[M]onth: ");
		scanf(" %c",&newdata.changeT);
		newdata.changeT=toupper(newdata.changeT);
		if(toupper(newdata.changeT) == 'D' || toupper(newdata.changeT) == 'M')
		{
			printf("Income = ");
			scanf("%d",&newdata.changeP);
			insertpay(root,newdata);
			return root;
		}
		else
			printf("Please key D or M.\n");
			root = changeP(root);
	}
	else
		printf("Not found please try again\n");
		root = changeP(root);
}
void deleteNode(bst **root,bst *prev,bst *run)
{
	bst *temp;
	//delete leaf node
	if(run->left == NULL && run->right == NULL)
    {
    	if(run == *root)  
    	   *root = NULL;
    	else              
		   if(prev->left == run)  
		      prev->left = NULL;
		   else
		      prev->right = NULL;   
	}
	else if(run->left != NULL && run->right != NULL) 
	{
		prev = run;
		temp = run->left;
		while(temp->right != NULL)
		{
			prev = temp;
			temp = temp->right;
		}
		run->data = temp->data;  
		if(prev == run)          
		   prev->left = temp->left;
		else
		   prev->right = temp->left;  
		free(temp);   
	}
	else if(run->left != NULL) 
	{
		if(run == *root)
		   *root = run->left;
		else 
		{
			if(prev->left == run)        
		       prev->left = run->left;   
		    else                        
		       prev->right = run->left;   
		}
	}
	else 
	{
		if(run == *root)
		   *root = run->right;
		else 
	    {
	    	if(prev->left == run)           
	    	   prev->left = run->right;    
	    	else                             
	    	   prev->right = run->right;     
		}
		free(run);	
		
	}
}

void deleteBST(bst **root,int *cnt)
{
	bst *run,*prev;
	run = *root;
	int target=0;
	char sure;
	printf("Enter ID to delete : ");
	scanf("%d",&target);
	while(run != NULL && target != run->data.id)
	{
		prev = run;
		if(target > run->data.id)
		   run = run->right;
		else
		   run = run->left;		
	}
	if(run == NULL)
	{
		printf("\nTarget not found!");
		//return 0;
	}
	else
	{
		printf("\nTarget = %d found!\n",target);
		printf("Do you sure to Delete this ID ? Y/N : ");
		scanf(" %c",&sure);
		if(toupper(sure) == 'Y')
		{
			(*cnt)--;
	    	deleteNode(&*root,prev,run);	
		}
		else if(toupper(sure) == 'N')
		{
			printf("goodbye !!!\n");
			start();
		}
    }
}


//=============================================================================================================================
//===================================================[4] Calculate Personal Income Tax=================================================================
bst* calculatepay(bst *root)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		calculatepay(run->left);
		run->data.dayh = run->data.day - run->data.dayw;
		if(toupper(run->data.emptype) == 'D')    //type D OT 1.0
		{
			run->data.income = run->data.dayw * run->data.pay;
			run->data.payot = (float)run->data.pay / 8;  //calculate OT per hour
			run->data.netot = run->data.ot * run->data.payot;
			run->data.net = run->data.income + run->data.netot;
		}
		else if(toupper(run->data.emptype) == 'M')  //type M OT 1.5
		{
			run->data.income = run->data.pay;
			run->data.payot = (((float)run->data.pay / run->data.day) / 8) * 1.5;  //calculate OT per hour
			run->data.netot = run->data.ot * run->data.payot;
			if(run->data.dayh <= 2)  
				run->data.net = run->data.pay + run->data.netot;
			else  
			{
				run->data.deduction = ((float)run->data.pay/run->data.day)*(run->data.dayh - run->data.proday);
				run->data.net = ((float)run->data.pay  + run->data.netot) - run->data.deduction;
			}			
		}
		calculatepay(run->right);
	}
	return root;
}





//=============================================================================================================================
//===================================================[5] PRINT=================================================================
int printmenu()
{
	int slprint=0;
	printf("===============select type of output===============\n");
	printf("	[1]print to textfile\n");
	printf("	[2]print to display output\n");
	printf("	[3]printslip to display output\n");
	printf("==================================================\n");
	printf("	Select menu : ");
	scanf("%d",&slprint);
	return slprint;
}
void printdata(bst *root)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		printdata(run->left);
		printf("====================================================================\n");
		printf("%-7d%-15s %-15s%",run->data.id,run->data.fname,run->data.lname);
		if(toupper(run->data.sex) == 'F')
			printf("Female");
		else
			printf("Male  ");
		printf("%10d",run->data.pay);
		if(toupper(run->data.emptype) == 'D')
			printf("       Day  \n");
		else
			printf("       Month\n");
		printdata(run->right);	
	}	
}

void printfile(bst *root,FILE *out)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		printfile(run->left,out);
		fprintf(out,"====================================================================\n");
		fprintf(out,"%-7d%-15s %-15s%",run->data.id,run->data.fname,run->data.lname);
		if(toupper(run->data.sex) == 'F')
			fprintf(out,"Female");
		else
			fprintf(out,"Male  ");
		fprintf(out,"%10d",run->data.pay);
		if(toupper(run->data.emptype) == 'D')
			fprintf(out,"       Day  \n");
		else
			fprintf(out,"       Month\n");
		printfile(run->right,out);	
	}	
}
void printslip(bst *root)
{
	bst *run;
	run = root;
	if(run != NULL)
	{
		printslip(run->left);
		printf("====================================================================\n");
		printf("%-7d%-15s %-15s%",run->data.id,run->data.fname,run->data.lname);
		if(toupper(run->data.sex) == 'F')
			printf("Female");
		else
			printf("Male  ");
		printf("%10d",run->data.pay);
		if(toupper(run->data.emptype) == 'D')
			printf("       Day  \n");
		else
			printf("       Month\n");
		printf("------------------------ Positive list ------------------------\n");
		printf("Day of work(30) = %d, OT(hrs) = %d\n",run->data.dayw,run->data.ot);
		printf("Work income = %10.2f, OT income = %10.2f\n",run->data.income,run->data.netot);
		if(toupper(run->data.emptype) == 'M')
		{
			printf("------------------------ Negative list ------------------------\n");
			printf("Quota of holidays = 2 day, leave working = %d day\n",run->data.dayh);
			printf("Reduction of salary = %.2f\n",run->data.deduction);
		}
		printf("Net income = %.2f\n",run->data.net);
		printslip(run->right);	
	}	
}

//=============================================================================================================================
int start()
{
	int sl=-1;
	printf("===============Welcome to BU Salary===============\n");
	printf("        [1] Input employee details\n");
	printf("        [2] Input day of work\n");
	printf("        [3] Change pay employee and Delete employee\n");
	printf("       	[4] Calculate Personal Income\n");
	printf("    	[5] Print salary slip\n");
	printf("	[0] Exit Program\n");
	printf("==================================================\n");
	printf("	Select Menu: ");
	scanf("%d",&sl);
	return sl;
}

int main()
{
	FILE *out;
	out = fopen("printout.txt","a");
	bst *root = NULL;
	int cnt=0,sl=-1,slinput=0,slprint=0,sldayw=0,slchange=0,day,i=1;
	
	while(sl != 0)
	{
		sl = start();
		if(sl == 1) //[1] Input employee details
		{
			slinput = inputmenu();
			if(slinput == 1)
			{
				createbst(&root,&cnt);
				sl=-1;
			}
			else if(slinput == 2)
			{
				inputkeyboard(&root,&cnt);
				sl=-1;
			}
			else
			{
				printf("wrong menu please try again !!!\n");
				sl=-1;
			}
		}
		else if(sl == 2) //[2] Input day of work
		{
			sldayw = daywmenu();
			if(sldayw == 1)
			{
				filedayw(root);
				sl=-1;
			}
			else if(sldayw == 2)
			{
				inputdayw(root);
				sl=-1;
			}
			else
			{
				printf("wrong menu please try again !!!\n");
				sl=-1;
			}	
		}
		else if(sl == 3) //[3] Change pay of employee
		{
			slchange = changemenu();
			if(slchange == 1)
			{
				root = changeP(root);
				sl=-1;
			}
			else if(slchange == 2)
			{
				deleteBST(&root,&cnt);
				sl=-1;
			}
		}
		else if(sl == 4)  //[4] Calculate Persanal Income Tax
		{
			root = calculatepay(root);
		}
		else if(sl == 5)  //[5] Print salary slip
		{
			slprint = printmenu();
			if(slprint == 1)
			{
				printfile(root,out);
				fprintf(out,"Total emp = %d\n",cnt);
				sl=-1;
			}
			else if(slprint == 2)
			{
				printf("------------------------- Employee Details -------------------------\n");
				printdata(root);
				printf("--------------------------------------------------------------------\n");
				printf("Total emp = %d\n",cnt);
				sl=-1;
			}
			else if(slprint == 3)
			{
				printslip(root);
				printf("Total emp = %d\n",cnt);
				sl=-1;
			}
			else
			{
				printf("wrong menu please try again !!!\n");
				sl=-1;
			}	
		}
		
	}
	printf("See you agin. Good bye!!!\n");
	fclose(out);
	return 0;
}

