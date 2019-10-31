#ifndef FUNCMATLAB
#define FUNCMATLAB
#include"MatlabDataArray.hpp"
#include"MatlabEngine.hpp"

void funcMatlab()
{
    auto start=chrono::system_clock::now();
    using namespace matlab::engine;
    //Start MATLAB engine synchronously
    std::unique_ptr<MATLABEngine> matlabPtr=startMATLAB();
    //Create MATLAB data array factory
    matlab::data::ArrayFactory factory;
    for(auto oi:seto)
    {
	for(auto oj:seto)
	{
	    for(int i=0;i!=oi->opkt.size();++i)
	    {
		//x-coord. of points
		double xtmp=oi->opkt[i].first;
		//y_coord. of points
		double ytmp=oi->opkt[i].second;
		//xmin,xmax,ymin,ymax of oj
		double xmintmp=oj->oxmin;
		double xmaxtmp=oj->oxmax;
		double ymintmp=oj->oymin;
		double ymaxtmp=oj->oymax;


		//Create variables
		auto xc=factory.createScalar<double>(xtmp);
		auto yc=factory.createScalar<double>(ytmp);
		auto xmin=factory.createScalar<double>(xmintmp);
		auto xmax=factory.createScalar<double>(xmaxtmp);
		auto ymin=factory.createScalar<double>(ymintmp);
		auto ymax=factory.createScalar<double>(ymaxtmp);



		//Put variables in the MATLAB workspace
		matlabPtr->setVariable(u"xc",std::move(xc));
		matlabPtr->setVariable(u"yc",std::move(yc));
		matlabPtr->setVariable(u"xmin",std::move(xmin));
		matlabPtr->setVariable(u"xmax",std::move(xmax));
		matlabPtr->setVariable(u"ymin",std::move(ymin));
		matlabPtr->setVariable(u"ymax",std::move(ymax));


		//Evaluate commands in MATLAB
		matlabPtr->eval(u"fun=@(x,y) 1./sqrt(2.*pi).*exp((-0.5).*(x.^2)-0.5.*(xc.^2)+x.*xc-0.5.*(y.^2)-0.5.*(yc.^2)+y.*yc);");
		matlabPtr->eval(u"q=integral2(fun,xmin,xmax,ymin,ymax);");
		//Get the result
		matlab::data::TypedArray<double> result=matlabPtr->getVariable(u"q");
		//push the result to coresponding place
		//map<pair<int,object*>,double>dtpkt;
		//result[0]
		oi->dtpkt[make_pair(i,oj)]=result[0];
	    }
	}
    }
    auto end=chrono::system_clock::now();
    chrono::duration<double> diff=end-start;
    cout<<"MATLAB needs "<<diff.count()<<" s to calculate"<<endl;
}
#endif
