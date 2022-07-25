#include<iostream>
using namespace std;
#include<Eigen/Eigen>
using namespace Eigen;
#include"component.h"
#include"Vsrc.h"
#include"Res.h"
#include"Ind.h"
#include"Cap.h"
#include"Isrc.h"
#include"Vcvs.h"
#include"Cccs.h"
#include<string>
#include<fstream>
ifstream input;
ofstream output;

int main()
{
	string s;
	cout << "Enter INPUT file name: ";
	cin >> s;
	input.open(s + ".txt");
	while (!input.is_open())
	{
		cout << "Wrong name, Enter a valid name\n";
		cin >> s;
		input.open(s + ".txt");
	}
	if (input.is_open())
	{
		component* complist[15];
		int compcount=0;
		int Nodes_Num=0;
		string w;
		input >> w;
		double w1 = stod(w);
		while (!input.eof())
		{
			string s;
			string name;
			string n1;
			string n2;
			int x;
			int y;
			/*for(int i=0;i<15;i++)
			{*/
			input>>s;
			if(s=="Vsrc")
			{
				string value;//mag
				string phase;
				input>>name>>n1>>n2>>value>>phase;
				x=stoi(n1);
				y=stoi(n2);
				double value1=stod(value);
				double phase1=stod(phase);
				complist[compcount]=new Vsrc(name,x,y,value1,phase1);
				compcount++;
			}
			else if(s=="Res")
			{
				string res;//mag
				input>>name>>n1>>n2>>res;
				double res1 =stod(res);
				x=stoi(n1);
				y=stoi(n2);
				complist[compcount]=new Res(name,x,y,res1);
				compcount++;
			}
			else if(s=="Cap")
			{
				string capcitance;//mag
				input>>name>>n1>>n2>>capcitance;
				double cap=stod(capcitance);
				x=stoi(n1);
				y=stoi(n2);
				complist[compcount]=new Cap(name,x,y,w1,cap);
				compcount++;
			}
			else if(s=="Ind")
			{
				string indu;//mag
				input>>name>>n1>>n2>>indu;
				x=stoi(n1);
				y=stoi(n2);
				double ind=stod(indu);
				complist[compcount]=new Ind(name,x,y,w1,ind);
				compcount++;
			}
			else if(s=="Isrc")
			{
				string value;//mag
				string phase;
				input>>name>>n1>>n2>>value>>phase;
				x=stoi(n1);
				y=stoi(n2);
				double v=stod(value);
				double p=stod(phase);
				complist[compcount]=new Isrc(name,x,y,v,p);
				compcount++;
			}
			else if(s=="Vcvs")
			{
				string cont;
				string n3;
				string n4;
				int a;
				int b;
				input>>name>>n1>>n2>>n3>>n4>>cont;
				a=stoi(n3);
				b=stoi(n4);
				x=stoi(n1);
				y=stoi(n2);
				double con=stod(cont);
				complist[compcount]=new Vcvs(name,x,y,a,b,con);
				compcount++;
			}
			else if(s=="Cccs")
			{
				string cont;
				string dependent;
				string n3;
				string n4;
				int a;
				int b;
				input>>name>>n1>>n2>>n3>>n4>>dependent>>cont;
				a=stoi(n3);
				b=stoi(n4);
				x=stoi(n1);
				y=stoi(n2);
				double con=stod(cont);
				complist[compcount]=new Cccs(name,x,y,a,b,dependent,con);
				compcount++;
			}
		}


		//calculating number of nodes;
		for(int i=0;i<compcount;i++)
		{
			if(complist[i]->getnode1()>Nodes_Num)
				Nodes_Num=complist[i]->getnode1();
			else if(complist[i]->getnode2()>Nodes_Num)
				Nodes_Num=complist[i]->getnode2();
		}

		Eigen::MatrixXcd Coff(Nodes_Num,Nodes_Num);
		Eigen::MatrixXcd constants(Nodes_Num,1);
		Eigen::MatrixXcd variables(Nodes_Num,1);

		//intializing to zero
		for(int i=0;i<Nodes_Num;i++)
		{
			for(int j=0;j<Nodes_Num;j++)
				Coff(i,j)=(0,0);
			constants(i,0)=(0,0);
			variables(i,0)=(0,0);
		}


		int equationorder=0;
		for(int i=1;i<=Nodes_Num;i++)
		{
			for(int j=0;j<compcount;j++)  //First componenet
			{
				if(i==complist[j]->getnode1())
				{
					if(complist[j]->getsymbol()=="Vsrc") //voltage source
					{
						Coff(equationorder,complist[j]->getnode1()-1)=complex<double>(1,0);
						if(complist[j]->getnode2()!=0)
							Coff(equationorder,complist[j]->getnode2()-1)=complex<double>(-1,0);
						constants(equationorder,0)=complist[j]->getvalue();
						equationorder++;
						break;
					}
					else if(complist[j]->getsymbol()=="Vcvs") //is voltage dependetnt source
					{
						if(((Vcvs*)(complist[j]))->getnode3()!=0)
							Coff(equationorder,((Vcvs*)(complist[j]))->getnode3()-1)-=complex<double>(((Vcvs*)(complist[j]))->getconst(),0);
						if(((Vcvs*)(complist[j]))->getnode4()!=0)
							Coff(equationorder,((Vcvs*)(complist[j]))->getnode4()-1)+=complex<double>(((Vcvs*)(complist[j]))->getconst(),0);
						Coff(equationorder,complist[j]->getnode1()-1)+=complex<double>(1,0);
						if(complist[j]->getnode2()!=0)
							Coff(equationorder,complist[j]->getnode2()-1)-=complex<double>(1,0);
						equationorder++;
						break;
					}
					else //not volatege source
					{
						bool VsrcFound=false;
						bool VcvsFound=false;
						for(int l=0;l<compcount;l++)
						{
							if(complist[l]->getnode1()==i||complist[l]->getnode2()==i)
							{
								if(complist[l]->getsymbol()=="Vsrc")
								{
									VsrcFound=true;
									break;	
								}
								else if(complist[l]->getsymbol()=="Vcvs")
								{
									VcvsFound=true;
									break;	
								}
							}
						}

						if(VsrcFound==true||VcvsFound==true)
							continue;
						else if(VsrcFound==false||VcvsFound==false)
						{
							if(complist[j]->getsymbol()=="Res"||complist[j]->getsymbol()=="Cap"||complist[j]->getsymbol()=="Ind"||complist[j]->getsymbol()=="Isrc"||complist[j]->getsymbol()=="Cccs")
							{
								if(complist[j]->getsymbol()=="Isrc")
								{
									constants(equationorder,0)+=complist[j]->getvalue();
								}
								else if(complist[j]->getsymbol()=="Cccs")
								{
									int m;
									for(m=0;m<compcount;m++)
									{
										if(((Cccs*)complist[j])->getdependent()==complist[m]->getname())
										{
											break;
										}
									}
									if(((Cccs*)(complist[j]))->getnode3()!=0)
									{
										Coff(equationorder,((Cccs*)(complist[j]))->getnode3()-1)+=complist[m]->getvalue();
										Coff(equationorder,((Cccs*)(complist[j]))->getnode3()-1)*=complex<double>(-1*((Cccs*)(complist[j]))->getconst(),0);
									}
									if(((Cccs*)(complist[j]))->getnode4()!=0)
									{
										Coff(equationorder,((Cccs*)(complist[j]))->getnode4()-1)-=complist[m]->getvalue();
										Coff(equationorder,((Cccs*)(complist[j]))->getnode4()-1)*=complex<double>(-1*((Cccs*)(complist[j]))->getconst(),0);
									}
								}
								else
								{
									Coff(equationorder,complist[j]->getnode1()-1)+=complist[j]->getvalue();
									if(complist[j]->getnode2()!=0)
										Coff(equationorder,complist[j]->getnode2()-1)-=complist[j]->getvalue();
								}
								for(int k=0;k<compcount;k++)
								{
									if(k==j)
										continue;
									if(complist[k]->getsymbol()!="Isrc"&&complist[k]->getsymbol()!="Cccs")
									{
										if(i==complist[k]->getnode1())
										{
											Coff(equationorder,complist[k]->getnode1()-1)+=complist[k]->getvalue();
											if(complist[k]->getnode2()!=0)
												Coff(equationorder,complist[k]->getnode2()-1)-=complist[k]->getvalue();
										}
										else if(i==complist[k]->getnode2())
										{
											Coff(equationorder,complist[k]->getnode2()-1)+=complist[k]->getvalue();
											if(complist[k]->getnode1()!=0)
												Coff(equationorder,complist[k]->getnode1()-1)-=complist[k]->getvalue();
										}
									}
									else if(complist[k]->getsymbol()=="Isrc")  //Isrc source
									{
										if(i==complist[k]->getnode1())
											constants(equationorder,0)+=(complist[k]->getvalue());
										else if(i==complist[k]->getnode2())
											constants(equationorder,0)-=(complist[k]->getvalue());

									}
									else if(complist[k]->getsymbol()=="Cccs")//Current controlled source
									{
										if(i==complist[k]->getnode1())
										{
											int m;
											for(m=0;m<compcount;m++)
											{
												if(((Cccs*)complist[k])->getdependent()==complist[m]->getname())
												{
													break;
												}
											}
											if(((Cccs*)(complist[k]))->getnode3()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)+=(((Cccs*)(complist[k]))->getconst()*complist[m]->getvalue());
												//Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)*=complex<double>(-1*(((Cccs*)(complist[k]))->getconst()),0);
											}
											if(((Cccs*)(complist[k]))->getnode4()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode4()-1)-=(((Cccs*)(complist[k]))->getconst()*complist[m]->getvalue());
												//Coff(equationorder,((Cccs*)(complist[k]))->getnode4()-1)*=complex<double>((-1*((Cccs*)(complist[k]))->getconst()),0);
											}
										}
										else if(i==complist[k]->getnode2())
										{
											int m;
											for(m=0;m<compcount;m++)
											{
												if(((Cccs*)complist[k])->getdependent()==complist[m]->getname())
												{
													break;
												}
											}
											if(((Cccs*)(complist[k]))->getnode3()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)-=(((Cccs*)(complist[k]))->getconst()*complist[m]->getvalue());
												//Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)*=complex<double>(((Cccs*)(complist[k]))->getconst(),0);
											}
											if(((Cccs*)(complist[k]))->getnode4()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode4()-1)+=(((Cccs*)(complist[k]))->getconst()*complist[m]->getvalue());
												//Coff(equationorder,((Cccs*)(complist[k]))->getnode4()-1)*=complex<double>(((Cccs*)(complist[k]))->getconst(),0);
											}
										}
									}
								}

							}
							equationorder++;
							break;
						}
					}

				}
				else if(i==complist[j]->getnode2())
				{
					if(complist[j]->getsymbol()=="Vsrc") //voltage source
					{
						Coff(equationorder,complist[j]->getnode1()-1)=complex<double>(-1,0);
						if(complist[j]->getnode2()!=0)
							Coff(equationorder,complist[j]->getnode2()-1)=complex<double>(1,0);
						constants(equationorder/*complist[j]->getnode1()-1*/,0)=complist[j]->getvalue();
						equationorder++;
						break;
					}
					else if(complist[j]->getsymbol()=="Vcvs") //is voltage dependetnt source
					{
						if(((Vcvs*)(complist[j]))->getnode3()!=0)
							Coff(equationorder,((Vcvs*)(complist[j]))->getnode3()-1)-=complex<double>(((Vcvs*)(complist[j]))->getconst(),0);
						if(((Vcvs*)(complist[j]))->getnode4()!=0)
							Coff(equationorder,((Vcvs*)(complist[j]))->getnode4()-1)+=complex<double>(((Vcvs*)(complist[j]))->getconst(),0);
						if(complist[j]->getnode1()!=0)
							Coff(equationorder,complist[j]->getnode1()-1)+=complex<double>(1,0);
						Coff(equationorder,complist[j]->getnode2()-1)-=complex<double>(1,0);
						equationorder++;
						break;
					}

					else //not volatege source
					{
						bool VsrcFound=false;
						bool VcvsFound=false;
						for(int l=0;l<compcount;l++)
						{
							if((complist[l]->getnode1()==i||complist[l]->getnode2()==i))
							{
								if(complist[l]->getsymbol()=="Vsrc")
								{
									VsrcFound=true;
									break;	}

								else if(complist[l]->getsymbol()=="Vcvs")
								{
									VcvsFound=true;
									break;	
								}
							}
						}
						if(VsrcFound==true||VcvsFound==true) 
							continue;
						else if(VsrcFound==false||VcvsFound==false)
						{

							if(complist[j]->getsymbol()=="Res"||complist[j]->getsymbol()=="Cap"||complist[j]->getsymbol()=="Ind"||complist[j]->getsymbol()=="Isrc"||complist[j]->getsymbol()=="Cccs")
							{
								if(complist[j]->getsymbol()=="Isrc")
								{
									constants(equationorder,0)-=complist[j]->getvalue();
								}
								else if(complist[j]->getsymbol()=="Cccs")
								{ 
									int m;
									for(m=0;m<compcount;m++)
									{
										//bool equal=(/*((Cccs*)complist[j])->getdependent())*/complist[m]->getname()==dependent);
										if((((Cccs*)complist[j])->getdependent())==complist[m]->getname())
										{
											break;
										}
									}
									if(((Cccs*)(complist[j]))->getnode3()!=0)
									{
										Coff(equationorder,((Cccs*)(complist[j]))->getnode3()-1)-=complist[m]->getvalue();
										Coff(equationorder,((Cccs*)(complist[j]))->getnode3()-1)*=complex<double>(((Cccs*)(complist[j]))->getconst(),0);
									}
									if(((Cccs*)(complist[j]))->getnode4()!=0)
									{
										Coff(equationorder,((Cccs*)(complist[j]))->getnode4()-1)+=complist[m]->getvalue();
										Coff(equationorder,((Cccs*)(complist[j]))->getnode4()-1)*=complex<double>(((Cccs*)(complist[j]))->getconst(),0);
									}

								}
								else
								{
									Coff(equationorder,complist[j]->getnode2()-1)+=complist[j]->getvalue();
									if(complist[j]->getnode1()!=0)
										Coff(equationorder,complist[j]->getnode1()-1)-=complist[j]->getvalue();
								}
								for(int k=0;k<compcount;k++)
								{
									if(k==j)
										continue;
									if(complist[k]->getsymbol()!="Isrc"&&complist[k]->getsymbol()!="Cccs")///
									{
										if(i==complist[k]->getnode1())
										{
											Coff(equationorder,complist[k]->getnode1()-1)+=complist[k]->getvalue();
											if(complist[k]->getnode2()!=0)
												Coff(equationorder,complist[k]->getnode2()-1)-=complist[k]->getvalue();
										}
										else if(i==complist[k]->getnode2())
										{
											Coff(equationorder,complist[k]->getnode2()-1)+=complist[k]->getvalue();
											if(complist[k]->getnode1()!=0)
												Coff(equationorder,complist[k]->getnode1()-1)-=complist[k]->getvalue();
										}
									}
									else if(complist[k]->getsymbol()=="Isrc")//Isrc source
									{
										if(i==complist[k]->getnode1())
											constants(equationorder,0)+=(complist[k]->getvalue());
										else if(i==complist[k]->getnode2())
											constants(equationorder,0)-=(complist[k]->getvalue());
									}
									else if(complist[k]->getsymbol()=="Cccs") //current controlled source
									{
										if(i==complist[k]->getnode1())
										{
											int m;
											for(m=0;m<compcount;m++)
											{
												//bool equal=(((Cccs*)complist[j])->getdependent())==(complist[m]->getname()==dependent);
												if((((Cccs*)complist[k])->getdependent())==(complist[m]->getname()))
												{
													break;
												}
											}
											if(((Cccs*)(complist[k]))->getnode3()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)+=((((Cccs*)(complist[k]))->getconst())*complist[m]->getvalue());
												//Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)*=complex<double>(-1*(((Cccs*)(complist[k]))->getconst()),0);
											}
											if(((Cccs*)(complist[k]))->getnode4()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode4()-1)-=((((Cccs*)(complist[k]))->getconst()))*complist[m]->getvalue();
												//Coff(equationorder,(((Cccs*)(complist[k]))->getnode4())-1)*=complex<double>((-1*((Cccs*)(complist[k]))->getconst()),0);
											}
										}
										else if(i==complist[k]->getnode2())
										{
											int m;
											for(m=0;m<compcount;m++)
											{
												//bool equal=(((Cccs*)complist[j])->getdependent())==(complist[m]->getname()==dependent);
												if((((Cccs*)complist[k])->getdependent())==(complist[m]->getname()))
												{
													break;
												}
											}
											if(((Cccs*)(complist[k]))->getnode3()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)-=(((Cccs*)(complist[k]))->getconst())*complist[m]->getvalue();
												//Coff(equationorder,((Cccs*)(complist[k]))->getnode3()-1)*=complex<double>(((Cccs*)(complist[k]))->getconst(),0);
											}
											if(((Cccs*)(complist[k]))->getnode4()!=0)
											{
												Coff(equationorder,((Cccs*)(complist[k]))->getnode4()-1)+=(((Cccs*)(complist[k]))->getconst())*complist[m]->getvalue();
												//Coff(equationorder,(((Cccs*)(complist[k]))->getnode4())-1)*=complex<double>(((Cccs*)(complist[k]))->getconst(),0);
											}
										}
									}
								}
							}
							equationorder++;
							break;
						}
					}

				}
			}
		}

		cout<<endl<<endl;
		cout<<"Coff Matrx:" <<endl;
		for(int i=0;i<Nodes_Num;i++)
		{
			for(int j=0;j<Nodes_Num;j++)
				cout<<Coff(i,j)<<" ";
			cout<<endl;
		}

		cout<<endl<<endl;
		cout<<"Const Matrx:" <<endl;
		for(int i=0;i<Nodes_Num;i++)
		{
			cout<<constants(i,0)<<endl;
		}



		variables+=Coff.jacobiSvd(ComputeThinU | ComputeThinV).solve(constants);

		cout<<endl<<endl;
		cout<<"variables Matrx:" <<endl;
		for(int i=0;i<Nodes_Num;i++)
		{
			cout<<"V ["<<i+1<<"] "<<variables(i,0)<<endl;
		}

		for(int i=1;i<=Nodes_Num;i++) //loop for each node
		{
			for(int k=0;k<compcount;k++)
			{
				if(i==complist[k]->getnode1())
				{
					if(complist[k]->getsymbol()=="Res"||complist[k]->getsymbol()=="Cap"||complist[k]->getsymbol()=="Ind")
					{
						if(complist[k]->getnode2()!=0)
						{
							cout<<"I ["<<i<<" "<<complist[k]->getnode2()<<"] "<<((variables(i-1,0)-variables(complist[k]->getnode2()-1,0))*complist[k]->getvalue())<<endl;
						}
						else //0
							cout<<"I ["<<i<<" "<<complist[k]->getnode2()<<"] " <<((variables(i-1,0))*complist[k]->getvalue())<<endl;
						break;
					}
					else if(complist[k]->getsymbol()=="Isrc")
					{
						cout<<"I ["<<complist[k]->getnode2()<<" "<<i<<"] " <<complist[k]->getvalue()<<endl;
						break;
					}
					else if(complist[k]->getsymbol()=="Vsrc")
					{
						//cout<<"Voltage source to be done"<<endl;
						/*break;*/
					}
				}

			}
		}
		int c;
		cin>>c;
		int y;

		return 0;
	}
}