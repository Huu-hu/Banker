#include<iostream>
#include<vector>
using namespace std;

int g_srcNum, g_proNum;
int flagCase;
int proID;
vector<int> Available;
vector<vector<int>> Max;
vector<vector<int>> Allocation;
vector<vector<int>> Need;
vector<int> Request;
vector<bool> Finish;
vector<int> Sum;
vector<int> Work;
vector<int> Sequence;

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

	for (int i = 0; i < g_proNum; i++)		//设置每种进程所需的各种资源数量
	{
		vector<int> maxTemp;
		for (int j = 0; j < g_srcNum; j++)
		{
			int temp = 0;
			cout << "输入第" << i + 1 << "进程的第" << j + 1 << "种资源所需要的最大数量:" ;
			cin >> temp;
			maxTemp.push_back(temp);
		}
		Max.push_back(maxTemp);
	}
	for (int i = 0; i < g_proNum; i++)		//设置各个进程已获得的资源数
	{
		vector<int> allocationTemp;
		for (int j = 0; j < g_srcNum; j++)
		{
			int temp;
			cout << "输入第" << i + 1 << "进程的第" << j + 1 << "种资源已获得数量：" ;
			cin >> temp;
			allocationTemp.push_back(temp);
		}
		Allocation.push_back(allocationTemp);
	}
	
	
	for (int i = 0; i < g_srcNum; i++)		//计算空闲资源
	{
		int temp = Sum[i];
		for (int j = 0; j < g_proNum; j++)
		{
			temp -= Allocation[j][i];
		}
		Available.push_back(temp);
	}

	for (int i = 0; i < g_proNum; i++)		//计算各个进程还需要的资源数量
	{
		vector<int> needTemp;
		int temp;
		for (size_t j = 0; j < g_srcNum; j++)
		{
			temp = Max[i][j] - Allocation[i][j];
			needTemp.push_back(temp);
		}
		Need.push_back(needTemp);
	}

	for (int i = 0; i < g_proNum; i++)		//初始化Finish标志
	{
		Finish.push_back(false);
	}

	//for (int j = 0; j < g_srcNum; j++)		//初始化Request
	//{
	//	Finish.push_back(0);
	//}
}										

void setRequest(vector<int> &Request)
{
	for (int i = 0; i < g_srcNum; i++)
	{
		int temp;
		cout << "输入第" << i + 1 << "种资源的请求数量：";
		cin >> temp;
		Request.push_back(temp);
	}
}


int safeCheck(vector<int> &Request, vector<vector<int>> &Need,vector<int> &Available, int proID)		//Need和Available检查
{
	int flagBool=1;		//设置检查标志量 -1未通过Need检查，0未通过Available检查，1通过检查
	//cout << "设置需要请求的进程的ID号:";
	//cin >> proID;
	/*setRequest(Request);*/
	for (int i = 0; i < g_srcNum; i++)
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

	//对资源进行预分配
	for (int i = 0; i < g_srcNum; i++)
	{
		Need[proID][i] -= Request[i];
	}

	//置标志位
	for (int i = 0; i < g_proNum; i++)
	{
		bool flagFinish = true;
		for (int j = 0; j < g_srcNum; j++)
		{
			if (Need[i][j]!=0)
			{
				flagFinish = false;
				break;
			}
		}
		if (flagFinish)
		{
			Finish[i] = true;
		}
	}

	//计算释放资源后Work数目
	if (Finish[proID]==true)
	{
		Sequence.push_back(proID);
		for (int i = 0; i < g_srcNum; i++)
		{
			Work[i] += Allocation[proID][i];
		}
	}

	//安全性算法检测
	//随机分配资源，分配以满足Need和Available优先
	bool finishChange = false;
	do
	{
		finishChange = false;		//Finish改变标志
		for (int i = 0; i < g_proNum; i++)
		{
			if (Finish[i] == true)
				continue;		//跳过已分配资源的进程
			bool checkTemp = false;
			for (int j = 0; j < g_srcNum; j++)
			{
				if (Need[i][j] > Work[j])
				{
					checkTemp = true;	//资源数量少于需求，分配失败
					break;
				}
			}
			if (checkTemp == true)
			{
				continue;
			}
			//进行资源分配
			Sequence.push_back(i);
			for (int j = 0; j < g_srcNum; j++)
			{
				Work[j] += Allocation[i][j];
				finishChange = true;
				Finish[i] = true;

			}
		}
		
	} while (finishChange);	
	for (int i = 0; i < g_proNum; i++)
	{
		if (Finish[i]==false)		//返回结果
		{								//回滚数据
			Need = tempNeed;
			Allocation = tempAllocation;
			Finish = tempFinish;
			return false;
		}
	}
	return true;

}


void resultFunction(bool flag)
{
	if (flag)
	{
		cout << "进程执行顺序为：";
		for (int i = 0; i < g_proNum; i++)
		{
			cout << Sequence[i]+1 << ",";
		}
		cout << endl;
		cout << "系统安全不会发生死锁" << endl;
	}
	else
	{
		cout << "系统处于不安全状态，停止分配资源" << endl;
	}
}


int main()
{

	while (true)
	{
		cout << "输入数字选择相应功能（请顺序执行）" << endl;
		cout << "======================" << endl;
		cout << "1。初始化数据" << endl;
		cout << "2、选择要测试的第一个进程号" << endl;
		cout << "3、启动测试" << endl;
		cout << "4、退出系统" << endl;
		cout << "======================" << endl;
		cin >> flagCase;
		switch (flagCase)
		{
		case 1:init(Available, Max, Allocation, Need, Request, Finish, Sum); break;
		case 2:
		{
			cout << "输入进程号：" ;
			cin >> proID;
			proID--;
		}; break;
		case 3: {
			int flag;
			setRequest(Request);
			flag = safeCheck(Request, Need, Available, proID);
			if (flag == -1)
			{
				cout << "未通过Need检查" << endl;
				break;
			}
			else
			{
				if (flag == 1)
				{
					resultFunction(bankerTest(Request, Need, Available, Finish, Allocation, Max, proID));
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
}
