#include<bits/stdc++.h>
#include <fstream>
#include "y.tab.h"
#include "normalise.h"
#include "functions.h"
using namespace std;

int my_scan_string(const char *s);
void my_cleanup(void);
int yyparse();
void traverse(Node*,int);
void traverseStringPrint(Node*,int,string &);
void normalise(Node*);
bool fill_allPropagate(map<string,Node*>&);
bool exists_propagate(map<string,Node*>&);
extern Node* rootPtr;

Node* parseTree(string concept){
    rootPtr=NULL;
    if (my_scan_string(concept.c_str()) != 0) {
        fputs("error setting up an internal buffer\n", stderr);
        exit(1);
    }
    yyparse();
    my_cleanup();
    return rootPtr;
}
int main2() {

    Node* root=parseTree("[AND [AND Staff [ALL :Degree Nursing][FILLS :WorksAt medicaldept][EXISTS 1 :Expertise][FILLS :Experience decade]][AND [EXITS 1 :Married][ALL :Married [AND [ALL :Loves [AND Staff [FILLS :WorksAt admindept][FILLS :WorksAt tdept][FILLS :Experience decade]]][FILLS :Loves adam]]]]]");
    traverse(root,1);
    normalise(root);
    traverse(root,1);
    return 0;
}

bool func(pair<string,string> a,pair<string,string> b){
    if(a.first.length()>b.first.length())
        return true;
    return false;
}

string expandTerms(string str,map<string,string> &cds){
    //cout << " start expand" << endl << flush;
    cout << str << endl;

    vector<pair<string,string> > cdsvec;
    for(map<string,string>::iterator it=cds.begin();it!=cds.end();it++){
        cdsvec.push_back(make_pair(it->first,it->second));
    }
    sort(cdsvec.begin(),cdsvec.end(),func);

    for(int i=0;i<cdsvec.size();i++){
        
        string concept = cdsvec[i].first;
        //cout << "going if" << endl << flush;
        size_t index = str.find(concept);
        if (index != string::npos) {
            //cout << "inside if" << endl << flush;
            //cout << concept << "-----" << str << endl << flush;
            str.erase(index,concept.length());
            //cout << str << endl << flush;
            string def=cdsvec[i].second;
            str.insert(index,def);
            //cout << "expand complete" << str << endl << flush;
        }
    }
    cout << str << endl << endl;
    return str;
}   


void handleFacts(string filename,map<string,vector<string> >&factsall,
    map<string,string> &factsnrml,
    map<string,Node*>&factstree,
    map<string,string>&factsPropagated,
    map<string,string>&cds)
{
	factsPropagated.clear();
	factsnrml.clear();
	factsall.clear();
	factstree.clear();
	 ifstream facts(filename);
	 string line;
	 while(getline(facts,line)){
        string individual,concept;
        istringstream stream(line);
        getline(stream,individual,'-');
        getline(stream,concept);
        concept.erase(0,1);
        
        map<string,vector<string> > :: iterator it;
        it = factsall.find(individual);
        //factsall = just facts.txt in dict form
        if(it == factsall.end())
        {
        	vector <string> ini;
        	ini.push_back(concept);
        	factsall.insert(make_pair(individual,ini));
        }
        else
        {
        	(it->second).push_back(concept);
        }
        
    }
   //cout << "msc " << endl;
   // printing(msc);
    facts.close();
    //factsnrml

    for(map<string,vector<string> >::iterator it=factsall.begin();it!=factsall.end();it++){
        for(vector<string>::iterator itt=it->second.begin();itt!=it->second.end();itt++){
            *itt=expandTerms(*itt,cds);
        }
    }
    cout << "expantion complete" << endl;



    map<string,vector<string> > :: iterator itfa;
    itfa = factsall.begin();
   // cout << endl;
    while(itfa!=factsall.end())
    {	string s;
    	// cout << "inserting" << itfa->first<< endl;
    	string normalised = "";
    	if((itfa->second).size()>1)
    	{
    		s = "[AND ";
        	for(int i=0; i<(itfa->second).size();i++)
        	{
        		s+=(itfa->second)[i];
        	}
        	s+="]";
            cout << "--"<<s<<"--" << endl<<flush;
	    	Node* root = parseTree(s);
	    	normalise(root);
	    	traverseStringPrint(root,0,normalised);
    	}
    	else
    	{
    		s = (itfa->second)[0];
    		normalised = s;
    		//cout << "--"<<s<<"--"<< endl;
    	}
    	//cout << "inserting" << itfa->first << " "<< normalised<< endl;
    	factsnrml.insert(make_pair(itfa->first,normalised));
    	itfa++;
    }
    //checking facts single: FAILED due to SEG Fault
    map<string, string> :: iterator itfs;
    for(itfs=factsnrml.begin();itfs!=factsnrml.end();itfs++)
    {
    	cout << itfs->first << " "<< itfs->second << endl<<flush;
    }

   	//filling factstree
   	for(itfs=factsnrml.begin();itfs!=factsnrml.end();itfs++)
   	{
   		factstree.insert(make_pair(itfs->first,parseTree(itfs->second)));
   	}
   	//cout << "entering fill_allPropagate"<<endl;
   	bool delta = true;
	while(delta)
	{
	    delta = fill_allPropagate(factstree);
	    if(exists_propagate(factstree)==true)
	    {
	    	delta = true;
	    }
	    //cout<<"Propagations while"<<endl;
	}
		//cout << "handled facts "<<endl<<flush;
   	map<string,Node*>::iterator ift;
   	for(ift=factstree.begin();ift!=factstree.end();ift++)
   	{
   		string cdstr="";
   		traverseStringPrint(ift->second,0,cdstr);
   		// cout << "--"<<cdstr<<"--"<<endl;
   		factsPropagated.insert(make_pair(ift->first,cdstr));
   	}      

    // propagate
    cout << "Propagations" << endl;
    for(map<string,string>::iterator it=factsPropagated.begin();it!=factsPropagated.end();it++){
        cout << it->first << " " << it->second << endl;
    }
}

