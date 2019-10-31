#ifndef NEWTON_FN_CPP
#define NEWTON_FN_CPP
//f
double dtime_f(double x,void *params)
{
    struct dtime_params *p=(struct dtime_params *)params;

    int n=p->n;
    int m=p->m;
    vector<pair<double,double>> x_minmax=p->x_minmax;
    vector<pair<double,double>> y_minmax=p->y_minmax;
    double xycp=p->xycp;
    double f=0;

    //f
    if(m==1)//x-coord. are the same
    {
	for(int i=0;i!=n;++i)
	{
	    double erfy=erf_Z_safe((y_minmax[i]).first-x)-erf_Z_safe((y_minmax[i]).second-x);
	    double erfxc=gsl_cdf_ugaussian_P((x_minmax[i]).second-xycp)-gsl_cdf_ugaussian_P((x_minmax[i]).first-xycp);

	    f+=2*M_PI*erfxc*erfy;
	}
    }
    if(m==2)//y-coord. are the same
    {
	for(int i=0;i!=n;++i)
	{
	    double erfx=erf_Z_safe((x_minmax[i]).first-x)-erf_Z_safe((x_minmax[i]).second-x);
	    double erfyc=gsl_cdf_ugaussian_P((y_minmax[i]).second-xycp)-gsl_cdf_ugaussian_P((y_minmax[i]).first-xycp);

	    f+=2*M_PI*erfx*erfyc;
	}
    }
    return f;
}

//df
double dtime_df(double x,void *params)
{
    struct dtime_params *p=(struct dtime_params *)params;

    int n=p->n;
    int m=p->m;
    vector<pair<double,double>> x_minmax=p->x_minmax;
    vector<pair<double,double>> y_minmax=p->y_minmax;
    double xycp=p->xycp;
    double df=0;
    if(m==1)//x-coord. are the same
    {
	for(int i=0;i!=n;++i)
	{
	    double erfxc=gsl_cdf_ugaussian_P((x_minmax[i]).second-xycp)-gsl_cdf_ugaussian_P((x_minmax[i]).first-xycp);
	    double erfy1=erf_Z_safe((y_minmax[i]).first-x);
	    double erfy2=erf_Z_safe((y_minmax[i]).second-x);

	    df+=2*M_PI*erfxc*(erfy1*((y_minmax[i]).first-x)-erfy2*((y_minmax[i]).second-x));
	}
    }
    if(m==2)//y-coord. are the same
    {
	for(int i=0;i!=n;++i)
	{
	    double erfyc=gsl_cdf_ugaussian_P((y_minmax[i]).second-xycp)-gsl_cdf_ugaussian_P((y_minmax[i]).first-xycp);
	    double erfx1=erf_Z_safe((x_minmax[i]).first-x);
	    double erfx2=erf_Z_safe((x_minmax[i]).second-x);

	    df+=2*M_PI*(erfx1*((x_minmax[i]).first-x)-erfx2*((x_minmax[i]).second-x))*erfyc;
	}
    }

    return df;
}

//fdf
void dtime_fdf(double x,void *params,double *y,double *dy)
{
    struct dtime_params *p=(struct dtime_params *)params;

    int n=p->n;
    int m=p->m;
    vector<pair<double,double>> x_minmax=p->x_minmax;
    vector<pair<double,double>> y_minmax=p->y_minmax;
    double xycp=p->xycp;
    double f=0;
    double df=0;
    if(m==1)//x-coord. are the same
    {
	for(int i=0;i!=n;++i)
	{
	    double erfxc=gsl_cdf_ugaussian_P((x_minmax[i]).second-xycp)-gsl_cdf_ugaussian_P((x_minmax[i]).first-xycp);
	    double erfy1=erf_Z_safe((y_minmax[i]).first-x);
	    double erfy2=erf_Z_safe((y_minmax[i]).second-x);

	    f+=2*M_PI*erfxc*(erfy1-erfy2);
	    df+=2*M_PI*erfxc*(erfy1*((y_minmax[i]).first-x)-erfy2*((y_minmax[i]).second-x));
	}
    }
    if(m==2)//y-coord. are the same
    {
	for(int i=0;i!=n;++i)
	{
	    double erfyc=gsl_cdf_ugaussian_P((y_minmax[i]).second-xycp)-gsl_cdf_ugaussian_P((y_minmax[i]).first-xycp);
	    double erfx1=erf_Z_safe((x_minmax[i]).first-x);
	    double erfx2=erf_Z_safe((x_minmax[i]).second-x);

	    f+=2*M_PI*(erfx1-erfx2)*erfyc;
	    df+=2*M_PI*(erfx1*((x_minmax[i]).first-x)-erfx2*((x_minmax[i]).second-x))*erfyc;
	}
    }

    *y=f;
    *dy=df;
}

	    






















#endif

