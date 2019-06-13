void structExpreRead(struct expre expre1[1000])
{
	int i=0;
	while(expre1[i].end!=1)
	{
		if(expre1[i].ifNum==1)
		{
			printf("%f ",expre1[i].num);
			i++;
		}
		else
		{
			printf("%c ",expre1[i].sign);
			i++;
		}
	}
}
//字符串转换成浮点数
double stringToDouble(char a[1000])
{
	int i=0;
	char *p=(char*)malloc(1000*sizeof(char));
	int top=0;
	double sum=0;
	double mul=1;
	while(a[i]!='\0'&&a[i]!='.')
	{
		p[top]=a[i];
		top++;
		i++;
	}
	while(top!=0)
	{
		top--;
		sum=sum+(p[top]-48)*mul;
		mul=mul*10;
	}
	if(a[i]=='.')
	{
		i++;
		mul=0.1;
		while(a[i]!='\0')
		{
			sum=sum+(a[i]-48)*mul;
			mul=mul/10;
			i++;
		}
	}
	return sum;
}
//四则运算
double compute(double a,double b,char sign)
{
	if(sign=='+')
		return a+b;
	if(sign=='-')
		return b-a;
	if(sign=='*')
		return a*b;
	if(sign=='/')
		return b/a;
}
//符号优先级
int signnum(char x)
{
	if(x=='(')
		return 1;
	if(x=='+'||x=='-')
		return 2;
	if(x=='*'||x=='/')
		return 3;
}
double com(char exp[1000])
{
	int strRead=0,strWrite=0;
	int strStart;
	int length=0;
	double strCuttenFloat;
	char strCutten[1000];
	int i=0;//表达式遍历索引
	int j=0;//后缀表达式索引
	int signtop=-1;//符号栈顶指针
	int signtop1=-1;//数据栈顶指针
	int s=0;
	int k=0;//后缀表达式遍历
	double a,b;//四则运算参数
	printf("输入表达式:\n");
	expre1=(struct expre*)malloc(strlen(exp)*sizeof(struct expre));
	while(exp[strRead]!='\0')
	{
		//如果是数字，存进结构体中的num
		if(exp[strRead]>='0'&&exp[strRead]<='9'||exp[strRead]=='.')
		{
			strStart=strRead;
			while(exp[strRead]>='0'&&exp[strRead]<='9'||exp[strRead]=='.')
			{
				strRead++;
				length++;
			}
			//截取数字字符串，并转换成数字
			strncpy(strCutten,exp+strStart,length);
			strCutten[length]='\0';
			strCuttenFloat=stringToDouble(strCutten);
			expre1[strWrite].num=strCuttenFloat;
			expre1[strWrite].ifNum=1;
			expre1[strWrite].end=0;
			expre1[strWrite].sign='o';
			strWrite++;
			length=0;
		}
		//如果是符号，存进结构体中的sign
		else
		{
			expre1[strWrite].sign=exp[strRead];
			expre1[strWrite].ifNum=0;
			expre1[strWrite].end=0;
			expre1[strWrite].num=0;
			strRead++;
			strWrite++;
		}
	}
	expre1[strWrite].end=1;
	expre1[strWrite].ifNum=1;
	expre1[strWrite].sign='o';
	expre1[strWrite].num=0;
	/*下面将表达式转换成后缀表达式*/
	//符号栈初始化
	signstack1.base=(char*)malloc(strlen(exp)*sizeof(char));
	//后缀表达式初始化
	expre2=(struct expre*)malloc(strlen(exp)*sizeof(struct expre));
	while(expre1[i].end!=1)
	{
		if(expre1[i].ifNum==1)
		{
			expre2[j].num=expre1[i].num;
			expre2[j].ifNum=1;
			j++;
			i++;
		}
		else if(expre1[i].sign=='(')
		{
			signtop++;
			signstack1.base[signtop]=expre1[i].sign;
			i++;
		}
		else if(signnum(expre1[i].sign)==2)
		{
			if(signtop==-1)
			{
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
			else
			{
				while(signnum(signstack1.base[signtop])==2||signnum(signstack1.base[signtop])==3)
				{
					expre2[j].sign=signstack1.base[signtop];
					expre2[j].ifNum=0;
					j++;
					signtop--;
					if(signtop==-1)
						break;
				}
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
		}
		else if(expre1[i].sign=='*'||expre1[i].sign=='/')
		{
			if(signtop==-1)
			{
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
			else
			{
				while(signnum(signstack1.base[signtop]==3))
				{
					expre2[j].sign=signstack1.base[signtop];
					expre2[j].ifNum=0;
					j++;
					signtop--;
					if(signtop==-1)
						break;
				}
				signtop++;
				signstack1.base[signtop]=expre1[i].sign;
				i++;
			}
		}
		else
		{
			if(expre1[i].sign==')')
			{
				while(signstack1.base[signtop]!='(')
				{
					expre2[j].sign=signstack1.base[signtop];
					expre2[j].ifNum=0;
					j++;
					signtop--;
				}
				i++;
				signtop--;
			}
			else
			{
				printf("输入表达式格式不规范!\n");
				Sleep(2000);
				exit(1);
			}
		}
	}
	while(signtop!=-1)
	{
		expre2[j].sign=signstack1.base[signtop];
		expre2[j].ifNum=0;
		j++;
		signtop--;
	}
	expre2[j].end=1;
	putchar('\n');
	numstack1.base=(double*)malloc(100*sizeof(double));
	if(numstack1.base==NULL)
	{
		printf("\n动态内存分配失败\n");
		Sleep(2000);
		exit(1);
	}
	while(expre2[k].end!=1)
	{
		if(expre2[k].ifNum==1)/*数字直接入栈*/
		{
			signtop1++;
			numstack1.base[signtop1]=expre2[k].num;
			k++;
		}
		else/*如果是符号*/
		{	
			
			a=numstack1.base[signtop1];
			signtop1--;
			b=numstack1.base[signtop1];		
			numstack1.base[signtop1]=compute(a,b,expre2[k].sign);
			k++;
		}
	}
	return numstack1.base[signtop1];
}