int main(int argc,char* argv[]){
    // Input file streams
    ifstream tax(argv[1]);      // taxonomy input file
    ifstream facts(argv[2]);    // facts input file
    ifstream cdefns(argv[3]);

    // Initializing required DS
    map<string,vector<string> > msc;    // key-most specific concept,  value- list of individuals
    map<string,vector<string> > dag;    // key subsumes all the concepts in the vector
    map<string,vector<string> > revDag; // handy for traversing up the tree
    map<string,bool> visited;
    map<string, string> cds; //map that has concept v/s its definition for each derived concept
    vector<string> KBconcepts;
    map<string,vector<string> > factsall;
    map<string,string> factsnrml;
    map<string,Node*>factstree;
    map<string,string>factsPropagated;
    // Reading & Processing input
    string line;

    while(getline(tax,line)){
        string result;
        istringstream stream(line);
        getline(stream,result,',');
        if(result!=line){
        //  cout << "2" << endl;
            string parent = result;
            getline(stream,result);
            string child = result;
            dag[parent].push_back(child);
            revDag[child].push_back(parent);
            continue;
        }
        else{
            KBconcepts.push_back(line);
        }   
        //cout << "1" << endl;
        //msc.insert(make_pair(result,vector<string>()));
        dag.insert(make_pair(result,vector<string>()));
        revDag.insert(make_pair(result,vector<string>()));
        visited.insert(make_pair(result,false));
    }
    tax.close();

    while(getline(cdefns,line)){
        string concept, conceptDefn;
        istringstream stream(line);
        getline(stream, concept, '=');
        getline(stream, conceptDefn);
        //cout << "hi->" << conceptDefn << endl;
        // concept.erase(0,1);
        concept.erase(remove_if(concept.begin(), concept.end(), ::isspace), concept.end());
        if(conceptDefn[0]==' ')
            conceptDefn.erase(0,1);
        cds[concept] = conceptDefn;
    }
    //adding cds for primitive concepts as themselves
    for(vector<string>::iterator it=KBconcepts.begin();it!=KBconcepts.end();it++)
    {
        map<string, string>::iterator itcd=cds.find(*it);
        if(itcd==cds.end())
        {
            cds.insert(make_pair(*it,*it));
        }
    }
     
    cdefns.close();
    //cout << "concept definitons" << endl;
    map<string, string>:: iterator it;
    for(it = cds.begin(); it!=cds.end(); it++)
    {
        //cout << it->first << " " << it->second << endl;
    }
   

   	
	handleFacts(argv[2],factsall,factsnrml,factstree,factsPropagated,cds);
	//comment the below line later
	//factsPropagated = factsnrml;
   	map<string,string>::iterator ifp;
   	for(ifp=factsPropagated.begin();ifp!=factsPropagated.end();ifp++)
    {
        if ((ifp->second).find('[') != string::npos)
        {
            addComplexFact(dag,revDag,msc,visited,ifp->first,ifp->second,cds);
        }
        else
        {
            addSimpleFact(dag,revDag,msc,visited,ifp->first,ifp->second);
        }
	}
    printing(msc);
    // if you didnt make revdag directly from input, uncomment below line to construct it..
    //revDag = remakeRevDag(dag,visited); // construct reverse dag

    //printing(msc);
    //printing(dag);

    /* --------------------------------------------------------------------------*/
    // Start querying...
    cout << "Hello" << endl;
    string command= "Start";
    while(command!="End")
    {
        cout << endl;
        cout << "Please enter your command"<<endl;
        cout << "1) Type 'View' to view the Taxonomy"<<endl;
        cout << "2) Type 'Query' to make a query"<<endl;
        cout << "3) Type 'Add' to add a fact to the Knowledge Base"<<endl;
        cout << "4) Type 'End' to exit the program" << endl;
        cin >> command;
        if(command != "View" && command != "Query" && command != "Add" && command != "End")
        {
            cout << "Invalid Command"<<endl;
        }
        else
        {
            if(command == "View")
            {
                system("g++ DAGInitialize.cpp -o dagbuild");
                system("./dagbuild");
                system("python3 DAG.py");
            }
            else if(command == "Query")
            {
                cout << endl;
                cout << "Three kinds of queries can be made. Select one."<<endl;
                cout << "1) Type 'Individual Individual_Name' to fetch all the concepts the individual belongs to."<<endl;
                cout << "2) Type 'Concept Concept_Name' to fetch all the associated individuals" << endl;
                cout << "3) Type 'Check Individual_Name Concept_Name' to check if the given individual is an instance of the given concept."<<endl;
                cout << "4) Type 'Back' to go back to the previous menu" << endl;
                string querycommand;
                cin >> querycommand;
                if(querycommand == "Back" || querycommand == "back")
                {
                    continue;
                }
                else if(querycommand == "Individual" || querycommand == "individual")
                {
                    string individual;
                    cin >> individual;
                    set<string> concepts = findAllConcepts(revDag,msc,visited,individual);
                    for(set<string>::iterator it=concepts.begin();it!=concepts.end();it++)
                        cout << *it << " ";
                    cout << endl;
                }
                else if(querycommand == "Concept" || querycommand == "concept")
                {
                    string concept;
                    cin >> concept;
                    if(msc.find(concept)==msc.end())
                    {
                    	cout << "Invalid Concept"<<endl;
                    }
                    else{
	                    set<string> individuals = findAllIndividuals(dag,msc,visited,concept);
	                    for(set<string>::iterator it=individuals.begin();it!=individuals.end();it++)
	                        cout << *it << " ";
	                    cout << endl;
               		}
                }
                else if(querycommand == "Check" || querycommand == "check")
                {
                    string line;
                    cin >> line;
                    istringstream stream(line);
                    string individual,concept;
                    getline(stream,individual,'-');
                    getline(stream,concept);
                    concept.erase(0,1);
                    
                    set<string> concepts = findAllConcepts(revDag,msc,visited,individual);
                    set<string>::iterator it = find(concepts.begin(),concepts.end(),concept);
                    if(it!=concepts.end())
                        cout << "Yes" << endl;
                    else
                        cout << "No" << endl;
                }
                else
                {
                    cout << "Invalid query." << endl;
                }
                // printing(msc);
            }
            else if(command == "Add")
            {
                //add simple/cpmplex fact
                cout << "Add a fact in the format a->c. 'c' could be a complex but must be normalised." << endl;
                string line;
                getline(cin>>ws,line);
                // cout << line;
                istringstream stream(line);
                string individual,concept;
                getline(stream,individual,'-');
                getline(stream,concept, '>');
                getline(stream,concept, '\n');
                // check if concept is valid..then or new concept will get created..
                cout << individual << endl;
                cout << concept << endl;
                ofstream outfile;
                outfile.open(argv[2],ios::app);
                outfile<<individual;
                outfile<<"->";
                outfile<<concept<<endl;
                handleFacts(argv[2],factsall,factsnrml,factstree,factsPropagated,cds);
                //comment the below line later
                //factsPropagated = factsnrml;
			   	map<string,string>::iterator ifp;
			   	for(ifp=factsPropagated.begin();ifp!=factsPropagated.end();ifp++)
			    {
			        if ((ifp->second).find('[') != string::npos)
			        {
			            addComplexFact(dag,revDag,msc,visited,ifp->first,ifp->second,cds);
			        }
			        else
			        {
			            addSimpleFact(dag,revDag,msc,visited,ifp->first,ifp->second);
			        }
				}
				printing(msc);
            }
        }
    }

    return 0;
}




