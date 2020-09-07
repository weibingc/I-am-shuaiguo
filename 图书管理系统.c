#include<stdio.h>
#include<string.h> 
#include<stdlib.h>
#include<windows.h>

int bookKinds=0;//不同种类书籍的数量； 

typedef struct Node
{
	char libraryCard[12];//借书证号 
	int year;//年份 
	int month;//月份 
	int date;//日期 
	struct Node *Next;
	struct Node *Last;
}borrowInfo;//借阅信息； 

typedef struct node
{
	char bookNumber[16];//书号 
	char title[30];//书名 
	char author[30];//作者 
	int inventory;//库存数量； 
	int standingCrop;//可借数量； 
	borrowInfo *borrow;//借阅信息； 
	struct node *last;
	struct node *next;
}wareHouse;//书籍信息 

void functionList(void);//图书管理系统使用指南列表；  
void storage(wareHouse *library);//新书入库管理； 
void borrowBooks(wareHouse *library);//书籍借阅管理； 
void returnBooks(wareHouse *library);//书籍归还管理； 
void infoBook(wareHouse *library);// 书籍库存信息查询；  
void writeToText(wareHouse *library); //外部.txt文件存储信息； 
int stringComparison(char *a,char *b);//字符串对比；
wareHouse *search(wareHouse *library);//书籍检索；
wareHouse *addFromText(void);//从外部.txt文件读取存储信息 ；

int main(void)
{
	wareHouse *library;//图书馆 
	library=addFromText();//读取文件信息； 
	printf("__________________________________________________________________________\n\n");
	printf("\n欢迎使用图书管理系统，请按照提示操作。\n\n");
	printf("__________________________________________________________________________\n\n");
	Sleep(1500);//一个启动界面，可设置取消； 
	system("cls");
	functionList();//操作指南； 
	printf("请确定你想要使用的功能：");
	char functions;
	while(~scanf("%c",&functions))//功能选择； 
	{
		if(functions=='#')
		{
			system("cls");
			writeToText(library);//保存信息到.txt文件中； 
        	printf("\n\n         系统成功退出，感谢您的使用^_^\n");
        	break;
        }
		else
		{
			switch(functions)
			{
				case '1':storage(library);//新书入库管理； 
					break;
				case '2':borrowBooks(library);//书籍借阅管理； 
					break;
				case '3':returnBooks(library);//书籍归还管理； 
					break;
				case '4':infoBook(library);//书籍库存信息查询；  
					break;
				default:printf("警告！！！误操作！！！\n");//误操作提醒； 
			}
			if(functions=='1'||functions=='2'||functions=='3'||functions=='4'||functions=='#')	system("pause");
			system("cls");
			functionList(); 
			printf("请输入你想要使用的功能：");
		} 
	}
	return 0;
} 


void functionList(void)
{	
	printf("__________________________________________________________________________\n\n");
	printf("图书管理系统使用指南：\n");
	printf("书籍入库：\t\t\t请按“1”\n");
	printf("书籍借阅：\t\t\t请按“2”\n");
	printf("书籍归还：\t\t\t请按“3”\n");
	printf("书籍查找：\t\t\t请按“4”\n");
	printf("退出系统：\t\t\t请按“#”\n"); 
	printf("__________________________________________________________________________\n");
}

int stringComparison(char *a,char *b)
{
	int i,flag=1;
	for(i=0;i<strlen(a); i++)
		if(a[i]!=b[i])
		{
			flag=0;
			break;
		}
	return flag;
}

void storage(wareHouse *library)
{
	getchar();
	system("cls");
	printf("__________________________________________________________________________\n\n");
	printf("图书馆书籍入库管理：\n");
	printf("__________________________________________________________________________\n\n");
	int flag=0;
	wareHouse *p,*news;
	p=library;
	news=(wareHouse *)malloc(sizeof(wareHouse));
	printf("请输入新书的书号(15位以内，非中文)：");
	scanf("%s",news->bookNumber);
	printf("请输入新书的书名：");
	scanf("%s",news->title);
	printf("请输入新书的作者：");
	scanf("%s",news->author);
	printf("请输入新书的数量：");
	scanf("%d",&news->inventory);
	news->standingCrop = news->inventory;
	news->borrow=NULL;
	news->next=NULL;
	while(p->next!=NULL) 
	{
		p=p->next;
		if(stringComparison(news->bookNumber,p->bookNumber)==1)
		{
			flag=1;
			break;
		}
	}
	if(flag==1)//新书已存在，则修改书籍的库存量和现存量； 
	{
		p->inventory +=news->inventory;
		p->standingCrop += news->standingCrop;
	}
	else//新书未存在，则入库，添加新书的库存数据； 
	{
		p->next = news;
		news->last =p;
		bookKinds++;
	}
	printf("\n书籍入库操作成功\n");
} 

