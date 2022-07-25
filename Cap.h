#include"component.h"
#include<complex>

#pragma once
class Cap: public component
{
private:
	complex<double>Y;
public:
	Cap(string n,int n1,int n2,double w ,double ca):component("Cap",n,n1,n2)
	{
		Y.real(0);
		Y.imag(w*ca);
		//Y.imag(-1/ca);
	}
	virtual complex<double>getvalue()
	{
		return Y;
	}
	~Cap(void)
	{}
};

