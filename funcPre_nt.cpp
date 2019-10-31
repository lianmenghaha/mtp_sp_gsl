#ifndef FUNCPRE_NT_CPP
#define FUNCPRE_NT_CPP
void funcPre_nt(int psol)
{
    //read the information from Gurobi
    
    //optimal solutions
    for(int i=1;i!=maxL->intrval+1;++i)
    {
	combi *c=new combi;
	setcmb.insert(c);
	for(auto o:seto)
	{
	    if(o->vl->intrval==i)
	    {
		c->setp.insert(o);
		c->scpx.push_back(o->cpx);
		c->scpy.push_back(o->cpy);
		c->x_minmax.push_back(make_pair(o->oxmin,o->oxmax));
		c->y_minmax.push_back(make_pair(o->oymin,o->oymax));
		

		double cxmin=*min_element(c->scpx.begin(),c->scpx.end());
		double cxmax=*max_element(c->scpx.begin(),c->scpx.end());
		double cymin=*min_element(c->scpy.begin(),c->scpy.end());
		double cymax=*max_element(c->scpy.begin(),c->scpy.end());
		if((cxmin==cxmax)&&(cymin!=cymax))
		{
		    c->m=1;
		    c->xypc=cxmin;
		}
		if((cymin==cymax)&&(cxmin!=cxmax))
		{
		    c->m=2;
		    c->xypc=cymin;
		}
		else
		    c->m=0;
	    }
	}
    }

    //sub-optimal solutions
    for(int k=0;k!=psol;++k)
    {
	for(int i=1;i!=maxL->intrval2[k]+1;++i)
	{
	    combi *c=new combi;
	    setcmb.insert(c);
	    for(auto o:seto)
	    {
		if(o->vl->intrval2[k]==i)
		{
		    c->setp.insert(o);
		    c->scpx.push_back(o->cpx);
		    c->scpy.push_back(o->cpy);
		    c->x_minmax.push_back(make_pair(o->oxmin,o->oxmax));
		    c->y_minmax.push_back(make_pair(o->oymin,o->oymax));

		    double cxmin=*min_element(c->scpx.begin(),c->scpx.end());
		    double cxmax=*max_element(c->scpx.begin(),c->scpx.end());
		    double cymin=*min_element(c->scpy.begin(),c->scpy.end());
		    double cymax=*max_element(c->scpy.begin(),c->scpy.end());
		    if((cxmin==cxmax)&&(cymin!=cymax))
		    {
			c->m=1;
			c->xypc=cxmin;
		    }
		    if((cymin==cymax)&&(cxmin!=cxmax))
		    {
			c->m=2;
			c->xypc=cymin;
		    }
		    else
			c->m=0;

		}
	    }
	}
    }
}
    


















#endif