wareHouse *search(wareHouse *library)//书籍检索，可按照书号或书名来查找书籍是否存在； 
{
	wareHouse *p,*q;
	p=library;
	q=(wareHouse *)malloc(sizeof(wareHouse));
	q=NULL;
	int searchBy; 
	printf("设置查找方式：\n"); 
	printf("按书号查找请按“1”\n"); 
	printf("按书名查找请按“2”\n"); 
	printf("您的选择是："); 
	scanf("%d",&searchBy);
	getchar();
	if(searchBy==1||searchBy==2)
	{
		if(searchBy==1)
		{
			char chose[16];
			printf("请输入要查找的书号：");
			scanf("%s",chose);
			while(p->next!=NULL)
			{
				p=p->next;
				if(stringComparison(chose,p->bookNumber)==1)
				{
					q=p;
					break;
				}
			}
		}
		else
		{
			char chose[30];
			printf("请输入要查找的书名：");
			scanf("%s",chose);
			while(p->next!=NULL)
			{
				p=p->next;
				if(stringComparison(chose,p->title)==1)
				{
					q=p;
					break;
				}
			}
		}
		if(q==NULL) printf("该书不存在。\n");
	}
	else printf("警告！！！误操作！！！\n");
	return q;
}

void borrowBooks(wareHouse *library)
/*书籍借阅时，需要填写借阅信息，包括借阅证号、年、月、日，并保存到书籍信息中，同时修改被借书籍的现存量；*/ 
{
	getchar();
	system("cls");
	printf("__________________________________________________________________________\n\n");
	printf("图书馆书籍借阅管理：\n");
	printf("__________________________________________________________________________\n\n");
	wareHouse *p,*q;
	p=library;
	q=search(p); 
	if(q!=NULL) 
	{
		if(q->standingCrop==0)  printf("此书籍可借数量为0，无法被借阅。\n");
		else
		{
			q->standingCrop--;
			getchar();
			borrowInfo *news,*s;
			news=(borrowInfo *)malloc(sizeof(borrowInfo));
			printf("请输入您的借书证号(10位):"); 
			scanf("%s",news->libraryCard); 
			printf("请填写您的归还日期：\n");
			printf("年：");
			scanf("%d",&news->year); 
			printf("月：");
			scanf("%d",&news->month);
			printf("日：");
			scanf("%d",&news->date);
			news->Next=NULL;
			news->Last=NULL;
			if(q->borrow==NULL)	q->borrow=news;
			else
			{
				s=q->borrow;
				while(s->Next!=NULL) s=s->Next;
				s->Next = news;
				news->Last = s;	
			}
			printf("\n书籍借阅操作成功。\n"); 
		}
	} 
}

void infoBook(wareHouse *library)
/*可查看书籍的库存信息，若此书被借阅过，则同时列出借阅信息*/ 
{
	getchar();
	system("cls");
	printf("__________________________________________________________________________\n\n");
	printf("图书馆书籍信息检索管理：\n");
	printf("__________________________________________________________________________\n\n");
	wareHouse *p,*q;
	p=library;
	q=search(p); 
	if(q!=NULL) 
	{
		system("cls");
		printf("这本书籍的信息为：\n\n\n"); 
		printf("书号：%s\n",q->bookNumber);
		printf("书名：%s\n",q->title);
		printf("作者：%s\n",q->author);
		printf("库存数量：%d\n",q->inventory);
		printf("可借数量：%d\n",q->standingCrop);
		if(q->borrow!=NULL)
		{
			printf("\n");
			borrowInfo *s;
			s=q->borrow;
			int i=1;
			while(s!=NULL)
			{
				printf("第%d个借书证：\n",i);
				printf("书证号码：%s\n",s->libraryCard);
				printf("归还日期：%d年%d月%d日\n\n",s->year,s->month,s->date); 
				s=s->Next;
				i++;
			}
		}
	}
} 

