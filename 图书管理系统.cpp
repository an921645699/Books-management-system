#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <windows.h>
//console
#include<conio.h>

#define BOOK_NUM  200
#define READER_NUM  100


#define BOOK_DATA  astBook[i].iNum,astBook[i].acName,astBook[i].acAuthor,astBook[i].acPress,astBook[i].iAmount
#define LEN_BOOK  sizeof(struct Book)

#define LEN_READER sizeof(struct Reader)
#define READER_DATA astReader[i].iNum,astReader[i].acName,astReader[i].acSex,astReader[i].iMax,astReader[i].iAmount
//两个结构体封装图书和读者的基本信息
struct Book
{
	int iNum;//图书编号
	char  acName[15];//图书名称;
	char  acAuthor[15];//图书作者
	char   acPress[15];//图书出版社;
	int  iAmount;//图书库存量;
};


struct Reader
{
	int iNum;//读者编号;
	char  acName[15];//读者姓名
	char  acSex[4];//读者性别;
	int iMax;//读者最大可借阅数量
	int iAmount;//读者当前可借阅的数量;
	int  BookId[10];//读者已经借阅的图书列表
};

//定义两个全局结构体数组来记录所有图书信息和所有读者信息
struct Book astBook[BOOK_NUM];
struct Reader  astReader[READER_NUM];

//显示主菜单
void  ShowMainMeun()
{
	//清屏函数
	system("cls");
	printf("\n\n\n\n\n");
	printf("\t***********************************欢迎进入****************************\n");
	printf("\t*******************************图书馆管理系统*************************\n");
	printf("\t*********************************主菜单********************************\n");
	printf("\t*********************************1.图书管理****************************\n");
	printf("\t*********************************2.读者管理****************************\n");
	printf("\t*********************************3.借还书登记**************************\n");
	printf("\t*********************************0.退出系统**************************\n");
	printf("\n");
	printf("\t\t请选择(0-3)");
}

//显示书籍管理的子菜单
void  ShowBookMenu()
{
	system("cls");
	printf("\n\n\n\n");
	printf("\t|******************************欢迎进入***************************|\n");
	printf("\t|******************************图书馆管理系统********************|\n");
	printf("\t|*********************************子菜单**************************|\n");
	printf("\t|******************************1.显示图书信息*********************|\n");
	printf("\t|******************************2.新增图书信息*********************|\n");
	printf("\t|******************************3.查询图书信息*********************|\n");
	printf("\t|******************************4.删除图书信息*********************|\n");
	printf("\t|******************************5.修改图书信息*********************|\n");
	printf("\t|******************************0.返回主菜单***********************|\n");
	printf("\n");
	printf("\t\t请选择(0-5):");
}

int  ReadBookFile(const char* pcMode)
{
	int iBookRecord = 0;
	FILE* pfBook;//文件指针;
	pfBook = fopen("book.txt", pcMode);
	if (pfBook == NULL)
	{
		return -1;
	}
	while (!feof(pfBook))
	{
		if (fread(&astBook[iBookRecord], LEN_BOOK, 1, pfBook))
		{
			iBookRecord++;
		}
	}
	fclose(pfBook);
	return iBookRecord;
}

void ShowBook()
{
	int i, iBookRecord;//记录条数
	system("cls");
	iBookRecord = ReadBookFile("rb");//只读
	if (iBookRecord == -1)
	{
		printf("该文件打开失败,请先新增图书信息!\n");
	}
	if (iBookRecord == 0)
	{
		printf("文件中没有图书信息!\n");
	}
	else
	{
		printf("\t|**********************************************************************|\n");
		printf("\n\n");
		printf("\t%-6s%-16s%-10s%-20s%-4s\n", "编号", "书名", "作者", "出版社", "库存");
		for (i = 0; i < iBookRecord; i++)
		{
			printf("\t%-6d%-16s%-10s%-20s%-4d\n", BOOK_DATA);
		}
		printf("\n\n");
		printf("\t|**********************************************************************|\n");
		printf("\n\n");
	}

}

