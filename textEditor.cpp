#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#define filename "textEditor.txt"
#define MAXSTRLEN 255

typedef char string[MAXSTRLEN+1];
typedef struct Node{
    string line;
    struct Node *next;
    struct Node *prev; 
 }Line;
 
 int nums(Line *text);//计算文本总行数 
 Line* addLine(Line *text,string newLine);//文末添加新行 
 Line* insertLine(Line *text,int n,string newLine);//在第n行前插入新行 
 Line* moveLine(Line *text,Line *preLine);//移除当前行，光标移动到下一行 
 void dropLine(Line *text,int n);//删除第n行 
 Line* insertStr(char *preFlag,Line *preLine,string newLine);//当前光标位置后插入字符串 
 Line* deleteChar(char *preFlag,Line *preLine);//删除光标位置所在字符 
 void moveChar(Line *text,int n,int m,char *preFlag,Line *preLine);//移动光标到n行m列 
 void printText(Line *text);//输出整个文本 

int nums(Line *text)
{
	int cnt=0;
	Line *p=text->next;
	while(p->prev!=text->prev)
	{
		cnt++;
		p=p->next;
	}
	return cnt;
}
Line *createText(void)
{
	Line *headNode=(Line*)malloc(sizeof(Line));
	headNode->next=headNode;
	headNode->prev=headNode;
	return headNode;
 }
Line *createNode(string newLine)
{
	Line *newNode=(Line*)malloc(sizeof(Line));
	strcpy(newNode->line,newLine);//字符串赋值 
	newNode->next=NULL;
	newNode->prev=NULL;
	return newNode; 
}
//表尾法插入节点 
Line *insertNode(Line *headNode,string newLine)
{
	Line *newNode=createNode(newLine);
	Line *tailNode=headNode->prev;
	tailNode->next=newNode;
	newNode->prev=tailNode;
	headNode->prev=newNode;
	newNode->next=headNode;
	return newNode; 
 }
Line* serchLine(Line *headNode,int n)
{
	int cnt=0;
	Line *p=headNode->next;
	while(p!=headNode)
	{
		cnt++;
		if(cnt==n||p->next==headNode){
			return p;
		}		
		p=p->next; 
	}
	//如果n>总行数则最终会返回尾节点 
	return p;	 
}
Line* addLine(Line *text,string newLine)
{ 
	return insertNode(text,newLine);
}
Line* insertLine(Line *text,int n,string newLine)
{
	Line *newNode=(Line*)malloc(sizeof(Line));
	strcpy(newNode->line,newLine);
	Line *nextLine=serchLine(text,n);
	Line *front=nextLine->prev;
	front->next=newNode;
	newNode->next=nextLine;
	nextLine->prev=newNode;
	newNode->prev=front; 
	return newNode;
}
Line* moveLine(Line *text,Line *preLine)
{
	Line *p=preLine;
	if(preLine->next==text)
		preLine=preLine->prev;
	else preLine=preLine->next;//移动到下一行 
	p->next->prev=p->prev;//后指针指向前指针
	p->prev->next=p->next;//前指针指向后指针 
	free(p);//释放节点内存
	return preLine;
}
void dropLine(Line *text,int n)
{
	if(n>nums(text)){
		printf("[ECHO]:Invalid Command\n");
		return;
	}
	Line *p=serchLine(text,n);//查找第n行 
	(p->next)->prev=p->prev;//后指针指向前指针
	(p->prev)->next=p->next;//前指针指向后指针
	free(p);
}

Line* insertStr(char *preFlag,Line *preLine,string newLine)
{
	if(newLine[0]+preLine->line[0]>MAXSTRLEN-1){
		printf("[ECHO]:Out of Range\n");
		return preLine;
	}
	preLine->line[0]=preLine->line[0]+newLine[0];
	char temp[MAXSTRLEN+1];
	preFlag=preFlag+1;
	strcpy(temp,preFlag);//把光标后面的内容
	memset(preFlag,0,sizeof(preFlag));
	strcat(preLine->line,&newLine[1]);//连接字符串
	strcat(preLine->line,temp);
	preFlag=&(preLine->line[strlen(preLine->line)-1]);
	return preLine;
}
Line* deleteChar(char *preFlag,Line *preLine)
{
	//利用字符指针和截断符分割字符串加之字符串操作函数 
	*preFlag='\0';
	preFlag=preFlag+1;
	strcat(preLine->line,preFlag);
	preLine->line[0]-1;
	return preLine;
}
Line* moveChar(Line *text,int n,Line *preLine)
{
	if(n>nums(text))
		n=nums(text);
	preLine=serchLine(text,n);
	return preLine;
}
void printText(Line *text)
{
	Line *p=text->next;
	while(p!=text)
	{
		printf("%s\n",&(p->line[1]));
		p=p->next;
	}
}
void input(string newLine)
{
	int i=1;
	while((newLine[i++]=getchar())!='\n')
		if(i>MAXSTRLEN) break; 
	newLine[i-1]='\0';//末尾添加截断符 
	newLine[0]=i-2;//首地址储存字符串实际长度 
}
int isEmpty(Line *text)//判断文本是否为空 
{
	if(text->next==text)
		return 1;
	return 0;
 } 