void returnBooks(wareHouse *library)
/*书籍归还后，清楚借阅人在该书本的借阅信息，同时书籍的现存量+1*/ 
{
	getchar();
	system("cls");
	printf("__________________________________________________________________________\n\n");
	printf("图书馆书籍归还管理：\n");
	printf("__________________________________________________________________________\n\n");
	wareHouse *p,*q;
	p=library;
	q=search(p); 
	if(q!=NULL)
	{
		getchar();
		borrowInfo *s,*r;
		char returns[12];
		s=q->borrow;
		r=NULL;
		printf("请输入您的借书证号(10位):"); 
		scanf("%s",returns); 
		while(s!=NULL)
		{
			if(stringComparison(returns,s->libraryCard)==1)
			{
				r=s;
				q->standingCrop++;
				break;
			}
			s=s->Next;
		}
		if(r!=NULL)
		{ 	
			if(r->Last==NULL&&r->Next==NULL) q->borrow=NULL;
			if(r->Last==NULL&&r->Next!=NULL) 
			{
				q->borrow=r->Next;
				q->borrow->Last=NULL;
			}
			if(r->Next==NULL&&r->Last!=NULL)  r->Last->Next=NULL;
			if(r->Last!=NULL&&r->Next!=NULL) 
			{
				r->Last->Next =r->Next;
				r->Next->Last =r->Last;
			}
			free(r);
			printf("\n书籍归还操作成功。\n"); 
		}
		else printf("没有此借阅信息。\n");
	} 
} 

wareHouse *addFromText(void)
/*从.txt文件中读取信息；*/
{
	FILE *fp;
	fp=fopen("D:\\bookManagement.txt","rb");/*若文件存在，则打开*/ 
	if(fp==NULL)	bookKinds=0;
	int i=0,j=0;
	wareHouse *p,*library;
	p=library=(wareHouse *)malloc(sizeof(wareHouse));
	library->next=NULL;
	library->last=NULL;
	fscanf(fp,"%d",&bookKinds); 
	i=bookKinds;
	while(i--)
	{
		wareHouse *q;
		q=(wareHouse *)malloc(sizeof(wareHouse));
		q->last=NULL;
		q->next=NULL;
		fscanf(fp,"%s %s %s %d %d",q->bookNumber,q->title,q->author,&q->inventory,&q->standingCrop);
		j=q->inventory - q->standingCrop;
		q->borrow=NULL;
		borrowInfo *r=NULL;
		while(j--)
		{
			borrowInfo *s;
			s=(borrowInfo *)malloc(sizeof(borrowInfo));
			s->Next=NULL;
			s->Last=NULL;
			fscanf(fp,"%s %d %d %d",s->libraryCard,&s->year,&s->month,&s->date);
			if(r==NULL) r=s;
			else
			{
				r->Next=s;
				s->Last=r;
				r=r->Next;
			}
		}
		if(r!=NULL)
		{
			while(r->Last!=NULL) r=r->Last;
			q->borrow=r;
		}
		p->next=q;
		q->last=p;
		p=p->next;
		
	}
	fclose(fp);/*关闭文件*/ 
	return library;
}

void writeToText(wareHouse *library)
{
	FILE *fp;
	fp=fopen("D:\\bookManagement.txt","wb");/*若文件存在，则打开；否则在规定路径新建一个同名的文件；*/ 
	int i=0,j=0;
	wareHouse *p;
	p=library;
	i=bookKinds;
	fprintf(fp,"%d\n",bookKinds);
	while(i--)
	{
		p=p->next;
		fprintf(fp,"%s %s %s %d %d\n",p->bookNumber,p->title,p->author,p->inventory,p->standingCrop);
		borrowInfo *s;
		s=p->borrow;		
		while(s!=NULL)
		{
			fprintf(fp,"%s %d %d %d\n",s->libraryCard,s->year,s->month,s->date);
			s=s->Next;
		}
	}
	printf("数据已保存进文件中。\n"); 
	fclose(fp);/*关闭文件*/ 
}