void  AddBook()
{
	FILE* pfBook;
	int iBookRecord, iFlagExist, i;
	char cFlag;
	system("cls");
	iBookRecord = ReadBookFile("ab+");//ab+追加方式打开或者新建二进制文件
	if (iBookRecord == -1)
	{
		printf("文件打开失败!\n");
		return;
	}
	else if (iBookRecord == 0)
	{
		printf("没有图书记录!\n");
	}
	else
	{
		ShowBook();//如果图书记录不为0则显示所有图书
	}
	//循环录入图书信息
	printf("请选择是否输入信息(y/n):");
	cFlag = getchar();
	getchar();
	if (cFlag == 'n')
	{
		return;
	}
	pfBook = fopen("book.txt", "ab+");
	if (pfBook == NULL)
	{
		printf("文件打开失败!\n");
		return;
	}
	while (cFlag == 'y')
	{
		//录入数据
		if (iBookRecord >= BOOK_NUM)
		{
			printf("记录已满!");
			fclose(pfBook);
			return;
		}

		//图书编号不能重复
		printf("请输入图书编号:  ");
		do
		{
			iFlagExist = 0;
			scanf("%d", &astBook[iBookRecord].iNum);
			getchar();
			for (i = 0; i < iBookRecord; i++)
			{
				if (astBook[i].iNum == astBook[iBookRecord].iNum)
				{
					iFlagExist = 1;
					printf("该图书编号已经存在,请重新输入:   ");
					break;
				}
			}
		} while (iFlagExist == 1);

		printf("请输入图书名称:");
		scanf("%s", astBook[iBookRecord].acName);
		getchar();

		printf("请输入图书作者:");
		scanf("%s", astBook[iBookRecord].acAuthor);
		getchar();

		printf("请输入图书出版社:");
		scanf("%s", astBook[iBookRecord].acPress);
		getchar();

		printf("请输入图书库存量:");
		scanf("%d", &astBook[iBookRecord].iAmount);
		getchar();

		//将新增的图书信息写入到文件中
		if (fwrite(&astBook[iBookRecord], LEN_BOOK, 1, pfBook) != 1)
		{
			printf("无法保存该信息!\n");
			return;
		}
		else
		{
			printf("%d号图书信息已经保存!\n", astBook[iBookRecord].iNum);
			iBookRecord++;
		}
		printf("继续输入信息吗?(y/n)");
		cFlag = getchar();
		getchar();
	}
	fclose(pfBook);
	printf("添加图书信息执行完毕!\n");
}

//查询
int SearchBook()
{
	int iBookNum, iBookRecord, iBookId, i;
	//注释
	system("cls");
	iBookRecord = ReadBookFile("rb");
	if (iBookRecord == -1)
	{
		printf("文件打开失败!\n");
		printf("|按任意键返回子菜单|");
		_getch();
		return -2;//文件打开失败,返回-2
	}
	else  if (iBookRecord == 0)
	{
		printf("没有图书记录!\n");
		printf("|按任意键返回子菜单|");
		_getch();
		return -3;//没有记录,返回-3
	}

	//进入查找程序
	printf("请输入图书编号:");
	scanf("%d", &iBookNum);
	getchar();

	for (i = 0; i < iBookRecord; i++)
	{
		if (iBookNum == astBook[i].iNum)
		{
			iBookId = i;//找到记录,返回记录编号
			printf("%d号图书信息如下:\n", iBookNum);
			printf("\t-----------------------------------------------------------\n");
			printf("\t  %-6s%-16s%-10s%-20s%-4s\n", "图书编号", "书名", "作者", "出版社", "库存量");
			printf("\t  %-6d%-16s%-10s%-20s%-4d\n", BOOK_DATA);
			printf("\t-----------------------------------------------------------\n");
			break;
		}
	}

	if (i == iBookRecord)
	{
		printf("找不到%d号图书信息!\n,", iBookNum);
		iBookId = -1;//找不到记录,返回-1;
	}

	return iBookId;
}


