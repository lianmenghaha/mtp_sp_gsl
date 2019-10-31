#ifndef NEWTON_MULTI_FN_CPP
#define NEWTON_MULTI_FN_CPP
//for m=0
//i.e.,the central points of patterns are not the same
//f
int dtime_f(const gsl_vector *x,void *params,gsl_vector *f)
{
    struct dtime_mt_params *p=(struct dtime_mt_params *)params;
    int n=p->n;
    vector<pair<double,double>> x_minmax=p->x_minmax;
    vector<pair<double,double>> y_minmax=p->y_minmax;

    const double x0=gsl_vector_get(x,0);//x
    const double x1=gsl_vector_get(x,1);//y

    double y0=0;//f0
    double y1=0;//f1

    for(int i=0;i!=n;++i)
    {
	//Abl.von x
	double erfx0=erf_Z_safe((x_minmax[i]).first-x0)-erf_Z_safe((x_minmax[i]).second-x0);
	double erfy0=gsl_cdf_ugaussian_P((y_minmax[i]).second-x1)-gsl_cdf_ugaussian_P((y_minmax[i]).first-x1);

	y0+=2*M_PI*erfx0*erfy0;

	//Abl. von y
	double erfx1=gsl_cdf_ugaussian_P((x_minmax[i]).second-x0)-gsl_cdf_ugaussian_P((x_minmax[i]).first-x0);
	double erfy1=erf_Z_safe((y_minmax[i]).first-x1)-erf_Z_safe((y_minmax[i]).second-x1);

	y1+=2*M_PI*erfx1*erfy1;
    }
    const double f0=y0;
    const double f1=y1;

    gsl_vector_set(f,0,f0);
    gsl_vector_set(f,1,f1);

    return GSL_SUCCESS;
}

//df
int dtime_df(const gsl_vector *x,void *params,gsl_matrix *J)
{
    struct dtime_mt_params *p=(struct dtime_mt_params *)params;

    int n=p->n;
    vector<pair<double,double>> x_minmax=p->x_minmax;
    vector<pair<double,double>> y_minmax=p->y_minmax;

    const double x0=gsl_vector_get(x,0);
    const double x1=gsl_vector_get(x,1);

    double f00=0,f01=0,f11=0;//f01=f10

    for(int i=0;i!=n;++i)
    {
	//f00
	double erfx00=((x_minmax[i]).first-x0)*erf_Z_safe((x_minmax[i]).first-x0)-((x_minmax[i]).second-x0)*erf_Z_safe((x_minmax[i]).second-x0);
	double erfy00=gsl_cdf_ugaussian_P((y_minmax[i]).second-x1)-gsl_cdf_ugaussian_P((y_minmax[i]).first-x1);

	f00+=2*M_PI*erfx00*erfy00;

	//f01,f10
	double erfx01=erf_Z_safe((x_minmax[i]).first-x0)-erf_Z_safe((x_minmax[i]).second-x0);
	double erfy01=erf_Z_safe((y_minmax[i]).first-x1)-erf_Z_safe((y_minmax[i]).second-x1);

	f01+=2*M_PI*erfx01*erfy01;

	//f11
	double erfx11=gsl_cdf_ugaussian_P((x_minmax[i]).second-x0)-gsl_cdf_ugaussian_P((x_minmax[i]).first-x0);
	double erfy11=((y_minmax[i]).first-x1)*erf_Z_safe((y_minmax[i]).first-x1)-((y_minmax[i]).second-x1)*erf_Z_safe((y_minmax[i]).second-x1);

	f11+=2*M_PI*erfx11*erfy11;
    }
    const double df00=f00;
    const double df01=f01;
    const double df10=f01;
    const double df11=f11;

    gsl_matrix_set(J,0,0,df00);
    gsl_matrix_set(J,0,1,df01);
    gsl_matrix_set(J,1,0,df10);
    gsl_matrix_set(J,1,1,df11);

    return GSL_SUCCESS;
}

int dtime_fdf(const gsl_vector *x,void *params,gsl_vector *f,gsl_matrix *J)
{
    dtime_f(x,params,f);
    dtime_df(x,params,J);

    return GSL_SUCCESS;
}
    

#endif

