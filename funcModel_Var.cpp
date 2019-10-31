#ifndef FUNCMODEL_VAR_CPP
#define FUNCMODEL_VAR_CPP

void funcModel_Var()
{
    //max printing group,N_P
    maxL=new var;
    maxL->index=vvar.size();//begin at 0
    maxL->lb=0;
    maxL->ub=seto.size();//N_G<=N_P,N_G is the number of patterns
    maxL->typ=1;
    vvar.push_back(maxL);
	//cout<<"maxL"<<endl;

    //o will be assigned into printing group o->vl
    //o->vl
    for(auto o:seto)
    {
	o->vl=new var;
	o->vl->index=vvar.size();
	o->vl->lb=1;
	o->vl->ub=seto.size();
	o->vl->typ=1;
	vvar.push_back(o->vl);
    }
	//cout<<"o->vl"<<endl;

	//binary variable for merging conflict
	//q_{(i,j),MC}
	for(auto o:seto)
	{
		for(auto on:o->setco)
		{
			o->qmc[on]=new var;
			o->qmc[on]->index=vvar.size();
			o->qmc[on]->lb=0;
			o->qmc[on]->ub=1;
			o->qmc[on]->typ=0;
			vvar.push_back(o->qmc[on]);
		}
	}
			

    //binary variable,q_{P_i,gm},i=1,_,Np,gm=1,_Ng
    //bvl[i]=1,indicates that obj. is assigned to printing group i;
    //bvl
    for(auto o:seto)
    {
	o->bvl=new var*[seto.size()+1];//index from 0,_,seto.size()
	for(int i=1;i!=seto.size()+1;++i)
	{
	    o->bvl[i]=new var;
	    o->bvl[i]->index=vvar.size();
	    o->bvl[i]->lb=0;
	    o->bvl[i]->ub=1;
	    o->bvl[i]->typ=0;
	    vvar.push_back(o->bvl[i]);
	}
    }
	//cout<<"bvl"<<endl;
    //binary variable,q_{(P_i,P_j),gm},i,j=1,_,Np,gm=1,_,Ng
    //=1,when P_i,P_j are assigned to gm-th group
    //zvl
    /*for(auto oi:seto)
    {
	for(auto oj:seto)
	{
	    //oi->zvl[oj]=new var*[seto.size()+1];
		//zvl[make_pair(oi,oj)]=new var*[seto.size()+1];
	    for(int i=1;i!=seto.size()+1;++i)
	    {
		zvl[make_pair(oi,oj)][i]=new var;
		zvl[make_pair(oi,oj)][i]->index=vvar.size();
		//zvl[make_pair(oi,oj)][i]->bindex=bvar.size();
		zvl[make_pair(oi,oj)][i]->lb=0;
		zvl[make_pair(oi,oj)][i]->ub=1;
		zvl[make_pair(oi,oj)][i]->typ=0;
		vvar.push_back(zvl[make_pair(oi,oj)][i]);
		//bvar.push_back(zvl[make_pair(oi,oj)][i]);
	    }
	}
    }*/
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();++setit)
	{
		object *oi=*setit;
		for(set<object*>::iterator setitt=setit;setitt!=seto.end();++setitt)
		{
			object *oj=*setitt;
			set<object*> settmp={oi,oj};
			zvl[settmp]=new var*[seto.size()+1];
			for(int k=1;k!=seto.size()+1;++k)
			{
				zvl[settmp][k]=new var;
				zvl[settmp][k]->index=vvar.size();
				zvl[settmp][k]->lb=0;
				zvl[settmp][k]->ub=1;
				zvl[settmp][k]->typ=0;
				vvar.push_back(zvl[settmp][k]);
			}
		}
	}
	//cout<<"zvl"<<endl;


    //binary variable q_{Pi,Pj,MC},i,j=1,_,Np
    //=1,when Pj is printed earlier than Pi
    //qmc
    //for(auto o:seto)
    //{
	//for(auto on:o->setco)
	//{
	    //o->qmc[on]=new var;
	    //o->qmc[on]->index=vvar.size();
	    //o->qmc[on]->lb=0;
	    //o->qmc[on]->ub=1;
	    //o->qmc[on]->typ=0;
	    //vvar.push_back(o->qmc[on]);
	//}
    //}
    //continus variable t_{Pi,gm,r},r=1,_,n_{Pi}
    //tpkt
    /*for(auto o:seto)
    {
	o->tpkt=new var **[seto.size()+1];
	for(int i=1;i!=seto.size()+1;++i)
	    o->tpkt[i]=new var *[o->opkt.size()];
	for(int i=1;i!=seto.size()+1;++i)
	{
	    for(int j=0;j!=o->opkt.size();++j)
	    {
		o->tpkt[i][j]=new var;
		o->tpkt[i][j]->index=vvar.size();
		o->tpkt[i][j]->lb=0;
		o->tpkt[i][j]->ub=Mval;
		o->tpkt[i][j]->typ=2;
		vvar.push_back(o->tpkt[i][j]);
	    }
	}
    }*/
	for(auto o:seto)
	{
		for(int i=1;i!=seto.size()+1;++i)
		{
			for(int j=0;j!=o->opkt.size();++j)
			{
				o->tpkt[make_pair(i,j)]=new var;
				o->tpkt[make_pair(i,j)]->index=vvar.size();
				o->tpkt[make_pair(i,j)]->lb=0;
				o->tpkt[make_pair(i,j)]->ub=Mval;
				o->tpkt[make_pair(i,j)]->typ=2;
				vvar.push_back(o->tpkt[make_pair(i,j)]);
			}
		}
	}
    //continus var. t_(Pi,gm,maxPi)
    //tpktmax
    /*for(auto o:seto)
    {
	o->tpktmax=new var *[seto.size()+1];
	for(int i=1;i!=seto.size()+1;++i)
	{
	    o->tpktmax[i]=new var;
	    o->tpktmax[i]->index=vvar.size();
	    o->tpktmax[i]->lb=0;
	    o->tpktmax[i]->ub=Mval;
	    o->tpktmax[i]->typ=2;
	    vvar.push_back(o->tpktmax[i]);
	}
    }*/
	for(auto o:seto)
	{
		for(int i=1;i!=seto.size()+1;++i)
		{
			o->tpktmax[i]=new var;
			o->tpktmax[i]->index=vvar.size();
			o->tpktmax[i]->lb=0;
			o->tpktmax[i]->ub=Mval;
			o->tpktmax[i]->typ=2;
			vvar.push_back(o->tpktmax[i]);
		}
	}
    //continus var. t_{gm,maxgm}
    //vmaxdt
    for(int i=1;i!=seto.size()+1;++i)
    {
	layer *l=new layer;
	l->vmaxdt=new var;
	l->vmaxdt->index=vvar.size();
	l->vmaxdt->lb=0;
	l->vmaxdt->ub=Mval;
	l->vmaxdt->typ=2;
	vvar.push_back(l->vmaxdt);
	vecl.push_back(l);
    }
}
#endif