void  DeleteBook()
{
	FILE* pfBook;
	int iBookId, iBookRecord, i;
	char cFlag;

	system("cls");
	iBookId = SearchBook();
	if (iBookId == -1)
	{
		return;
	}

	iBookRecord = ReadBookFile("rb");//???
	printf("已经找到该图书,是否删除?(y/n)");
	cFlag = getchar();
	getchar();

	//删除之前需要保存删除之前的图书编号,因为删除之后图书编号信息会被覆盖
	int  index = astBook[iBookId].iNum;
	if (cFlag == 'n')
	{
		return;
	}
	else if (cFlag == 'y')
	{
		for (i = iBookId; i < iBookRecord - 1; i++)
		{
			astBook[i] = astBook[i + 1];//数组依次前移
		}
		iBookRecord--;
	}

	pfBook = fopen("book.txt", "wb");
	if (pfBook != NULL)
	{
		for (i = 0; i < iBookRecord; i++)
		{
			if (fwrite(&astBook[i], LEN_BOOK, 1, pfBook) != 1)
			{
				printf("无法保存该信息!\n");
				return;
			}
		}
		fclose(pfBook);
		printf("%d号图书信息已经删除!\n", index);
	}
}

//显示修改选项的子菜单
void ShowModifyBookMenu()
{
	printf("\n\n\n");
	printf("\t                             1.编号                                               \n");
	printf("\t                             2.书名                                               \n");
	printf("\t                             3.作者                                               \n");
	printf("\t                             4.出版社                                              \n");
	printf("\t                             5.图书库存量                                       \n");
	printf("\t       请输入所要修改的信息(输入相应的数字1-5):");
}


void SaveBookFile(int iBookId)
{
	FILE* pfBook;
	pfBook = fopen("book.txt", "rb+");
	if (pfBook != NULL)
	{
		fseek(pfBook, LEN_BOOK * iBookId, SEEK_SET);
		if (fwrite(&astBook[iBookId], LEN_BOOK, 1, pfBook) != 1)
		{
			printf("无法保存该信息!\n");
		}
	}
	fclose(pfBook);
}

void  ModifyBook()//修改图书信息;
{
	int iBookId, iBookRecord, iFlagExist, iItem, iNum, i;

	system("cls");
	iBookId = SearchBook();
	if (iBookId == -1)
	{
		return;
	}
	if (iBookId == -3)
	{
		return;
	}
	//找到该序号的图书,可以进行修改操作;
	iBookRecord = ReadBookFile("rb");
	//显示修改选项的子菜单
	ShowModifyBookMenu();
	scanf("%d", &iItem);
	getchar();

	switch (iItem)
	{
	case 1:
		printf("请输入图书编号:");
		do
		{
			iFlagExist = 0;
			scanf("%d", &iNum);
			getchar();
			for (i = 0; i < iBookRecord; i++)
			{
				if (iNum == astBook[i].iNum && i != iBookId)
				{
					iFlagExist = 1;
					printf("错误,该图书编号已经存在,请重新输入:");
					break;
				}
			}
		} while (iFlagExist == 1);
		astBook[iBookId].iNum = iNum;
		break;
	case 2:
		printf("请输入图书名称:");
		scanf("%s", astBook[iBookId].acName);
		getchar();
		break;
	case 3:
		printf("请输入图书作者:");
		scanf("%s", astBook[iBookId].acAuthor);
		getchar();
		break;
	case 4:
		printf("请输入图书出版社:");
		scanf("%s", astBook[iBookId].acPress);
		getchar();
		break;
	case 5:
		printf("请输入图书库存量:");
		scanf("%d", &astBook[iBookId].iAmount);
		getchar();
		break;
	}
	SaveBookFile(iBookId);
	printf("图书信息修改成功!\n");
}

void MangaeBook()//书籍管理
{
	int  iItem;
	ShowBookMenu();//显示书籍管理的子菜单
	scanf("%d", &iItem);
	getchar();
	while (iItem)
	{
		switch (iItem)
		{
		case 1:
			ShowBook();//显示图书信息
			break;
		case 2:
			AddBook();//新增图书信息
			break;
		case 3:
			SearchBook();//查找图书信息;
			break;
		case 4:
			DeleteBook();//删除图书信息;
			break;
		case 5:
			ModifyBook();//修改图书信息;
			break;
		default:
			printf("\t\t请输入正确的数字!\n\t\t");
		}
		printf("|请按任意键返回子菜单|");
		_getch();//从控件中获取字符而无需回显;
		ShowBookMenu();
		scanf("%d", &iItem);
		getchar();
	}

}

