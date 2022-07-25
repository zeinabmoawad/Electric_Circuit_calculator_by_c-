#include<iostream>
using namespace std;

#pragma once
class component
{
	string symbol;
	string name;
	int node1;
	int node2;
public:

	component(string s,string n,int n1,int n2)
	{
		symbol =s;
		name =n;
		node1=n1;
		node2=n2;
	}
	int getnode1()
	{
		return node1;
	}

	int getnode2()
	{
		return node2;
	}

	string getsymbol()
	{
		return symbol;
	}
	string getname()
	{
		return name;
	}
	virtual complex<double>getvalue()=0;
	~component(void)
	{}
};

