#ifndef FUNCMODEL_CONS
#define FUNCMODEL_CONS
void funcModel_Cons()
{
    nCons=-1;
    for(auto o:seto)
    {
	//g_{Pi}<=Ng
	//o->vl-maxL<=0
	nCons++;
	mapcv[nCons].insert(o->vl);
	o->vl->cons_i.insert(nCons);
	o->vl->coef_cons[nCons]=1;
	mapcv[nCons].insert(maxL);
	maxL->cons_i.insert(nCons);
	maxL->coef_cons[nCons]=-1;
	mapcs[nCons]=-1;
	mapcc[nCons]=0;
    }
	//cout<<"maxL"<<endl;
     //(1)Modelling of Laplace Pressure Conflict
    for(auto o:seto)
    {
	//g_{Pi}+1<=g_{Pj}
	for(auto on:o->setbo)
	{
	    //o->vl+1<=on->vl
		//o->vl-on->vl<=1
	    nCons++;
	    mapcv[nCons].insert(o->vl);
	    o->vl->cons_i.insert(nCons);
	    o->vl->coef_cons[nCons]=1;
	    mapcv[nCons].insert(on->vl);
	    on->vl->cons_i.insert(nCons);
	    on->vl->coef_cons[nCons]=-1;
	    mapcs[nCons]=-1;
	    mapcc[nCons]=-1;
	}
    }
	//cout<<"Laplace"<<endl;
    //(2)Modelling of Merging Conflict
    for(auto o:seto)
    {
	//g_{Pi}+1<=g_{Pj}+Mq_{(i,j),MC}
	//g_{Pj}+1<=g_{Pi}+M-Mq_{(i,j),MC}
	for(auto on:o->setco)
	{
		//build var. q(Pi,Pj,M)
		/*var *q=new var;
		q->index=vvar.size();
		q->lb=0;
		q->ub=1;
		q->typ=0;
		vvar.push_back(q);*/
	    //o->vl+1<=on->vl+Mval*o->q
	    //o->vl-on->vl-Mval*o->q<=-1
	    nCons++;
	    mapcv[nCons].insert(o->vl);
	    o->vl->cons_i.insert(nCons);
	    o->vl->coef_cons[nCons]=1;
	    mapcv[nCons].insert(on->vl);
	    on->vl->cons_i.insert(nCons);
	    on->vl->coef_cons[nCons]=-1;
	    mapcv[nCons].insert(o->qmc[on]);
	    o->qmc[on]->cons_i.insert(nCons);
	    o->qmc[on]->coef_cons[nCons]=-1*Mval;
	    mapcs[nCons]=-1;
	    mapcc[nCons]=-1;
	    //on->vl+1<=o->vl+M-Mq
	    //on->vl-o->vl+Mval*q<=-1+M
	    nCons++;
	    mapcv[nCons].insert(on->vl);
	    on->vl->cons_i.insert(nCons);
	    on->vl->coef_cons[nCons]=1;
	    mapcv[nCons].insert(o->vl);
	    o->vl->cons_i.insert(nCons);
	    o->vl->coef_cons[nCons]=-1;
	    mapcv[nCons].insert(o->qmc[on]);
	    o->qmc[on]->cons_i.insert(nCons);
	    o->qmc[on]->coef_cons[nCons]=Mval;
	    mapcs[nCons]=-1;
	    mapcc[nCons]=-1+Mval;
	}
    }
    //(3)drying time
    for(auto o:seto)
    {
	//sum_{m=1}^Ng q_{i,m}=1
	//sum_i o->bvl[i]=1,i=1,_,seto.size()
	nCons++;
	for(int i=1;i!=seto.size()+1;++i)
	{
	    mapcv[nCons].insert(o->bvl[i]);
	    o->bvl[i]->cons_i.insert(nCons);
	    o->bvl[i]->coef_cons[nCons]=1;
	}
	mapcs[nCons]=0;
	mapcc[nCons]=1;
    }

    for(auto o:seto)
    {
	//g_{Pi}=sum_{i=1}^Ng i*q_{Pi,gm}
	//o->vl=sum_i i*o->bvl[i]
	//o->vl-sum_i i*o->bvl[i]=0
	nCons++;
	mapcv[nCons].insert(o->vl);
	o->vl->cons_i.insert(nCons);
	o->vl->coef_cons[nCons]=1;
	for(int i=1;i!=seto.size()+1;++i)
	{
	    mapcv[nCons].insert(o->bvl[i]);
	    o->bvl[i]->cons_i.insert(nCons);
	    o->bvl[i]->coef_cons[nCons]=-1*i;
	}
	mapcs[nCons]=0;
	mapcc[nCons]=0;
    }
	//cout<<"o->vl and o->bvl"<<endl;
	//q_{Pi,gm}+q_{Pj,gm}<=q_{(Pi,Pj),gm}+1
	//oi->bvl[k]+oj->bvl[k]<=zvl[make_pair(oi,oj)][k]+1
	//without redundant
	for(set<object*>::iterator setit=seto.begin();setit!=seto.end();)
	{
		object *oi=*setit;
		++setit;
		for(set<object*>::iterator setitt=setit;setitt!=seto.end();++setitt)
		{
			object *oj=*setitt;
			set<object*> settmp={oi,oj};
			for(int k=1;k!=seto.size()+1;++k)
			{
				nCons++;
				mapcv[nCons].insert(oi->bvl[k]);
				oi->bvl[k]->cons_i.insert(nCons);
				oi->bvl[k]->coef_cons[nCons]=1;
				mapcv[nCons].insert(oj->bvl[k]);
				oj->bvl[k]->cons_i.insert(nCons);
				oj->bvl[k]->coef_cons[nCons]=1;
				mapcv[nCons].insert(zvl[settmp][k]);
				zvl[settmp][k]->cons_i.insert(nCons);
				zvl[settmp][k]->coef_cons[nCons]=-1;
				mapcs[nCons]=-1;
				mapcc[nCons]=1;
			}
		}
	}
	//cout<<"without redundant"<<endl;

	
	for(auto o:seto)
	{
		for(int k=1;k!=seto.size()+1;++k)
		{
			//zvl[{o}][k]=o->bvl[k]
			//zvl[{o}][k]-o->bvl[k]=0
			nCons++;
			set<object*> settmp={o};
			mapcv[nCons].insert(zvl[settmp][k]);
			zvl[settmp][k]->cons_i.insert(nCons);
			zvl[settmp][k]->coef_cons[nCons]=1;
			mapcv[nCons].insert(o->bvl[k]);
			o->bvl[k]->cons_i.insert(nCons);
			o->bvl[k]->coef_cons[nCons]=-1;
			mapcs[nCons]=0;
			mapcc[nCons]=0;
		}
	}
	//cout<<"set zvl and bvl"<<endl;

	//with input Matlab,q_{(Pi,Pj),gm} to comput the t_{Pi,gm,r},r=1,_,n_{Pi}
	for(auto oi:seto)
	{
		for(int k=1;k!=seto.size()+1;++k)
		{
			for(int r=0;r!=oi->opkt.size();++r)
			{
				nCons++;
				//k-th printing group
				//r-th point in oi
				mapcv[nCons].insert(oi->tpkt[make_pair(k,r)]);
				oi->tpkt[make_pair(k,r)]->cons_i.insert(nCons);
				oi->tpkt[make_pair(k,r)]->coef_cons[nCons]=1;
				//e.g.oi is o1
				//o1->tpkt[{1,1}]=c_{o1,o1,1}*zvl[{o1}][1]+c_{o1,o2,1}*zvl[o1,o2][1]+c_{o1,o3,1}*zvl[{o1,o3}][1]
				//cout<<oi->name<<" hallo "<<k<<r<<endl;
				for(auto oj:seto)
				{
					set<object*> settmp={oi,oj};
					mapcv[nCons].insert(zvl[settmp][k]);
					zvl[settmp][k]->cons_i.insert(nCons);
					zvl[settmp][k]->coef_cons[nCons]=-1*oi->dtpkt[make_pair(r,oj)];
					//cout<<oj->name<<" yes "<<k<<endl;
				}
				mapcs[nCons]=0;
				mapcc[nCons]=0;
			}
		}
	}
	//cout<<"korrektur"<<endl;
	//oi->bvl[k]>=zvl[{oi,oj}][k]
	//oj->bvl[k]>=zvl[{oi,oj}][k]
	for(auto oi:seto)
	{
		for(auto oj:seto)
		{
			set<object*> settmp={oi,oj};
			for(int k=1;k!=seto.size()+1;++k)
			{
				nCons++;
				mapcv[nCons].insert(oi->bvl[k]);
				oi->bvl[k]->cons_i.insert(nCons);
				oi->bvl[k]->coef_cons[nCons]=1;
				mapcv[nCons].insert(zvl[settmp][k]);
				zvl[settmp][k]->cons_i.insert(nCons);
				zvl[settmp][k]->coef_cons[nCons]=-1;
				mapcs[nCons]=1;
				mapcc[nCons]=0;
				nCons++;
				mapcv[nCons].insert(oj->bvl[k]);
				oj->bvl[k]->cons_i.insert(nCons);
				oj->bvl[k]->coef_cons[nCons]=1;
				mapcv[nCons].insert(zvl[settmp][k]);
				zvl[settmp][k]->cons_i.insert(nCons);
				zvl[settmp][k]->coef_cons[nCons]=-1;
				mapcs[nCons]=1;
				mapcc[nCons]=0;
			}
		}
	}


    //t_{i,m,maxPi} denote the maximum drying time for Pi in group gm
    for(auto o:seto)
    {
	for(int i=1;i!=seto.size()+1;++i)
	{
	    for(int j=0;j!=o->opkt.size();++j)
	    {
		nCons++;
		//t_{Pi,gm,maxPi}>=t_{Pi,gm,r},r=1,_,n_{Pi}
		//o->tpktmax[i]>=o->tpkt[{i,j}]
		mapcv[nCons].insert(o->tpktmax[i]);
		o->tpktmax[i]->cons_i.insert(nCons);
		o->tpktmax[i]->coef_cons[nCons]=1;
		mapcv[nCons].insert(o->tpkt[make_pair(i,j)]);
		o->tpkt[make_pair(i,j)]->cons_i.insert(nCons);
		o->tpkt[make_pair(i,j)]->coef_cons[nCons]=-1;
		mapcs[nCons]=1;
		mapcc[nCons]=0;
	    }
	}
    }
    //t_{m,max m} denote the maximum drying time for group gm
    //deal with class layer var *vmaxdt;
    for(int i=0;i!=vecl.size();++i)
    {
	for(auto o:seto)
	{
	    nCons++;
	    //vecl[i]->vmaxdt>=o->tpktmax[i+1]
	    mapcv[nCons].insert(vecl[i]->vmaxdt);
	    vecl[i]->vmaxdt->cons_i.insert(nCons);
	    vecl[i]->vmaxdt->coef_cons[nCons]=1;
	    mapcv[nCons].insert(o->tpktmax[i+1]);
	    o->tpktmax[i+1]->cons_i.insert(nCons);
	    o->tpktmax[i+1]->coef_cons[nCons]=-1;
	    mapcs[nCons]=1;
	    mapcc[nCons]=0;
	}
    }
    //(4)minimize sum_{gm=1}^Ng t_{gm,maxgm}+Ng
    nCons++;
    objcons=nCons;
    mapcv[objcons].insert(maxL);
    maxL->cons_i.insert(objcons);
    maxL->coef_cons[objcons]=1;
    for(int i=0;i!=vecl.size();++i)
    {
	mapcv[objcons].insert(vecl[i]->vmaxdt);
	vecl[i]->vmaxdt->cons_i.insert(objcons);
	vecl[i]->vmaxdt->coef_cons[objcons]=1;
    }
}
#endif
