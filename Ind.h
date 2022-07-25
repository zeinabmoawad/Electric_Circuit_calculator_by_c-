#include"component.h"
#include<complex>

#pragma once
class Ind: public component
{
private:
	complex<double>Y;
public:
	Ind(string n,int n1,int n2,double w ,double l):component("Ind",n,n1,n2)
	{
		Y.real(0);
		Y.imag(-1/(w*l));
		//Y.imag(-1/l);
	}
	virtual complex<double>getvalue()
	{
		return Y;
	}
	~Ind(void)
	{}
};