void menu(Line *text)//主操作函数
{
	int c=1;int op;
	printf("Text Editor has started  ");
	printf("\nInstructions are as follows:\n");
	printf("options:1.$AL 2.$IL 3.$DD 4.$DL 5.$IS 6.$DC 7.$MC 8.$PL 9.$PT 0.END\nYou only need to enter the serial number to specify the function\n\nExamples and comments:\n");
	printf("0                 Ends the text editing and saves the text content\n");
	printf("1 textstring      Add a line of text at the end and Move the cursor to the beginning of the added line\n");
	printf("2 n textstring    Insert a line of text before line n and Move the cursor to the beginning of the inserted line\n");
	printf("3                 Delete current line and Move the cursor to the beginning of the next line\n"); 
	printf("4 n               Delete line n but it doesn't work for n greater than total number of rows\n");
	printf("5 textstring      Insert a string at the current cursor position and Move the cursor to the end of string\n");
	printf("6                 Delete the character at the current cursor position\n");
	printf("7 n m             Move the cursor to n rows and m columns\n");
	printf("8                 Displays the current paragraph content, cursor position, and string length\n");
	printf("9                 Print the entire text\n\nNow enter the command below by English:\n\n");
	string newLine;
	Line *preLine=text->next;
	char *preFlag=&preLine->line[1];//光标为字符指针，指向字符 
	do{ 
		printf("[INFO]:");
		scanf("%d",&op);
		if(op==1)
		{
			getchar();
			input(newLine);
			preLine=addLine(text,newLine);
			preFlag=&(preLine->line)[1];
		}
		else if(op==2)
		{
			int n;
			scanf("%d",&n);
			getchar();
			input(newLine);
			preLine=insertLine(text,n,newLine);
			preFlag=&(preLine->line)[1];
		}
		else if(op==3)
		{
			preLine=moveLine(text,preLine);
			preFlag=&(preLine->line)[1];
		}
		else if(op==4)
		{
			int n;
			scanf("%d",&n);
			dropLine(text,n);
		}
		else if(op==5)
		{
			getchar();
			input(newLine);
			preLine=insertStr(preFlag,preLine,newLine);
			preFlag=&preLine->line[strlen(preLine->line)-1];
		}
		else if(op==6)
		{
			if(0==*(preFlag+1)){//光标在行末的情况，光标后无字符(ASCII=0) 
				if(preFlag==&(preLine->line)[1])
					*preFlag=0;
				else preFlag=preFlag-1;//前移 
			}
			if(0==preLine->line[0]){//本行无字符
				preLine=moveLine(text,preLine);//删除整行 
			} 
			preLine=deleteChar(preFlag,preLine);
		}
		else if(op==7)
		{
			int n,m;
			scanf("%d %d",&n,&m);//获取坐标，移动光标 
			preLine=moveChar(text,abs(n),preLine);
			if(m>preLine->line[0])
				m=preLine->line[0];
			preFlag=&preLine->line[m];  
		}
		else if(op==8)
		{
			printf("[ECHO]:%s  Cursor position:%d  Paragraph length:%d\n",&preLine->line[1],preFlag-preLine->line,preLine->line[0]);
		}
		else if(op==9)
		{
			printf("\n[ECHO]:Output text content\n");
			printf("------------------------------------------------------------\n"); 
			printText(text);
			printf("------------------------------------------------------------\n"); 
		}
		else if(op==0)
		{
			printf("[ECHO]:End of the program\n");
			c=0;
		}
		else
			printf("[WARN]:Misoperation\n");//错误操作
	}while(c);
 }
void readInfo(Line *text)
{
	FILE *fp=NULL;string newLine; char ch;
	fp=fopen(filename,"r");
	if(NULL==fp){
		fp=fopen(filename,"w+");
		return;//文件不存在，创建后返回 
	}
	int i=0;
	while((ch=fgetc(fp))!=EOF)
	{
		newLine[++i]=ch;
		if(ch=='\n'){
			newLine[i]=0;
			newLine[0]=i-2;
			addLine(text,newLine);
			i=0;
		}
	}
	fclose(fp);
}
void writeInfo(Line *text)
{
	FILE *fp=NULL;
	Line *p=text->next;
	fp=fopen(filename,"w");
	while(p!=text){
		fprintf(fp,"%s\n",&p->line[1]);
		p=p->next;
	}
	fclose(fp);
}
int main(void)
{
	Line *text=createText();
	readInfo(text); 
	menu(text);
	writeInfo(text);
	system("pause");
	return 0;		
}
