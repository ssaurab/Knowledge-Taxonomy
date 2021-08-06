using namespace std;

/**************** declarations *******************/

set<string> findAllIndividuals(map<string,vector<string> > &dag,
								map<string,vector<string> > &msc,
								map<string,bool> visited,
								string concept);
set<string> findAllConcepts(map<string,vector<string> > &revDag,
								map<string,vector<string> > &msc,
								map<string,bool> visited,
								string individual);

void addComplexFact(map<string,vector<string> > &dag,
					map<string,vector<string> > &revDag,
					map<string,vector<string> > &msc,
					map<string,bool> visited,
					string individual,
					string conceptdefn, map<string,string> &cds);
// Utils
void printing(map<string,vector<string> > &ma);
void addVectorToSet(vector<string> &vec,set<string> &s);
map<string,vector<string> > remakeRevDag(map<string,vector<string> > &dag,
											map<string,bool> visited);


/************* function definitions ***************/

int StructMatching(string cd1, string cd2)
{
	string FinalCommand = "python -c \"from SM import fun; fun(" + cd1 + ", "+ cd2 +")\"";
    //cout << FinalCommand;
	int result = system(&FinalCommand[0]);
	if(result==1792)
	{
		return 0;
	}
	if(result == 2048)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


void addSimpleFact(map<string,vector<string> > &dag,
					map<string,vector<string> > &revDag,
					map<string,vector<string> > &msc,
					map<string,bool> visited,
					string individual,
					string concept){

	map<string,bool> visited1=visited,visited2=visited;
	queue<string> que1,que2;
	
	// traverse down the tree and check individual already exists
	que1.push(concept);
	visited1[concept]=true;
	while(!que1.empty()){
		string con = que1.front();
		que1.pop();
		vector<string> vec = msc[con];
		vector<string> ::iterator it = find(vec.begin(),vec.end(),individual); // check if indi already exists..
		if(it!=vec.end()){	// found the individual in more specific concept, so nothing..
			return;
		}
		vec = dag[con];
		for(int i=0;i<vec.size();i++){
			if(!visited1[vec[i]]){
				que1.push(vec[i]);	
				visited1[vec[i]]=true;
			}
		}
	}
	// now erase indi from parents by traversing up the tree..
	que2.push(concept);
	visited2[concept]=true;
	while(!que2.empty()){
		string con = que2.front();
		que2.pop();
		vector<string> ::iterator it = find(msc[con].begin(),msc[con].end(),individual); // check if indi already exists..
		if(it!=msc[con].end()){	// found the individual in more specific concept, so nothing..
			msc[con].erase(it);
		}
		vector<string> vec =revDag[con];
		for(int i=0;i<vec.size();i++){
			if(!visited2[vec[i]]){
				que2.push(vec[i]);	
				visited2[vec[i]]=true;
			}
		}
	}
	// add indi to the concept..
	msc[concept].push_back(individual);
}

void addComplexFact(map<string,vector<string> > &dag,
					map<string,vector<string> > &revDag,
					map<string,vector<string> > &msc,
					map<string,bool> visited,
					string individual,
					string conceptdefn, map<string,string> &cds){

	map<string,bool> visited1=visited,visited2=visited;
	queue<string> que1,que2;
	
	//traverse down the tree from top and check individual already exists
	que1.push("Thing");
	visited1["Thing"]=true;
	while(!que1.empty()){//removing all traces of the individual from the taxonomy
		string con = que1.front();
		que1.pop();
		vector<string> vec = msc[con];
		vector<string> ::iterator it = find(vec.begin(),vec.end(),individual); 
		if(it!=vec.end()){	// found the individual somewhere in the taxonomy, so remove it first
			vec.erase(it);
			msc[con] = vec;
		}
		vector<string>vec_child = dag[con];
		for(int i=0;i<vec_child.size();i++){
			if(!visited1[vec_child[i]]){
				que1.push(vec_child[i]);	
				visited1[vec_child[i]]=true;
			}
		}
	}
	//cout << "Removed the traces" << endl;
	//now, based on SM procedure, add the individual to appropriate mostspecific parents
	que2.push("Thing");
	visited2["Thing"]=true;
	vector <string> finalparents;
	while(!que2.empty()){
		string con = que2.front();
		que2.pop();
		int flag = 1;
		//cout << dag[con].size() << "is the size" << endl;
		vector<string> ::iterator it = dag[con].begin();
		while(it!=dag[con].end()){
			//cout << "Calling SM on" << "'"+cds[*it]+"'"<<" "<<"'"+conceptdefn+"'"<<endl;
			if(StructMatching("'"+cds[*it]+"'","'"+conceptdefn+"'"))
			{
				//cout << "MatchFound" << endl;
				flag*=0;
				if(!visited2[*it])
				{
					que2.push(*it);
					visited2[*it] = true;
				}
			}	
			it++;
		}
		if(flag == 1)
		{
			finalparents.push_back(con);
		}
	}
	//adding individual to all most specific parents
	for(vector<string>::iterator it=finalparents.begin();it!=finalparents.end();it++)
	{
		msc[*it].push_back(individual);
	}	
	return;
}

set<string> findAllIndividuals(map<string,vector<string> > &dag,
								map<string,vector<string> > &msc,
								map<string,bool> visited,
								string concept){
	set<string> result;
	queue<string> que;
	que.push(concept);
	visited[concept]=true;
	while(!que.empty()){
		string con = que.front();
		que.pop();
		addVectorToSet(msc[con],result);
		vector<string> vec = dag[con];
		for(int i=0;i<vec.size();i++){
			if(!visited[vec[i]]){
				que.push(vec[i]);	
				visited[vec[i]]=true;
			}
		}
	}
	return result;
}

set<string> findAllConcepts(map<string,vector<string> > &revDag,
								map<string,vector<string> > &msc,
								map<string,bool> visited,
								string individual){
	// find the most specific concept..
	string concept;
	queue<string> que;

	for(map<string,vector<string> >::iterator it=msc.begin();
												it!=msc.end();
												it++){
		vector<string>::iterator itt=find(it->second.begin(),it->second.end(),individual);
		if(itt!=it->second.end()){
			que.push(it->first);
			visited[it->first]=true;
		}
	}

	// find all the parents of the above concepts
	set<string> result;
	while(!que.empty()){
		string con = que.front();
		que.pop();
		result.insert(con);
		vector<string> vec = revDag[con];
		for(int i=0;i<vec.size();i++){
			if(!visited[vec[i]]){
				que.push(vec[i]);	
				visited[vec[i]]=true;
			}		
		}
		visited[con]=true;
	}
	return result;
}




///////////// Utility functions are below..
void printing(map<string,vector<string> > &ma){
	for(map<string,vector<string> >::iterator it=ma.begin();
												it!=ma.end();
												it++){
		cout << it->first << "-> " ;
		for(int i=0;i<it->second.size();i++){
			cout << it->second[i] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void addVectorToSet(vector<string> &vec,set<string> &s){
	for(int i=0;i<vec.size();i++){
		s.insert(vec[i]);
	}
}

map<string,vector<string> > remakeRevDag(map<string,vector<string> > &dag,
											map<string,bool> visited){
	map<string,vector<string> > revDag;
	for(map<string,vector<string> >::iterator it=dag.begin();it!=dag.end();it++){
		revDag.insert(make_pair(it->first,vector<string>()));
	}

	queue<string> que;
	que.push("Thing");
	visited["Thing"]=true;
	while(!que.empty()){
		string con = que.front();
		que.pop();
		vector<string> vec = dag[con];
		for(int i=0;i<vec.size();i++){
			revDag[vec[i]].push_back(con);
			if(!visited[vec[i]]){
				que.push(vec[i]);	
				visited[vec[i]]=true;
			}
		}
	}
	return revDag;
}

