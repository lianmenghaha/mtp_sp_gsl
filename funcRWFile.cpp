#ifndef FUNCREADFILE_CPP
#define FUNCREADFILE_CPP
void funcReadFile(const char* filename_obj,const char *filename_conf)
{
    //read path info of objects,object.txt
    fstream ReadFile_obj(filename_obj,fstream::in);
    string rname,line;
    ReadFile_obj>>rname;
    while(!ReadFile_obj.eof())
    {
	object *o=new object;
	o->name=rname;
	mapnameo[rname]=o;
	seto.insert(o);
	ReadFile_obj>>o->area;
	//read details of path
	while(ReadFile_obj>>rname)
	{
	    if(ReadFile_obj.eof()||rname[rname.size()-1]!=':')
		break;
	    getline(ReadFile_obj,line);
	    auto strpos1=1;
	    auto strpos2=line.find_first_of("+");
	    auto strpos3=line.find_first_of("i");
	    auto strpos4=line.find_last_of("+");
	    double x1,x2,y1,y2;
	    x1=atof(line.substr(strpos1,strpos2-strpos1).c_str());
	    y1=atof(line.substr(strpos2+1,strpos3-strpos2-1).c_str());
	    x2=atof(line.substr(strpos3+2,strpos4-strpos3-2).c_str());
	    y2=atof(line.substr(strpos4+1,line.size()-strpos4-2).c_str());
	    o->path.push_back({x1,y1});
	    o->path.push_back({x2,y2});
	}
    }
    ReadFile_obj.close();
    //read conflict info of objects
    fstream ReadFile_conf(filename_conf,fstream::in);
    while(getline(ReadFile_conf,line))
    {
	//jiance
	//cout<<"conf"<<endl;
	if(ReadFile_conf.eof())
	    break;
	if(line.compare("no merge conflict:")==0)
	{
	    //jiance
	    //cout<<"yidu"<<endl;
	    while(ReadFile_conf>>rname)
	    {
		if(rname.compare("FIN")==0)
		    break;
		object *o=mapnameo[rname];
		//cout<<rname<<" ";
		int nobj;
		ReadFile_conf>>nobj;
		//cout<<nobj<<" ";
		for(int i=0;i!=nobj;++i)
		{
		    ReadFile_conf>>rname;
		    o->setco.insert(mapnameo[rname]);
		    //jiance
		    //cout<<rname<<endl;
		}
	    }
	}
	if(line.compare("no absorption conflict:")==0)
	{
	    //jiance
	    //cout<<"yidu2"<<endl;
	    while(ReadFile_conf>>rname)
	    {
		if(rname.compare("FIN")==0)
		    break;
		object *o=mapnameo[rname];
		int nobj;
		ReadFile_conf>>nobj;
		for(int i=0;i!=nobj;++i)
		{
		    ReadFile_conf>>rname;
		    o->setbo.insert(mapnameo[rname]);
		    //jiance
		   // cout<<"no"<<endl;
		}
	    }
	}
    }
    ReadFile_conf.close();
}
void funcJiance()
{
	cout<<"Jiance setco"<<endl;
	for(auto o:seto)
	{
		cout<<o->name<<" :";
		for(auto on:o->setco)
		  cout<<on->name<<" ";
		cout<<endl;
	}
	cout<<"Jiance setbo"<<endl;
	for(auto o:seto)
	{
		cout<<o->name<<" :";
		for(auto on:o->setbo)
		  cout<<on->name<<" ";
		cout<<endl;
	}
}


void funcReadFile(const char* filename_dtime)
{
    fstream ReadFile_dtime(filename_dtime,fstream::in);
    string rname;
    while(!ReadFile_dtime.eof())
    {
	//jiance
	//cout<<"now"<<endl;
	ReadFile_dtime>>rname;
	if(rname.compare("END")==0)
	    break;
	object *oi=mapnameo[rname];
	//jiance
	//cout<<"oi"<<endl;
	//cout<<oi->opkt.size()<<endl;
	ReadFile_dtime>>rname;
	object *oj=mapnameo[rname];
	//jiance
	//cout<<"oj"<<endl;
	int rnbio=pow(oi->nbio+1,2);
	for(int i=0;i!=rnbio;++i)
	{
	    int itmp;
	    //jiance
	    //cout<<"itmp"<<endl;
	    ReadFile_dtime>>itmp;
	    ReadFile_dtime>>oi->dtpkt[make_pair(itmp,oj)];
	    //jiance
	    //cout<<oi->dtpkt[{itmp,oj}]<<endl;
	}
    }
    ReadFile_dtime.close();
}


