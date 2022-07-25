#include"component.h"
#include<complex>
#define PI 3.14
#pragma once
class Isrc: public component
{
private:
	complex<double>current;
public:
	Isrc(string n,int n1,int n2,double value ,double phase):component("Isrc",n,n1,n2)
	{
		if(phase!=90)
		{
			current.real(value*cos(phase*(PI/180)));
		}
		else 
			current.real(0);
		if(phase!=0)
		{
			current.imag(value*sin(phase*(PI/180)));
		}
		else 
			current.imag(0);
	}
	virtual complex<double>getvalue()
	{
		return current;
	}
	~Isrc(void)
	{}
};

