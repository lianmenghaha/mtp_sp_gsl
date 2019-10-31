#ifndef FUNCNEWTON_CPP
#define FUNCNEWTON_CPP

void funcNewton()
{
    for(auto c:setcmb)
    {
	int n=c->setp.size();
	if(n==1)
	{
	    //only one patten
	    double cxmin=c->x_minmax[0].first;
	    double cxmax=c->x_minmax[0].second;
	    double cymin=c->y_minmax[0].first;
	    double cymax=c->y_minmax[0].second;
	    double cx=c->scpx[0];
	    double cy=c->scpy[0];

	    //
	    double maxdx=gsl_cdf_ugaussian_P(cxmax-cx)-gsl_cdf_ugaussian_P(cxmin-cx);
	    double maxdy=gsl_cdf_ugaussian_P(cymax-cy)-gsl_cdf_ugaussian_P(cymin-cy);
	    c->maxdt=sqrt(2*M_PI)*maxdx*maxdy;
	}
	else//combinations with more than one pattern
	{
	    if(c->m!=0)
	    {
		//for every cp we do once iteration
		for(int k=0;k!=c->setp.size();++k)
		{
		    int status;
		    int iter=0,max_iter=100;

		    const gsl_root_fdfsolver_type *T;
		    gsl_root_fdfsolver *s;

		    double x0;
		    double x;//the initial guess
		    if(c->m==1)//x-coord. same
			x=c->scpy[k];
		    if(c->m==2)//y-coord. same
			x=c->scpx[k];

		    //fdf function
		    gsl_function_fdf FDF;
		    //params
		    struct dtime_params params={n,c->x_minmax,c->y_minmax,c->xypc,c->m};

		    FDF.f=&dtime_f;
		    FDF.df=&dtime_df;
		    FDF.fdf=&dtime_fdf;
		    FDF.params=&params;

		    //we use Newton Raphson Method
		    T=gsl_root_fdfsolver_newton;
		    s=gsl_root_fdfsolver_alloc(T);

		    gsl_root_fdfsolver_set(s,&FDF,x);
		
		    do
		    {
			iter++;
			status=gsl_root_fdfsolver_iterate(s);
			x0=x;
			x=gsl_root_fdfsolver_root(s);

			//we set the Search Stopping Parameters
			//int gsl_root_test_delta(x,x0,epsabs,epsrel)
			//|x-x0|<epsabs+epsrel|x|
			status=gsl_root_test_delta(x,x0,0,1e-12);
			//|x-x0|<1e-12*|x|
			if(status==GSL_SUCCESS)
			{
			    if(c->m==1)//x-coord.same
			    {
				if((x>=c->y_minmax[k].first)&&(x<=c->y_minmax[k].second))//x in [ymin,ymax]
				    c->xy.push_back(make_pair(c->xypc,x));
				if(x<c->y_minmax[k].first)
				    c->xy.push_back(make_pair(c->xypc,c->y_minmax[k].first));
				if(x>c->y_minmax[k].second)
				    c->xy.push_back(make_pair(c->xypc,c->y_minmax[k].second));
			    }
			    if(c->m==2)//y-coord.same
			    {
				if((x>=c->x_minmax[k].first)&&(x<=c->x_minmax[k].second))
				    c->xy.push_back(make_pair(x,c->xypc));
				if(x<c->x_minmax[k].first)
				    c->xy.push_back(make_pair(c->x_minmax[k].first,c->xypc));
				if(x>c->x_minmax[k].second)
				    c->xy.push_back(make_pair(c->x_minmax[k].second,c->xypc));
			    }
			}
		    }
		    while(status==GSL_CONTINUE&&iter<max_iter);
		    gsl_root_fdfsolver_free(s);
		}//for loop
	    }//if(c->m!=0)
	    else//for m==0
	    {
		//for every cp we do once iteration
		for(int k=0;k!=c->setp.size();++k)
		{
		    const gsl_multiroot_fdfsolver_type *T;
		    gsl_multiroot_fdfsolver *s;

		    int status;
		    size_t iter=0,max_iter=1000;
		    //the dimension of the system
		    //i.e.,the number of components of the vectors x and f
		    const size_t dim=2;

		    //params
		    struct dtime_mt_params p={n,c->x_minmax,c->y_minmax};
		    gsl_multiroot_function_fdf f={&dtime_f,&dtime_df,&dtime_fdf,dim,&p};
		    //the initial guess
		    double x_init[dim]={c->scpx[k],c->scpy[k]};//cp=(x,y)
		    gsl_vector *x=gsl_vector_alloc(dim);

		    gsl_vector_set(x,0,x_init[0]);
		    gsl_vector_set(x,1,x_init[1]);

		    //Newton's Method
		    T=gsl_multiroot_fdfsolver_newton;
		    s=gsl_multiroot_fdfsolver_alloc(T,dim);
		    gsl_multiroot_fdfsolver_set(s,&f,x);

		    do
		    {
			iter++;
			status=gsl_multiroot_fdfsolver_iterate(s);
			if(status)//check if solver is stuck
			    break;
			//we set the Search Stopping Parameters
			//int gsl_multiroot_test_residual (const gsl_vector *f,double epsabs)
			//sum_i|f_i|<epsabs
			//return GSL_SUCCESS GSL_CONTINUE
			//epsabs=1e-12
			status=gsl_multiroot_test_residual(s->f,1e-12);
			if(status==GSL_SUCCESS)
			{
			    //we store the message from NW
			    double solx=gsl_vector_get(s->x,0);
			    double soly=gsl_vector_get(s->x,1);
			    if((solx>=c->x_minmax[k].first)&&(solx<=c->x_minmax[k].second))//x in [xmin,xmax]
			    {
				if((soly>=c->y_minmax[k].first)&&(soly<=c->y_minmax[k].second))//y in [ymin,ymax]
				    c->xy.push_back(make_pair(solx,soly));
				if(soly<c->y_minmax[k].first)//y<ymin
				    c->xy.push_back(make_pair(solx,c->y_minmax[k].first));
				if(soly>c->y_minmax[k].second)//y>ymax
				    c->xy.push_back(make_pair(solx,c->y_minmax[k].second));
			    }//if x in [xmin,xmax]
			    if(solx<c->x_minmax[k].first)//x<xmin
			    {
				if((soly>=c->y_minmax[k].first)&&(soly<=c->y_minmax[k].second))
				    c->xy.push_back(make_pair(c->x_minmax[k].first,soly));
				if(soly<c->y_minmax[k].first)
				    c->xy.push_back(make_pair(c->x_minmax[k].first,c->y_minmax[k].first));
				if(soly>c->y_minmax[k].second)
				    c->xy.push_back(make_pair(c->x_minmax[k].first,c->y_minmax[k].second));
			    }//x<xmin
			    if(solx>c->x_minmax[k].second)//x>xmin
			    {
				if((soly>=c->y_minmax[k].first)&&(soly<=c->y_minmax[k].second))//y in [ymin,ymax]
				    c->xy.push_back(make_pair(c->x_minmax[k].second,soly));
				if(soly<c->y_minmax[k].first)//y<ymin
				    c->xy.push_back(make_pair(c->x_minmax[k].second,c->y_minmax[k].first));
				if(soly>c->y_minmax[k].second)//y>ymax
				    c->xy.push_back(make_pair(c->x_minmax[k].second,c->y_minmax[k].second));
			    }//x>xmax
			}
		    }//do
		    while(status==GSL_CONTINUE&&iter<max_iter);

		    gsl_multiroot_fdfsolver_free(s);
		    gsl_vector_free(x);
		}//for loop
	    }//else for m==0
	    //comput the drying time for this combination
	    for(auto vxy:c->xy)
	    {
		double dtmpx=vxy.first;
		double dtmpy=vxy.second;
		double ddt=0;
		for(int i=0;i!=n;++i)
		{
		    double dxmin=c->x_minmax[i].first;
		    double dxmax=c->x_minmax[i].second;
		    double dymin=c->y_minmax[i].first;
		    double dymax=c->y_minmax[i].second;
		    
		    double maxdx=gsl_cdf_ugaussian_P(dxmax-dtmpx)-gsl_cdf_ugaussian_P(dxmin-dtmpx);
		    double maxdy=gsl_cdf_ugaussian_P(dymax-dtmpy)-gsl_cdf_ugaussian_P(dymin-dtmpy);

		    ddt+=maxdx*maxdy;
		}
		ddt*=sqrt(2*M_PI);
		c->dt.insert(ddt);
	    }
	    c->maxdt=*max_element(c->dt.begin(),c->dt.end());
	}//else:combi with more than 1 pattern
	//jiance
	/*for(auto s:c->setp)
	    cout<<s->name<<" : ";
	cout<<c->vmaxdt<<endl;*/

    }
}		    



























#endif


