#include<iostream>
#include<vector>
using namespace std;

int g_srcNum, g_proNum;

void init(vector<int> &Available, vector<vector<int>> &Max, vector<vector<int>> &Allocation, vector<vector<int>> &Need, vector<int> &Request, vector<bool> &Finish,vector<int> &Sum)		//初始化相关变量
{
	cout << "输入资源数量：";
	cin >> g_srcNum;
	cout << "输入需要测试的进程数量";

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
			cout << "输入第" << i + 1 << "进程的第" << j + 1 << "种资源所需要的最大数量" << endl;
			cin >> Max[i][j];
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

int safeCheck(vector<int> &Request, vector<vector<int>> &Need,vector<vector<int>> &Available)		//Need和Available检查
{
	int proID;
	int flagBool=1;		//设置检查标志量 -1未通过Need检查，0未通过Available检查，1通过检查
	cout << "设置需要请求的进程的ID号:";
	cin >> proID;
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
		if (Request[i]>Available[proID][i])
		{
			return 0;
		}
	}

	return flagBool;
}

