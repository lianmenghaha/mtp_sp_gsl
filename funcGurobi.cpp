#ifndef FUNCGUROBI_CPP
#define FUNCGUROBI_CPP
void funcGurobi(int opt,double heu,double time,double absgap,double gap,int idis,int focus,int psol,int psmode)
{
	cout<<"Gurobi start"<<endl;
	GRBVar *x=new GRBVar [vvar.size()];
	//try{
	GRBEnv env = GRBEnv();
	GRBModel model = GRBModel(env);
	model.getEnv().set(GRB_DoubleParam_TimeLimit, time); // e.g. 900
	model.getEnv().set(GRB_DoubleParam_MIPGapAbs, absgap); // e.g. 4020
	model.getEnv().set(GRB_DoubleParam_MIPGap, gap); // e.g. 0.02
	model.getEnv().set(GRB_DoubleParam_Heuristics, heu); // default 0.05
	model.getEnv().set(GRB_IntParam_OutputFlag, idis);
	model.getEnv().set(GRB_IntParam_MIPFocus, focus);
	model.getEnv().set(GRB_IntParam_PoolSolutions,psol);
	model.getEnv().set(GRB_IntParam_PoolSearchMode,psmode);//With a setting of 2, it will find the n best solutions, where n is determined by the value of the PoolSolutions parameter
	//int solnr=0;
	//model.getEnv().set(GRB_IntParam_SolutionNumber,solnr);//this parameter determines which alternate solution is retrieved.



	map<var*,string> mapvs;
	for(int i=0;i<vvar.size();i++)
	{
		ostringstream convi;
		convi<<i;
		mapvs[vvar[i]]='x'+convi.str();
	}

	for(int i=0;i!=vvar.size();i++)
	{
		if(vvar[i]->typ==0)
		  x[i]=model.addVar(0.0,1.0,0.0,GRB_BINARY,mapvs[vvar[i]]);
		else if(vvar[i]->typ==1)
		  x[i]=model.addVar(vvar[i]->lb,vvar[i]->ub,0.0,GRB_INTEGER,mapvs[vvar[i]]);
		else if(vvar[i]->typ==2)
		  x[i]=model.addVar(vvar[i]->lb,vvar[i]->ub,0.0,GRB_CONTINUOUS,mapvs[vvar[i]]);
	}
	//
	model.update();
	for(int i=0;i<=nCons;i++)
	  if(mapcv[i].size()!=0) // cons with 0 var is eliminated
	  {
		  ostringstream convi;
		  convi<<i;
		  if(qcons.find(i)==qcons.end())
		  {
			  GRBLinExpr expr;
			  for(set<var*>::iterator setit=mapcv[i].begin();setit!=mapcv[i].end();setit++)
				expr+=x[(*setit)->index]*((*setit)->coef_cons[i]);
			  //for(auto cv:mapcv[i])
				//expr+=x[cv->index]*(cv->coef_cons[i]);
			  if(i!=objcons)
			  {
				  string name='c'+convi.str();
				  if(mapcs[i]==1)
					model.addConstr(expr,GRB_GREATER_EQUAL,mapcc[i],name);
				  else if(mapcs[i]==-1)
					model.addConstr(expr,GRB_LESS_EQUAL,mapcc[i],name);
				  else if(mapcs[i]==0)
					model.addConstr(expr,GRB_EQUAL,mapcc[i],name);
				  //
				  /*else if(mapcs[i]==2)
				  {
					  if(mapcv[i].size()==2);
					  {
						  var* vartmp1;
						  for(set<var*>::iterator setit=mapcv[i].begin();setit!=mapcv[i].end();setit++)
						  {
							  if((*setit)->lrhs==1)
								vartmp1=*setit;
						  }
						  mapcv[i].erase(vartmp1);
						  set<var*>::iterator setit2=mapcv[i].begin();
						  var* vartmp2=*setit2;
						  GRBVar res=x[vartmp1->index];
						  GRBVar arr[1];
						  arr[0]=x[vartmp2->index];
						  model.addGenConstrAnd(res,arr,1,"andconstr1");
					  }
					  if(mapcv[i].size()==3);
					  {
						  var* vartmp1;
						  for(set<var*>::iterator setit=mapcv[i].begin();setit!=mapcv[i].end();setit++)
						  {
							  if((*setit)->lrhs==1)
								vartmp1=*setit;
						  }
						  mapcv[i].erase(vartmp1);
						  set<var*>::iterator setit2=mapcv[i].begin();
						  mapcv[i].erase(setit2);
						  set<var*>::iterator setit3=mapcv[i].begin();
						  var* vartmp2=*setit2;
						  var* vartmp3=*setit3;
						  GRBVar res=x[vartmp1->index];
						  GRBVar arr[2];
						  arr[0]=x[vartmp2->index];
						  arr[1]=x[vartmp3->index];
						  model.addGenConstrAnd(res,arr,2,"andconstr2");
						  //model.addConstr(x[vartmp1->index]==and_(x[vartmp2->index],x[vartmp3->index]),"andconstr2");
					  }
				  }*/

			  }
			  else
			  {
				  if(opt==0)
					model.setObjective(expr,GRB_MINIMIZE);
				  else
					model.setObjective(expr,GRB_MAXIMIZE);
			  }
		  }
		  else
		  {
			  GRBQuadExpr expr;
			  for(set<var*>::iterator setit=mapcv[i].begin();setit!=mapcv[i].end();setit++)
			  {
				  expr+=((*setit)->coef_cons[i])*x[(*setit)->index];
				  for(set<var*>::iterator setitt=(*setit)->coef_setvar[i].begin();setitt!=(*setit)->coef_setvar[i].end();setitt++)
					expr+=((*setit)->coef_varcons[make_pair(i,*setitt)])*x[(*setit)->index]*x[(*setitt)->index];
			  }
			  if(i!=objcons)
			  {
				  string name='c'+convi.str();
				  if(mapcs[i]==1)
					model.addQConstr(expr,GRB_GREATER_EQUAL,mapcc[i],name);
				  else if(mapcs[i]==-1)
					model.addQConstr(expr,GRB_LESS_EQUAL,mapcc[i],name);
				  else if(mapcs[i]==0)
					model.addQConstr(expr,GRB_EQUAL,mapcc[i],name);
			  }
			  else
			  {
				  if(opt==0)
					model.setObjective(expr,GRB_MINIMIZE);
				  else
					model.setObjective(expr,GRB_MAXIMIZE);
			  }
		  }
	  }

	//	mycallback cb(time, absgap);
	//	model.setCallback(&cb);
	int optite=0;
	model.optimize();
	if(model.get(GRB_IntAttr_Status)==GRB_INFEASIBLE)
	{
		model.getEnv().set(GRB_DoubleParam_TimeLimit, 3600); // e.g. 900
		model.computeIIS();
		model.write("model.ilp");
		exit(1);
	}
	else
	  while(model.get(GRB_IntAttr_SolCount)==0)
	  {
		  time+=5;
		  optite++;
		  model.getEnv().set(GRB_DoubleParam_TimeLimit, time); // e.g. 900
		  model.optimize();
	  }
	//	} catch (GRBException e) {
	//		cout << "Error number: " << e.getErrorCode() << endl;
	//		cout << e.getMessage() << endl;
	//	} catch (...) {
	//		cout << "Error during optimization" << endl;
	//	}
	for(int i=0;i<vvar.size();i++)
	{
		if(vvar[i]->typ==0||vvar[i]->typ==1)
		{
			if(x[i].get(GRB_DoubleAttr_X)-(int)x[i].get(GRB_DoubleAttr_X)<0.5)
			  vvar[i]->intrval=(int)x[i].get(GRB_DoubleAttr_X);
			else
			  vvar[i]->intrval=(int)x[i].get(GRB_DoubleAttr_X)+1;
		}
		else if(vvar[i]->typ==2)
		  vvar[i]->dourval=x[i].get(GRB_DoubleAttr_X);
		else
		  cout<<"new type"<<endl;
	}
	/*
	//store the alternative solution of Gurobi
	for(int i=0;i<vvar.size();i++)
	{
		if(vvar[i]->typ==0||vvar[i]->typ==1)
		{
			if(x[i].get(GRB_DoubleAttr_Xn)-(int)x[i].get(GRB_DoubleAttr_Xn)<0.5)
			  vvar[i]->intrval2=(int)x[i].get(GRB_DoubleAttr_Xn);
			else
			  vvar[i]->intrval2=(int)x[i].get(GRB_DoubleAttr_Xn)+1;
		}
		else if(vvar[i]->typ==2)
		  vvar[i]->dourval2=x[i].get(GRB_DoubleAttr_Xn);
		else
		  cout<<"new type"<<endl;
	}*/
	int solnr;
	for(int k=0;k!=psol;++k)
	{
		solnr=k;
		model.getEnv().set(GRB_IntParam_SolutionNumber,solnr);//this parameter d    etermines which alternate solution is retrieved.
		for(int i=0;i<vvar.size();i++)
		{
			if(vvar[i]->typ==0||vvar[i]->typ==1)
			{
				if(x[i].get(GRB_DoubleAttr_Xn)-(int)x[i].get(GRB_DoubleAttr_Xn)<0.5)
				  (vvar[i]->intrval2).push_back((int)x[i].get(GRB_DoubleAttr_Xn));
				else
				  (vvar[i]->intrval2).push_back((int)x[i].get(GRB_DoubleAttr_Xn)+1);
			}
			else if(vvar[i]->typ==2)
			  (vvar[i]->dourval2).push_back(x[i].get(GRB_DoubleAttr_Xn));
			else
			  cout<<"new type"<<endl;
		}
	}






	cout<<"Gurobi end"<<endl;
}






#endif
