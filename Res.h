#include"component.h"
#include<complex>

#pragma once
class Res: public component
{
private:
	complex<double>Y;
public:
	Res(string n,int n1,int n2,double res):component("Res",n,n1,n2)
	{
		Y.real(1/res);
		Y.imag(0);
	}
	virtual complex<double>getvalue()
	{
		return Y;
	}	
	~Res(void)
	{}
};

