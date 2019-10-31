#ifndef FUNCDTIME_CPP
#define FUNCDTIME_CPP
void funcDtime()
{
    for(auto oi:seto)
    {
	//jiance
	//cout<<oi->name<<" ";
	for(auto oj:seto)
	{
	    //jiance
	    //cout<<oj->name<<" ";
	    for(int i=0;i!=oi->opkt.size();++i)
	    {
		//x-coord.of points
		double xtmp=oi->opkt[i].first;
		//y-coord.of points
		double ytmp=oi->opkt[i].second;
		//xmin,xmax,ymin,ymax of oj
		double xmintmp=oj->oxmin;
		double xmaxtmp=oj->oxmax;
		double ymintmp=oj->oymin;
		double ymaxtmp=oj->oymax;

		double dtx=gsl_cdf_ugaussian_P(xmaxtmp-xtmp)-gsl_cdf_ugaussian_P(xmintmp-xtmp);
		double dty=gsl_cdf_ugaussian_P(ymaxtmp-ytmp)-gsl_cdf_ugaussian_P(ymintmp-ytmp);

		//drying time for this point
		oi->dtpkt[make_pair(i,oj)]=sqrt(2*M_PI)*dtx*dty;

		//jiance
		//cout<<oi->dtpkt[make_pair(i,oj)]<<endl;
	    }
	}
    }
}
















#endif