void ShowReaderMenu()//显示读者管理的子菜单
{
	system("cls");//清屏函数
	printf("\n\n\n\n\n");//目的是界面往中间一点
	printf("\t|****************欢迎进入****************|\n");
	printf("\t|*************读者管理系统***************|\n");
	printf("\t|*****************子菜单*****************|\n");
	printf("\t|**************1.显示读者信息************|\n");
	printf("\t|**************2.新增读者信息************|\n");
	printf("\t|**************3.查询读者信息************|\n");
	printf("\t|**************4.删除读者信息************|\n");
	printf("\t|**************5.修改读者信息************|\n");
	printf("\t|**************0.返回主菜单**************|\n");
	printf("\t|****************************************|\n");
	printf("\n");
	printf("\t\t请选择(0-5):");
}

int  ReadReaderFile(const char* fopenMode)
{
	int iReaderRecord = 0;
	FILE* pfReader;
	pfReader = fopen("reader.txt", fopenMode);
	if (pfReader == NULL)
	{
		return -1;
	}
	while (!feof(pfReader))
	{
		if (fread(&astReader[iReaderRecord], LEN_READER, 1, pfReader))
		{
			iReaderRecord++;
		}
	}
	fclose(pfReader);
	return iReaderRecord;
}

void ShowReader()//显示读者信息
{
	int i, iReaderRecord;
	system("cls");
	iReaderRecord = ReadReaderFile("rb");
	if (iReaderRecord == -1)
	{
		printf("该文件打开失败,请先新增加读者信息!\n");
	}
	if (iReaderRecord == 0)
	{
		printf("文件中没有读者信息!\n");
	}
	else
	{
		printf("\t|******************************************************|\n");
		printf("\n\n");
		printf("\t%-6s%-16s%-10s%-20s%-20s\n", "编号", "姓名", "性别", "最大可借书数量", "当前可借书数量");
		for (i = 0; i < iReaderRecord; i++)
		{
			printf("\t%-6d%-16s%-10s%-20d%-20d\n", READER_DATA);
			//此处加宏定义,否则代码太长,加宏可以减少代码量
		}
		printf("\n\n");
		printf("\t|******************************************************|\n");
		printf("\n\n");
	}
}

void AddReader()//新增读者信息;
{
	FILE* pfReader;//文件指针
	int iReaderRecord, iFlagExist, i;
	char cFlag;

	system("cls");
	iReaderRecord = ReadReaderFile("ab+");//ab+追加方式打开或新建二进制文件
	if (iReaderRecord == -1)
	{
		printf("文件打开失败!\n");
		return;
	}
	else if (iReaderRecord == 0)
	{
		printf("没有读者记录\n");
	}
	else
	{
		ShowReader();//如果读者记录不为0则调用ShowReader函数显示所有读者信息
	}
	//以下代码为循环录入读者信息
	printf("请选择是否输入读者信息(y/n): ");
	cFlag = getchar();
	getchar();
	if (cFlag == 'n')
	{
		return;
	}
	pfReader = fopen("reader.txt", "ab+");
	if (pfReader == NULL)
	{
		printf("文件打开失败!\n");
		return;
	}
	while (cFlag == 'y')
	{
		if (iReaderRecord >= READER_NUM)//若超出容量范围则不能继续写入
		{
			printf("记录已满!");
			fclose(pfReader);
			return;
		}
		printf("请输入读者编号: ");
		do
		{
			iFlagExist = 0;
			scanf("%d", &astReader[iReaderRecord].iNum);
			getchar();
			for (i = 0; i < iReaderRecord; i++)
			{
				if (astReader[i].iNum == astReader[iReaderRecord].iNum)
				{
					iFlagExist = 1;
					printf("该读者编号已经存在,请重新输入:  ");
					break;
				}
			}
		} while (iFlagExist == 1);


		//新增读者的基本信息
		printf("请输入读者姓名:  ");
		scanf("%s", astReader[iReaderRecord].acName);
		getchar();

		printf("请输入读者性别:  ");//F M   //细节   //a l
		scanf("%s", astReader[iReaderRecord].acSex);
		getchar();

		printf("请输入读者最大可借阅数量:  ");//10,范围
		scanf("%d", &astReader[iReaderRecord].iMax);
		getchar();

		printf("请输入读者当前可借阅数量:  ");//新增的读者 10,提示,两者必须相等
		//读者已借阅的数量=iMax-iAmount
		scanf("%d", &astReader[iReaderRecord].iAmount);
		getchar();

		//将新增的读者信息写入文件中
		if (fwrite(&astReader[iReaderRecord], LEN_READER, 1, pfReader) != 1)
		{
			printf("无法保存该信息!\n");
			return;
		}
		else
		{
			printf("%d号读者信息已经保存!\n", astReader[iReaderRecord].iNum);
			iReaderRecord++;
		}
		printf("继续输入读者信息吗?(y/n)");
		cFlag = getchar();
		getchar();
	}

	fclose(pfReader);
	printf("添加读者信息执行完毕!\n");
}


