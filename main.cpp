#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<cmath>
#include<vector>
#include<map>
#include<set>
#include<string>
#include<cstring>
#include<ctime>
#include<algorithm>
#include"gurobi_c++.h"

#include"class.h"
#include"gv.h"
#define Mval 1000000

#include<gsl/gsl_errno.h>
#include<gsl/gsl_math.h>
#include<gsl/gsl_sf.h>
#include<gsl/gsl_randist.h>
#include<gsl/gsl_cdf.h>
#include<gsl/gsl_roots.h>
#include<gsl/gsl_vector.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_multiroots.h>



#include"funcRWFile.cpp"
#include"funcPrepare.cpp"
#include"funcDtime.cpp"
#include"funcModel_Var.cpp"
#include"funcModel_Cons.cpp"
#include"funcScreenMessage.cpp"
#include"funcGurobi.cpp"
#include"erf_Z_safe.cpp"
#include"Newton_fn.h"
#include"Newton_multi_fn.h"
#include"Newton_fn.cpp"
#include"Newton_multi_fn.cpp"
#include"funcPre_nt.cpp"
#include"funcNewton.cpp"
using namespace std;
int main (int argc,char**argv)
{
	cout<<"Start Reading Files: objects.txt conflict.txt"<<endl;
    funcReadFile(argv[1],argv[2]);
    cout<<"Finished Reading Files."<<endl;
    cout<<"Start Computing the Coordinates and Drying Time of the Points in each object."<<endl;
    funcPrepare();
    funcDtime();
    cout<<"Finished Computing."<<endl;
    cout<<"Start Building Variables."<<endl;
    funcModel_Var();
    cout<<"Finished Building Variables: "<<vvar.size()<<" Variables."<<endl;
    cout<<"Start Building Constraints:"<<endl;
    funcModel_Cons();
    cout<<"Finished Building Constraints: "<<nCons<<" Constraints."<<endl;
    funcGurobi(0,0.05,atoi(argv[3]),0.0,0.0,1,1,atoi(argv[4]),2);
    //min/max,heu,time,absgap,gap,display,focus,psol,psmode
    cout<<"Start Computing the Drying Time for each Printing Group with Newton Method"<<endl;
    funcPre_nt(atoi(argv[4]));//prepare for newton
    funcNewton();
    cout<<"Finished Computing"<<endl;
    funcWriteFile(argv[5],atoi(argv[4]),argv[6]);
    funcScreenMessage();
    return 0;
}
