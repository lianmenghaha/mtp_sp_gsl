#ifndef NEWTON_FN_H
#define NEWTON_FN_H
struct dtime_params
{
    //n patterns
    int n;
    //xmin,xmax of each pattern
    //x_minmax.size()=n;
    vector<pair<double,double>> x_minmax;
    //ymin,ymax of each pattern
    //y_minmax.size()=n;
    vector<pair<double,double>> y_minmax;
    //if the central points of the patterns have the same x-coordinate or y-coordinate, we denote it as xycp
    double xycp;
    //indicate: 1: the same x-coordinate
    //indicate: 2: the same y-coordinate
    int m;
};

double dtime_f(double x,void *params);
double dtime_df(double x,void *params);
void dtime_fdf(double x,void *params,double *y,double *dy);













#endif