int SearchReader()//查找读者信息
{
	int iReaderNum, iReaderRecord, iReaderId, i;
	system("cls");
	iReaderRecord = ReadReaderFile("rb");//以"rb"形式打开文件,如果失败则返回
	if (iReaderRecord == -1)
	{
		printf("文件打开失败!\n");
		printf("|按任意键返回子菜单|");
		_getch();
		return -2;//文件打开失败,返回-2
	}
	else if (iReaderRecord == 0)
	{
		printf("没有读者记录!\n");
		printf("|按任意键返回子菜单|");
		_getch();
		return -3;//没有记录,返回-3
	}

	//以下进入查找程序
	printf("请输入读者编号:");
	scanf("%d", &iReaderNum);
	getchar();
	for (i = 0; i < iReaderRecord; i++)
	{
		if (iReaderNum == astReader[i].iNum)
		{
			iReaderId = i;//找到记录,返回记录号
			printf("%d号读者信息如下:\n", iReaderNum);
			printf("\t|--------------------------------------------\n");
			printf("\t  %-6s%-16s%-10s%-20s%-20s\n", "编号", "姓名", "性别", "最大可借阅数量", "当前可借阅数量");
			printf("\t  %-6d%-16s%-10s%-20d%-20d\n", READER_DATA);
			printf("\t|--------------------------------------------\n");
			break;
		}
	}
	if (i == iReaderRecord)
	{
		printf("找不到%d号读者信息!\n", iReaderNum);
		iReaderId = -1;//找不到记录,返回-1;
	}
	return iReaderId;
}

void DeleteReader()//删除读者信息
{
	FILE* pfReader;//文件指针
	int iReaderId, iReaderRecord, i;
	char cFlag;

	system("cls");
	iReaderId = SearchReader();
	if (iReaderId == -1)
	{
		return;
	}

	iReaderRecord = ReadReaderFile("rb");
	printf("已经找到该读者,是否删除?(y/n)");
	cFlag = getchar();
	getchar();
	int index = astReader[iReaderId].iNum;//index保存删除之前的读者编号,因为删除
	//之后新的读者编号就会覆盖掉原来的读者编号
	if (cFlag == 'n')
	{
		return;
	}

	else if (cFlag == 'y')
	{
		for (i = iReaderId; i < iReaderRecord - 1; i++)
		{
			astReader[i] = astReader[i + 1];//数组依次前移
		}
		iReaderRecord--;
	}
	pfReader = fopen("reader.txt", "wb");
	if (pfReader != NULL)
	{
		for (i = 0; i < iReaderRecord; i++)
		{
			if (fwrite(&astReader[i], LEN_READER, 1, pfReader) != 1)
			{
				printf("无法保存该信息!\n");
				return;
			}
		}
		fclose(pfReader);
		printf("%d号读者信息已删除!\n", index);
	}
}
void ShowModifyReaderMenu()//显示修改的选项
{
	printf("\n");
	printf("\t                    1.编号                                   \n");
	printf("\t                    2.姓名                                   \n");
	printf("\t                    3.性别                                   \n");
	printf("\t                    4.最大可借阅数量                  \n");
	printf("请输入所要修改的信息(输入相应的数字:1-4):");
}

void SaveReaderFile(int iReaderId)
{
	FILE* pfReader;

	pfReader = fopen("reader.txt", "rb+");
	if (pfReader != NULL)
	{
		fseek(pfReader, LEN_READER * iReaderId, SEEK_SET);
		if (fwrite(&astReader[iReaderId], LEN_READER, 1, pfReader) != 1)
		{
			printf("无法保存该信息!\n");
		}
	}
	fclose(pfReader);
}

