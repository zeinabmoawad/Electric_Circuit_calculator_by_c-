#include"component.h"
#include<complex>
#define PI 3.14
#pragma once
class Cccs :public component
{
private:
	double constant;
	string dependent;
	int node3;
	int node4;
public:
	Cccs(string n,int n1,int n2,int n3,int n4,string dep,int cont):component("Cccs",n,n1,n2)
	{
		node3=n3;
		node4=n4;
		dependent=dep;
		constant=cont;
	}

	int getnode3()
	{
		return node3;
	}

	int getnode4()
	{
		return node4;
	}

	string getdependent()
	{
		return dependent;
	}
	virtual complex<double>getvalue()
	{
		/*return constant;*/
		return (0,0);
	}

	double getconst()
	{
		return constant;
	}
	~Cccs(void)
	{}
};

