#include"component.h"
#include<complex>
#define PI 3.14
#pragma once
class Vsrc: public component
{
private:
	complex<double>volatge;
public:
	Vsrc(string n,int n1,int n2,double value ,double phase):component("Vsrc",n,n1,n2)
	{
		if(phase!=90)
		{
		volatge.real(value*cos(phase*(PI/180)));
		}
		else volatge.real(0);
		if(phase!=0)
		{
		volatge.imag(value*sin(phase*(PI/180)));
		}
		else volatge.imag(0);
	}
	virtual complex<double>getvalue()
	{
		return volatge;
	}
	~Vsrc(void)
	{}
};