void  ModifyReader()//修改读者信息
{
	int   iReaderId, iReaderRecord, iBorrow, iItem, iNum, iMax, iFlagExist, i;
	system(" cls");
	iReaderId = SearchReader();
	if (iReaderId == -1)
	{
		return;
	}
	//找到该序号的读者,可以进行修改

	iReaderRecord = ReadReaderFile("rb");
	ShowModifyReaderMenu();//显示修改的选项
	scanf("%d", &iItem);
	getchar();

	switch (iItem)
	{
	case 1:
		printf("请输入读者编号:");
		do
		{
			iFlagExist = 0;
			scanf("%d", &iNum);
			getchar();
			for (i = 0; i < iReaderRecord; i++)
			{
				if (iNum == astReader[i].iNum && i != iReaderId)
				{
					iFlagExist = 1;
					printf("错误,该读者编号已经存在,请重新输入:");
					break;
				}
			}
		} while (iFlagExist == -1);
		astReader[iReaderId].iNum = iNum;
		break;
	case 2:
		printf("请输入读者姓名:");
		scanf("%s", astReader[iReaderId].acName);
		getchar();
		break;
	case 3:
		printf("请输入读者性别:F/M:");
		while (scanf("%s", astReader[iReaderId].acSex) > 0)
		{
			getchar();
			if (strcmp(astReader[iReaderId].acSex, "F") == 0 || strcmp(astReader[iReaderId].acSex, "M") == 0)
			{
				break;
			}
			printf("错误,只能输入'F'或者'M',请重新输入\n");
		}
		break;
	case 4:
		//iBorrow:已经借阅的数量
		iBorrow = astReader[iReaderId].iMax - astReader[iReaderId].iAmount;
		printf("请输入读者最大可借阅的数量(范围为5-10):\n");
		while (scanf("%d", &iMax) == 1)
		{
			getchar();
			if (iMax >= 5 && iMax <= 10)
			{
				if (iBorrow > iMax)
				{
					printf("该读者目前图书借阅数量大于该数目,需要先还书再修改!\n");
					return;
				}
				else
				{
					astReader[iReaderId].iMax = iMax;
					astReader[iReaderId].iAmount = iMax - iBorrow;
					for (i = iBorrow; i < iMax; i++)
					{
						astReader[iReaderId].BookId[i] = 0;
					}
					break;
				}
			}
			printf("错误,读者最大可借阅的书的范围为5-10,请重新输入\n");
		}
		break;
	}
	SaveReaderFile(iReaderId);
	printf("该读者信息已经修改成功!\n");

}


void  MangaeReader()//读者管理模块
{
	int rselect;
	ShowReaderMenu();//显示读者管理的子菜单
	scanf("%d", &rselect);
	getchar();

	while (rselect)
	{
		switch (rselect)
		{
		case 1:
			ShowReader();//显示读者信息;
			break;
		case 2:
			AddReader();//新增读者信息;
			break;
		case 3:
			SearchReader();//查找读者信息;
			break;
		case 4:
			DeleteReader();//删除读者信息;
			break;
		case 5:
			ModifyReader();//修改读者信息;
			break;
		defalut:
			printf("\t\t 请输入正确的数字!\n\t\t");
		}
		printf("|按任意键返回子菜单|");
		_getch();
		ShowReaderMenu();
		scanf("%d", &rselect);
		getchar();
	}
}

void ShowBorrowReturnMenu()//借还书子菜单
{
	system("cls");
	printf("\n\n\n\n");
	printf("\t************************欢迎进入**********************\n");
	printf("\t***********************借还书登记*********************\n");
	printf("\t************************子菜单************************\n");
	printf("\t***********************1.借书登记*********************\n");
	printf("\t***********************2.还书登记*********************\n");
	printf("\t***********************0.返回主菜单*******************\n");
	printf("\t*******************************************************\n");
	printf("\n");
	printf("\t\t请选择(0-2)");
}

