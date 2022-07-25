#include"component.h"
#include<complex>
#define PI 3.14
#pragma once
class Vcvs :public component
{
private:
	double constant;
	int node3;
	int node4;
public:
	Vcvs(string n,int n1,int n2,int n3,int n4,int cont):component("Vcvs",n,n1,n2)
	{
		node3=n3;
		node4=n4;
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


	virtual complex<double>getvalue()
	{
		/*return constant;*/
		return (0,0);
	}

	double getconst()
	{
		return constant;
	}
	~Vcvs(void)
	{}
};

