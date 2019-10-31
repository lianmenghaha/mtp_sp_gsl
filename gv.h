void funcReadFile(const char*, const char*);
void funcPrepare();
void funcModel_Var();
void funcModel_Cons();
void funcWriteFile(const char*);
void funcGurobi(int, double, double, double, double, int, int,int);
void funcConsClear();//
void funcScreenMessage();
void funcPre_nt(int);
void funcNewton();
// ReadFile
	// ReadFile_obj
	set<object*> seto;
	map<string, object*> mapnameo;
	// ReadFile_conf
	// ReadFile_dtime
// funcPrepare
//funcNewton
	set<combi*> setcmb;
// funcModel_Var
var *maxL;
vector<layer*> vecl;
//zvl
map<set<object*>,var**> zvl;
// Model
int nCons, objcons;
map<int,int> mapcs; // use cons index to map sense: 1=g, 0=e, -1=l
map<int,double> mapcc; // use cons index to map right side constant
map<int,set<var*>> mapcv; // use cons index to map corresponding vars
vector<var*> vvar;
set<int> qcons;
