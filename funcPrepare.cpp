#ifndef FUNCPREPARE
#define FUNCPREPARE

void funcPrepare()
{
	//from the elements in path, we want to find out for every object
    //oxmin,oxmax,oymin,oymax
    //we assume every pattern [oxmin,oxmax]*[oymin,oymax]
    int nboo=0;
    for(auto o:seto)
    {
	vector<double> vx,vy;
	for(auto xy:o->path)
	{
	    vx.push_back(xy.first);
	    vy.push_back(xy.second);
	}
	o->oxmin=*min_element(vx.begin(),vx.end());
	o->oxmax=*max_element(vx.begin(),vx.end());
	o->oymin=*min_element(vy.begin(),vy.end());
	o->oymax=*max_element(vy.begin(),vy.end());
	o->cpx=(o->oxmin+o->oxmax)/2;
	o->cpy=(o->oymin+o->oymax)/2;
	//检测
	//cout<<o->oxmin<<endl;
	//cout<<o->oxmax<<endl;
	//cout<<o->oymin<<endl;
	//cout<<o->oymax<<endl;
	//compute the coordinates for points in objects
	double deltax=(o->oxmax-o->oxmin)/o->nbio;
	double deltay=(o->oymax-o->oymin)/o->nbio;
	//检测
	//cout<<"deltax"<<deltax<<endl;
	//cout<<"deltay"<<deltay<<endl;
	for(int i=0;i!=o->nbio+1;++i)
	{
	    for(int j=0;j!=o->nbio+1;++j)
	    {
		o->opkt.push_back({o->oxmin+i*deltax,o->oymin+j*deltay});
	    }
	}
	//检测
	//for(int i=0;i!=o->opkt.size();++i)
	//cout<<o->opkt[i].first<<" "<<o->opkt[i].second<<endl;
	//cout<<o->name<<" has "<<o->opkt.size()<<" points."<<endl;
	nboo+=o->opkt.size();
    }
    cout<<"The number of points: "<<nboo<<endl;
}
#endif

