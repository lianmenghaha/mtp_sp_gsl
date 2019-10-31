#ifndef NEWTON_MULTI_FN_H
#define NEWTON_MULTI_FN_H
struct dtime_mt_params
{
    //n patterns
    int n;
    vector<pair<double,double>> x_minmax;
    vector<pair<double,double>> y_minmax;
};

int dtime_f(const gsl_vector *x,void *params,gsl_vector *f);
int dtime_df(const gsl_vector *x,void *params,gsl_matrix *J);
int dtime_fdf(const gsl_vector *x,void *params,gsl_vector *f,gsl_matrix *J);
















#endif