void funcWriteFile(const char* filename,int psol,const char* filenamesub)
{
    for(int i=1;i!=maxL->intrval+1;++i)
    {
		ostringstream convi;
		convi<<i;
		string strtmp=filename;
		strtmp+="_objassign"+convi.str()+".txt";
		fstream WriteFile_oa(strtmp.c_str(),fstream::out);
		//int itmp=0;
		set<object*> sotmp;
		for(auto o:seto)
		{
			/*set<object*> settmp={o};
			WriteFile_oa<<i<<" "<<o->bvl[i]->intrval<<" "<<zvl[settmp][i]->intrval<<endl;*/
			if(o->vl->intrval==i)
			{
				//++itmp;
				WriteFile_oa<<o->name<<endl;
				sotmp.insert(o);
				//for(auto oj:seto)
				//{
				//	WriteFile_oa<<o->name<<" "<<oj->name<<endl;
					//for(int j=0;j!=o->opkt.size();++j)
					  //WriteFile_oa<<j<<" "<<o->dtpkt[{j,oj}]<<endl;
				//}
			}
		
			/*for(auto oj:seto)
			{set<object*> settmp2={o,oj};
			  WriteFile_oa<<o->name<<" "<<oj->name<<" "<<i<<" "<<zvl[settmp2][i]->intrval<<" ? "<<o->bvl[i]->intrval<<" ? "<<oj->bvl[i]->intrval<<endl;
			}*/
		}
		WriteFile_oa.setf(ios_base::fixed,ios_base::floatfield);
		WriteFile_oa.precision(10);
		WriteFile_oa<<"The Number of Objects: "<<sotmp.size()<<endl;
		WriteFile_oa<<"The Drying Time for this Layer is: "<<vecl[i-1]->vmaxdt->dourval<<endl;
		//WriteFile_oa<<maxL->intrval;
		//the Drying time which computing with Newton
		for(auto c:setcmb)
		{
		    if((includes(c->setp.begin(),c->setp.end(),sotmp.begin(),sotmp.end()))&&(includes(sotmp.begin(),sotmp.end(),c->setp.begin(),c->setp.end())))
			WriteFile_oa<<"The Drying Time for this Layer which computing with Newton Method is: "<<c->maxdt<<endl;
			break;
		}
		WriteFile_oa.close();
	}
	//write the information about sub-optimal solution
	for(int i=0;i!=psol;++i)
	{
		int itmp=i;
		ostringstream convi;
		convi<<itmp;
		string strtmp=filenamesub;
		strtmp+="_subop"+convi.str()+".txt";
		fstream WriteFile_sub(strtmp.c_str(),fstream::out);
		WriteFile_sub<<"The "<<itmp<<"-th sub-optimal Solution"<<endl;
		double dtmp=0;
		double dtmp2=0;
		for(int k=1;k!=maxL->intrval2[i]+1;++k)
		{
		    set<object*> sotmp;
		    WriteFile_sub<<"Printing Group "<<k<<" :"<<endl;
		    for(auto o:seto)
		    {
			if(o->vl->intrval2[i]==k)
			{
			    WriteFile_sub<<o->name<<" ";
			    sotmp.insert(o);
			}
		    }
			WriteFile_sub<<endl;
			WriteFile_sub.setf(ios_base::fixed,ios_base::floatfield);
			WriteFile_sub.precision(10);
			WriteFile_sub<<"The Drying Time for this Printing Group: "<<vecl[k-1]->vmaxdt->dourval2[i]<<endl;
			dtmp+=vecl[k-1]->vmaxdt->dourval2[i];
			//the Drying time which computing with Newton
			for(auto c:setcmb)
			{
			    if((includes(c->setp.begin(),c->setp.end(),sotmp.begin(),sotmp.end()))&&(includes(sotmp.begin(),sotmp.end(),c->setp.begin(),c->setp.end())))
			    {
				WriteFile_sub<<"The Drying Time for this Printing Group which computing with Newton Method is: "<<c->maxdt<<endl;
				dtmp2+=c->maxdt;
				break;
			    }
			}
		}
		WriteFile_sub<<endl;
		WriteFile_sub<<"Total Printing Group: "<<maxL->intrval2[i]<<endl;
		WriteFile_sub<<"Total Sum of Drying Time is: "<<dtmp<<endl;
		WriteFile_sub<<"Total Sum of Drying Time with Newton Method is: "<<dtmp2<<endl;
		WriteFile_sub.close();
	}








}
#endif

