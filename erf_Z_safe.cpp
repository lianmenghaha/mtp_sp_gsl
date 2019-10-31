#ifndef ERF_Z_SAFE_CPP
#define ERF_Z_SAFE_CPP
double erf_Z_safe(double value)
{
  gsl_sf_result eval;
  gsl_error_handler_t *old_error_handler=gsl_set_error_handler_off (); // turn off the error handler
  int code=gsl_sf_erf_Z_e(value,&eval); // compute the density, see gsl/gsl_errno.h for the error code
  gsl_set_error_handler(old_error_handler); //reset the error handler
  return (code==GSL_SUCCESS ? eval.val : 0.0);
}
#endif

