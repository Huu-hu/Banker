﻿#include<iostream>
#include<vector>
using namespace std;

int g_srcNum, g_proNum;

void init(vector<int> &Available, vector<vector<int>> &Max, vector<vector<int>> &Allocation, vector<vector<int>> &Need, vector<int> &Request, vector<bool> &Finish,vector<int> &Sum)		//初始化相关变量
{
	cout << "输入资源数量：";
	cin >> g_srcNum;
	cout << "输入需要测试的进程数量:";
	cin >> g_proNum;

	for (int i = 0; i < g_srcNum; i++)
	{
		int temp=0;
		cout << "输入第" << i + 1 << "种资源总量：";
		cin >> temp;
		Sum.push_back(temp);
	}

	for(int i=0;i<g_proNum;i++)		//设置每种进程所需的各种资源数量
		for (int j = 0; j < g_srcNum; j++)
		{
			int temp = 0;
			cout << "输入第" << i + 1 << "进程的第" << j + 1 << "种资源所需要的最大数量:" << endl;
			cin >> temp;
			Max[i].push_back(temp);
		}

	for (int i = 0; i < g_proNum; i++)		//设置各个进程已获得的资源数
	{
		for (int j = 0; j < g_srcNum; j++)
		{
			cout << "输入第" << i + 1 << "进程的第" << j + 1 << "种资源已获得数量" << endl;
			cin >> Allocation[i][j];
		}
	}
	
	for (int i = 0; i < g_srcNum; i++)		//计算空闲资源
	{
		for (int j = 0; j < g_proNum; j++)
		{
			Available[i] = Max[j][i] - Allocation[j][i];
		}
	}

	for (int i = 0; i < g_proNum; i++)		//计算各个进程还需要的资源数量
	{
		for (size_t j = 0; j < g_srcNum; j++)
		{
			Need[i][j] = Max[i][j] - Allocation[i][j];
		}
	}

	for (int i = 0; i < g_proNum; i++)		//初始化Finish标志
	{
		Finish[i] = false;
	}

	for (int j = 0; j < g_srcNum; j++)		//初始化Request
	{
		Request[j] = 0;
	}
}										

void setRequest(vector<int> &Request)
{
	for (int i = 0; i < g_srcNum; i++)
	{
		cout << "输入第" << i + 1 << "种资源的请求数量：";
		cin >> Request[i];
	}
}


int safeCheck(vector<int> &Request, vector<vector<int>> &Need,vector<int> &Available, int proID)		//Need和Available检查
{
	int flagBool=1;		//设置检查标志量 -1未通过Need检查，0未通过Available检查，1通过检查
	//cout << "设置需要请求的进程的ID号:";
	//cin >> proID;
	setRequest(Request);
	for (size_t i = 0; i < g_srcNum; i++)
	{
		if (Request[i]>Need[proID][i])
		{
			return -1;
		}
	}

	for (int i = 0; i < g_srcNum; i++)
	{
		if (Request[i]>Available[i])
		{
			return 0;
		}
	}

	return flagBool;
}

bool bankerTest(vector<int> &Request, vector<vector<int>> &Need, vector<int> &Available, vector<bool> &Finish, vector<vector<int>> &Allocation,vector<vector<int>> &Max,int proID)
{
	vector<int> Work = Available;		//创建Work容器
	//保存现场
	vector<vector<int>> tempNeed = Need;		//保存Need数组
	vector<vector<int>> tempAllocation = Allocation;		//保存Allocation数组
	vector<bool> tempFinish = Finish;
	int flagFinish = 1;		//Finish计数器,默认第一个请求成功

	//对资源进行预分配
	for (int i = 0; i < g_srcNum; i++)
	{
		Need[proID][i] -= Request[i];
	}

	//置标志位
	for (int i = 0; i < g_proNum; i++)
	{
		if (Need[proID][i]==0)
		{
			Finish[proID] = true;
		}
		else
		{
			Finish[proID] = false;
			flagFinish = 0;		//分配数量不够，计数器清零
			break;
		}
	}

	//计算释放资源后Work数目
	if (Finish[proID]==true)
	{
		for (int i = 0; i < g_srcNum; i++)
		{
			Work[i] += Allocation[proID][i];
		}
	}

	//安全性算法检测
	//随机分配资源，分配以满足Need和Available优先
	bool finishChange = false;		//Finish改变标志
	do
	{
		for (int i = 0; i < g_proNum; i++)
		{
			if (Finish[i] == true)
				continue;		//跳过已分配资源的进程
			bool checkTemp = false;
			for (int j = 0; j < g_srcNum; j++)
			{
				if (Need[i][j] > Work[j])
				{
					checkTemp = true;		//资源数量少于需求，分配失败
				}
			}
			if (checkTemp == true)
			{
				continue;
			}
			//进行资源分配
			for (int j = 0; j < g_srcNum; j++)
			{
				Work[j] += Allocation[i][j];
				finishChange = true;
				flagFinish++;
				Finish[i] = true;
			}
		}
	} while (finishChange);		
	if (flagFinish==g_proNum)		//返回结果
	{
		return true;
	}
	else
	{								//回滚数据
		Need = tempNeed;
		Allocation = tempAllocation;
		Finish = tempFinish;
		return false;
	}
}


void resultFunction(bool flag)
{
	if (flag)
	{
		cout << "系统安全不会发生死锁" << endl;
	}
	else
	{
		cout << "系统处于不安全状态，停止分配资源" << endl;
	}
}

void menu()
{
	int flagCase;
	cout << "输入数字选择相应功能（请顺序执行）" << endl;
	cout << "======================" << endl;
	cout << "1。初始化数据" << endl;
	cout << "2、选择要测试的第一个进程号" << endl;
	cout << "3、启动测试" << endl;
	cout << "4、退出系统" << endl;
	cout << "======================" << endl;
	int proID;
	vector<int> Available;
	vector<vector<int>> Max;
	vector<vector<int>> Allocation;
	vector<vector<int>> Need;
	vector<int> Request;
	vector<bool> Finish;
	vector<int> Sum;
	vector<int> Work;
	cin >> flagCase;

	switch (flagCase)
	{
	case 1:init(Available, Max, Allocation, Need, Request, Finish, Sum); break;
	case 2:cin>>proID; break;
	case 3: {
		int flag;
		flag = safeCheck(Request,Need,Available,proID);
		if (flag==-1)
		{
			cout << "未通过Need检查" << endl;
			break;
		}
		else
		{
			if (flag==1)
			{
				resultFunction(bankerTest(Request,Need,Available,Finish,Allocation,Max,proID));
				break;
			}
			else
			{
				cout << "未通过Available检查" << endl;
				break;
			}
		}
	}
	case 4: exit(1);
	default:
		cout << "输入错误重新输入" << endl;
		break;
	}
}

int main()
{
	while (true)
		menu();
}