void BorrowBook()//借书
{
	//读者是否可以借书
	//system("cls");
	int iBookId, iReaderId, iBorrow, i;
	//输入要借书的读者的编号,判断编号是否存在,如果在,显示读者已经借阅的图书信息
	iReaderId = SearchReader();
	if (iReaderId == -1)
	{
		return;
	}
	iBorrow = astReader[iReaderId].iMax - astReader[iReaderId].iAmount;
	if (iBorrow == 0)
	{
		printf("该读者目前没有借阅任何书");
	}
	else
	{
		printf("\t该读者当前所借图书为:");
		for (i = 0; i < iBorrow; i++)
		{
			printf("%d  ", astReader[iReaderId].BookId[i]);
		}
		printf("\n\n");
	}

	if (astReader[iReaderId].iAmount == 0)
	{
		printf("该读者可借书的数量为0,不能继续借书!\n");
	}
	//图书是否可借阅
	printf("\n按任意键输入要借阅的图书信息\n");
	_getch;
	iBookId = SearchBook();
	if (iBookId == -1)
	{
		return;
	}
	if (astBook[iBookId].iAmount == 0)
	{
		printf("该图书库存量为0!图书不可借阅!\n");
		return;
	}
	//借书
	astReader[iReaderId].BookId[iBorrow] = astBook[iBookId].iNum;
	//图书的库存减一
	astBook[iBookId].iAmount--;
	//当前读者的可借书的数量减一;
	astReader[iReaderId].iAmount--;

	SaveBookFile(iBookId);
	SaveReaderFile(iReaderId);

	printf("借书成功!\n");
}

void  ReturnBook()//还书
{
	system("cls");
	int iBookId, iReaderId, iBorrow, i, j;
	//1.读者在不在
	iReaderId = SearchReader();
	if (iReaderId == -1)
	{
		return;
	}
	iBorrow = astReader[iReaderId].iMax - astReader[iReaderId].iAmount;
	if (iBorrow == 0)
	{
		printf("\t该读者没有借阅任何书,无需归还!\n");
		return;
	}
	else
	{
		printf("\t该读者当前所借阅的图书为:");
		for (i = 0; i < iBorrow; i++)
		{
			printf("%d  ", astReader[iReaderId].BookId[i]);
		}
	}
	printf("按任意键输入要归还的图书信息\n");
	_getch();
	//2.图书在不在
	iBookId = SearchBook();
	if (iBookId == -1)
	{
		return;
	}
	//3.借书列表里面有没有这本书
	for (i = 0; i < iBorrow; i++)
	{
		if (astReader[iReaderId].BookId[i] == astBook[iBookId].iNum)
		{
			//还书
			for (j = i; j < iBorrow - 1; j++)
			{
				astReader[iReaderId].BookId[j] = astReader[iReaderId].BookId[j + 1];
			}
			astReader[iReaderId].BookId[iBorrow - 1] = 0;
			//图书库存加1
			astBook[iBookId].iAmount++;
			//读者的可借书的数量加1
			astReader[iReaderId].iAmount++;
			break;
		}
	}

	if (i == iBorrow)
	{
		printf("该读者没有借阅这本书,无需归还!\n");
		return;
	}

	SaveBookFile(iBookId);
	SaveReaderFile(iReaderId);
	printf("还书成功!\n");
}


void BorrowReturnManger()//借还书模块
{
	ShowBorrowReturnMenu();//借还书的子菜单
	int  iItem;
	scanf("%d", &iItem);
	getchar();
	while (iItem)
	{
		switch (iItem)
		{
		case 1:
			BorrowBook();//借书
			break;
		case 2:
			ReturnBook();//还书
			break;
		default:
			printf("\t\t请输入正确的数字!\n");
		}
		printf("按任意键返回子菜单");
		_getch();
		ShowBorrowReturnMenu();
		scanf("%d", &iItem);
		getchar();
	}

}

int main()
{
	//显示主函数的界面;
	//Init();//加载
	ShowMainMeun();
	int  iItem;
	scanf("%d", &iItem);
	while (iItem)
	{
		switch (iItem)
		{
		case 1:
			MangaeBook();//图书管理模块
			break;
		case 2:
			MangaeReader();//读者管理模块
			break;
		case 3:
			BorrowReturnManger();//借还书模块
			break;
		default:
			printf("\t\t请输入正确的数字!\n\t程序将于3秒后跳转到主菜单");
			Sleep(3000);
		}
		ShowMainMeun();
		scanf("%d", &iItem);
		getchar();
	}
	return 0;

}