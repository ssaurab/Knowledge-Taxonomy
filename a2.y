%token AND FILLS EXISTS ALL CONSTANT NAME INTEGER           
%start program
%{
	#include<bits/stdc++.h>
    #include "normalise.h"
    using namespace std;
    extern void yyerror(string s);
	extern int yylex(void);
    
    Node* rootPtr;
	string name;    // used these variables in the grammar
    string constant;
    int number;
    #define YYSTYPE struct Node*
    #define MORECONCEPTS "moreconcepts"

	void makeListConcepts(Node* root,vector<Node*> &vec);

    
%}

%%

program:    
    concept
    {
        rootPtr=$1;
    }
;

concepts:
    concept concept
    {
        $$ = new Node();
        $$->type="concepts";
        $$->children.push_back($1);
        $$->children.push_back($2);
    }
|   concept concepts
    {
        $$ = new Node();
        $$->type=MORECONCEPTS;
        $$->children.push_back($1);
        $$->children.push_back($2);
    }
;

concept:
    NAME
    {
        $$ = new Node();
        $$->type="concept";
        $$->name=name;
    }
|   '[' NAME ']'
    {
        $$ = new Node();
        $$->type="concept";
        $$->name=name;
    }
|   '[' FILLS role CONSTANT ']'
    {
        $$ = new Node();
        $$->type="fills";
        $$->name=name;
        $$->constant=constant;
    }
|   '[' ALL role concept ']'
    {
        $$ = new Node();
        $$->type="all";
        $$->name=$3->name;
        $$->children.push_back($4);
    }
|   '[' EXISTS INTEGER role ']'
    {
        $$ = new Node();
        $$->type="exists";
        $$->name=name;
        $$->number=number;
    }   
|   '[' AND concepts ']'
    {
        $$ = new Node();
        $$->type="and";
        $$->name="Concepts";
        vector<Node*> vec;
        makeListConcepts($3,vec);
        $$->children=vec;
    }
;

role:
    ':' NAME
    {
        $$ = new Node();
        $$->type="role";
        $$->name=name;
    }
;



%%

/*int main() {
    
    char* str = "[AND Julie Joe]";
    yy_scan_string(str);
    yyparse();
    yylex_destroy();
    traverse(rootPtr,1);
    return 1;
}*/

void makeListConcepts(Node* root,vector<Node*> &vec){
    vec.push_back(root->children[0]);
    if(root->type==MORECONCEPTS){
        makeListConcepts(root->children[1],vec);
    }else{
        vec.push_back(root->children[1]);
    }    
}



void normalise(Node* root);     // main function - this function calls below 3
void flattenAnd(Node* root);    // flattening and arguments 
void combineAll(Node* root);    
void combineExists(Node* root);

void traverse(Node* root,int n); // for printing the parse tree
void traverseStringPrint(Node* root,int n, string&s);

// Dont bother, Util fucntions
Node* combineConcepts(vector<Node*> vec);
void someDamnFunction(Node* root,vector<Node*> &temp);
void shrinkNode(Node* root);
// these are comparators
bool comp(Node* a,Node* b);
bool removeDuplicates(Node* a,Node* b);
bool sortChildren(Node* a,Node* b);

//Propagation related functions
bool fill_allPropagate(map<string,Node*>&factstree);
int StructMatching(string cd1, string cd2);
bool exists_propagate(map<string,Node*>&factstree);
Node* parseTree(string concept);
void printfacts(map<string,Node*>&factstree);
/*********************** definitions *****************************/
// careful it's clumsy down there..

bool fill_allPropagate(map<string,Node*>&factstree)
{
    int changeFlag = 1;
    map<string,Node*>::iterator it1;
    map<string,Node*>::iterator it2;
    vector<Node*>::iterator itchild;
    vector<Node*>::iterator itchild2;
    bool realFactChange = false;
    while(changeFlag)
    {
        changeFlag = 0;
        for(it1=factstree.begin();it1!=factstree.end();it1++)
        {
            for(itchild=((it1->second)->children).begin();itchild!=((it1->second)->children).end();itchild++)
            {
                if((*itchild)->type == "fills")
                {
                    for(itchild2=((it1->second)->children).begin();itchild2!=((it1->second)->children).end();itchild2++)
                    {
                        if((*itchild2)->type == "all" && (*itchild2)->name == (*itchild)->name)
                        {
                            int indiPresentFlag = 0;
                            for(it2=factstree.begin();it2!=factstree.end();it2++)
                            {
                                ////cout << "Entered"<< endl;
                                if(it2->first==(*itchild)->constant)
                                {
                                    indiPresentFlag = 1;
                                    string sminp="";
                                    // cout<<((*itchild2)->children)[0]->
                                    traverseStringPrint(((*itchild2)->children)[0],0,sminp);
                                    // cout<<sminp;
                                    string sminp2="";
                                    traverseStringPrint(it2->second,0,sminp2);
                                    if(StructMatching("'"+sminp+"'","'"+sminp2+"'"))
                                    {
                                        //nothing
                                        //cout<<"meh"<<endl;
                                    }
                                    else
                                    {
                                        realFactChange = true;
                                        //cout << "got in" <<endl;
                                        changeFlag = 1;
                                        Node* replace = new Node();
                                        replace->type = "and";
                                        replace->name = "concepts";
                                        replace->children.push_back(((*itchild2)->children)[0]);
                                        replace->children.push_back(it2->second);
                                        normalise(replace);
                                        factstree[(it2->first)] = replace;
                                        //cout<<"done with normalize"<<endl;
                                    }
                                }
                                //cout << "Exited"<< endl;
                            }
                            if(indiPresentFlag == 0)
                            {
                                realFactChange = true;
                                //cout << "encountered Joe" << endl;
                                //cout << "hi" << endl<<flush;
                                changeFlag = 1;
                                printfacts(factstree);
                                string nps = "";
                                traverse((*itchild2)->children[0],0);
                                traverseStringPrint(((*itchild2)->children)[0],0,nps);
                                Node* newparse = parseTree(nps);
                                //traverse(newparse,0);
                                //cout<<"prop"<<nps;
                                factstree.insert(make_pair((*itchild)->constant,newparse));
                                //printfacts(factstree);
                                //cout<<"successfully added a new individual" <<endl;
                            }
                        }
                    }
                }
            } 
        }
    }
    //cout << "Exited fill-all"<< endl;
    return realFactChange;
}
bool rolechaincomp(pair<string,vector<string> >p1,string newrole,pair<string,vector<string> >p2)
{
    
    if(p1.first != p2.first)
    {
        return false;
    }
    else
    {
        if((p1.second).size()+1!=(p2.second).size())
        {
            return false;
        }
        else
        {
            for(int i=0;i<(p1.second).size();i++)
            {
                if((p1.second)[i]!=(p2.second)[i])
                {
                    return false;
                }
            }
        }
        if(p2.second[(p1.second).size()]!=newrole)
        {
            return false;
        }
    }
    // cout<<"exited rolechaincomp"<<endl<<flush;
    return true;
}
void pstprinter(map<pair<string,vector<string> >,Node*> pseudotree)
{
    map<pair<string,vector<string> >,Node*>::iterator it;
    for(it=pseudotree.begin();it!=pseudotree.end();it++)
    {
        cout<<(it->first).first << " ";
        for(int i=0;i<((it->first).second).size();i++)
        {
            cout<<((it->first).second)[i]<<" ";
        }

        cout <<endl<<flush;
    }
}
void printfacts(map<string,Node*>&factstree)
{
    for(auto it=factstree.begin();it!=factstree.end();it++)
    {
       cout << it->first << " " << (it->second)->type<< endl;
    }
}
bool realFactchanger(Node*inp,map<string,Node*>&factstree)
{
    //cout << "entered realFactchanger"<< endl;
    map<string,Node*>appendedfactstree;
    appendedfactstree.insert(factstree.begin(),factstree.end());
    string nps = "";
    traverseStringPrint(inp,0,nps);
    Node* inp1 = parseTree(nps);
    appendedfactstree.insert(make_pair("pseudo",inp1));
    if(fill_allPropagate(appendedfactstree))
    {
        factstree.clear();
        factstree.insert(appendedfactstree.begin(),appendedfactstree.end());
        factstree.erase("pseudo");
        //cout<<"observed exists-fillall dependency";
        return true;
    }
    
    //cout << "exited realFactchanger"<< endl;
    return false;
}
bool exists_propagate(map<string,Node*>&factstree)
{
    //cout << "entered exists_propagate"<< endl;
    int changeFlag = 1;
    map<string,Node*>::iterator ito;
    
    vector<Node*>::iterator itchild;
    vector<Node*>::iterator itchild2;
    //construct tree with role chains
    map<pair<string,vector<string> >,Node*>pseudotree;
    map<pair<string,vector<string> >,Node*>::iterator it1;
    map<pair<string,vector<string> >,Node*>::iterator it2;
    for(ito=factstree.begin();ito!=factstree.end();ito++)
    {
        pseudotree.insert(make_pair(make_pair(ito->first,vector<string>()),ito->second));
    }
    //cout << "made pseudotree"<<endl;
    bool realFactchange = false;
    while(changeFlag)
    {
        //printfacts(factstree);
        // pstprinter(pseudotree);
        changeFlag = 0;
        for(it1=pseudotree.begin();it1!=pseudotree.end();it1++)
        {
            for(itchild=((it1->second)->children).begin();itchild!=((it1->second)->children).end();itchild++)
           {
                if((*itchild)->type == "exists")
                {   
                    //cout<<"found a exists"<<endl<<flush;
                    for(itchild2=((it1->second)->children).begin();itchild2!=((it1->second)->children).end();itchild2++)
                    {
                        // cout<<"looping for victim"<<endl<<flush;
                        if((*itchild2)->type == "all" && (*itchild2)->name == (*itchild)->name)
                        {
                            // cout<<"found all"<<endl<<flush;
                            int indiPresentFlag = 0;
                            for(it2=pseudotree.begin();it2!=pseudotree.end();it2++)
                            {
                                // cout << "entered indi2 loop"<< endl<<flush;
                                if(rolechaincomp(it1->first,(*itchild2)->name,it2->first))
                                {
                                    // cout << "faulty"<< endl<<flush;
                                    indiPresentFlag = 1;
                                    string sminp="";
                                    //cout << "TSP on "<< (it2->second)->type<<endl;
                                    traverseStringPrint(((*itchild2)->children)[0],0,sminp);
                                    string sminp2="";
                                    traverseStringPrint(it2->second,0,sminp2);

                                    if(StructMatching("'"+sminp+"'","'"+sminp2+"'"))
                                    {
                                        //nothing
                                        //cout<<"meh"<<endl;
                                    }
                                    else
                                    {
                                        //cout << "got in" <<endl;
                                        changeFlag = 1;
                                        Node* replace = new Node();
                                        replace->type = "and";
                                        replace->name = "concepts";
                                        replace->children.push_back(((*itchild2)->children)[0]);
                                        replace->children.push_back(it2->second);
                                        normalise(replace);
                                        //factstree[(it2->first)] = replace;

                                        // Nodereplace;
                                        // replace.type = "and";
                                        // replace.name = "concepts";
                                        // vector <Node*> childrenToBe;
                                        // childrenToBe.push_back(((*itchild2)->children)[0]);
                                        // childrenToBe.push_back(it2->second);
                                        // replace.children = childrenToBe;
                                        // normalise(&replace);
                                        // //cout << "insertingz"<< replace.type << endl;
                                        pseudotree[(it2->first)] = replace;
                                        realFactchange = realFactchanger(replace,factstree);
                                        //cout<<"done with normalize"<<replace.type<<endl;
                                    }
                                }
                                //cout << "Exited"<< endl;
                            }
                            if(indiPresentFlag == 0)
                            {
                                // cout << "suspected entry"<<endl<<flush;
                                //cout << "encountered rolechain" << endl;
                                changeFlag = 1;
                                pair<string,vector<string> >p =it1->first;
                                (p.second).push_back((*itchild2)->name);
                                // cout << "adding role"<<(*itchild2)->name<<endl<<flush;
                                //cout << "insertingz"<< ((*itchild2)->children)[0]->type << endl;


                                string nps = "";
                                traverseStringPrint(((*itchild2)->children)[0],0,nps);
                                Node* newparse = parseTree(nps);                            
                                pseudotree.insert(make_pair(p,newparse));
                                // cout << "created new pseduoindividual"<<flush;
                                //cout <<"zzzzzzzzzz" << ((*itchild2)->children)[0]->type << endl;
                                realFactchange = realFactchanger(newparse,factstree);
                                //cout<<"successfully added a new pseduoindividual" <<endl;
                                //cout << "suspected exit"<<endl<<flush;
                            }
                        }
                    }
                }
            } 
        }
    }
    //cout << "exited exists_propagate"<< endl;
    return realFactchange;
}

// void spacehandle(string &s)
// {
//     while(s[0]==' ')
//     {
//         s.erase(0,1);
//     }
//     while(s[s.size()-1]==' ')
//     {
//         s.erase()
//     }
// }

void traverse(Node* root,int n){
    if(root->type=="exists"){
        cout << root->type << " " << root->name << " " << root->number << endl;
    }else if(root->type=="fills"){
        cout << root->type << " " << root->name << " " << root->constant << endl;
    }else{
        cout << root->type << " " << root->name << " " << root->children.size() << endl;
    }
    for(int i=0;i<root->children.size();i++){
        for(int j=0;j<n;j++) cout << "\t";
        traverse(root->children[i],n+1);
    }
}

void traverseStringPrint(Node* root,int n, string &s){
    //cout << "enteredhere"<<endl;
    // transform(root->type.begin(), root->type.end(), root->type.begin(), ::toupper);
    if(root->type=="exists"){
        //cout << "["<<root->type << " " << root->number << " :" << root->name<<"] ";
        s+="[EXISTS " + to_string(root->number) + " :" + root->name +"] ";
    }else if(root->type=="fills"){
        //cout<< "[" << root->type << " :" << root->name << " " << root->constant << "] ";
        s+="[FILLS :" + root->name + " " + root->constant + "] ";
    }else if(root->type=="concept"){
        //cout << "special";
        //cout << " "<<root->name;
        if(n==0)
          s+=root->name;
        else
            s+=" "+root->name;
    }
    else if(root->type=="all"){
        //cout << "[" << root->type << " :";
        s+="[ALL :";
        //cout << root->name << " ";
        s+=root->name + " ";
        for(int i=0;i<root->children.size();i++){
            // for(int j=0;j<n;j++) //cout << "\t";
            traverseStringPrint(root->children[i],n+1,s);
        }
        //cout << "]";
        s+="]";
    }
    else{
        //cout << "[" << root->type << " ";
        s+="[AND ";
        
        for(int i=0;i<root->children.size();i++){
            // for(int j=0;j<n;j++) //cout << "\t";
            traverseStringPrint(root->children[i],n+1,s);
        }
        //cout << "]";
        s+="]";
    }
    //cout << "s so far is" << s << endl;
}



void flattenAnd(Node* root){
    //cout << "1 "  << flush << endl;
    vector<Node*> vec;
    queue<Node*> que;
    que.push(root);
    while(!que.empty()){
        Node* t=que.front();
        que.pop();
        if(t->type=="and"){
            for(int i=0;i<t->children.size();i++){
                que.push(t->children[i]);
            }
        }else{
            vec.push_back(t);
        }
    }

    root->children=vec; 
    //cout << "1 finished"  << flush << endl;
    
}

bool comp(Node* a,Node* b){
    if(a->name<b->name)
        return true;
    return false;
}

Node* combineConcepts(vector<Node*> vec){
    Node* node = new Node();
    node->type="and";
    node->name="Concepts";
    for(int i=0;i<vec.size();i++){
        node->children.push_back(vec[i]->children[0]);
    }
    return node;
}

void someDamnFunction(Node* root,vector<Node*> &temp){

    if(temp.size()==1){
        normalise(temp[0]->children[0]);
        root->children.push_back(temp[0]);
    }else{
        Node* andNode=combineConcepts(temp);
        normalise(andNode);
        Node* roleNode = new Node();
        roleNode->type="all";
        roleNode->name=temp[0]->name;
        roleNode->children.push_back(andNode);
        root->children.push_back(roleNode);
    }
    temp.clear();
}

void combineAll(Node* root){
    //cout << "2 "  <<  flush << endl;
    vector<Node*> vec,others;
    for(vector<Node*>::iterator it=root->children.begin();it!=root->children.end();it++){
        if((*it)->type=="all"){
            vec.push_back(*it);
        }else{
            others.push_back(*it);
        }
    }
    if(vec.size()==0){ // no all expressions
        return;
    }
    if(vec.size()==1){ // flattenAnd, as there is only one all expression
        normalise(vec[0]->children[0]);
        return;
    }
    root->children.clear();
    sort(vec.begin(),vec.end(),comp);

    string role=vec[0]->name;
    vector<Node*> temp;
    temp.push_back(vec[0]);
    int i=1;
    while(i<vec.size()){
        if(role==vec[i]->name){
            temp.push_back(vec[i]);
            i++;
        }else{
            someDamnFunction(root,temp);
            role=vec[i]->name;
        }
    }
    someDamnFunction(root,temp);
            
    // add the concepts back to the children
    for(int i=0;i<others.size();i++){
        root->children.insert(root->children.begin(),others[i]);
    }
    //cout << "2 fnished"  <<  flush << endl;
    
}


void combineExists(Node* root){
    //cout << "3 " << flush << endl;
    vector<Node*> vec,others;
    for(vector<Node*>::iterator it=root->children.begin();it!=root->children.end();it++){
        if((*it)->type=="exists"){
            vec.push_back(*it);
        }else{
            others.push_back(*it);
        }
    }
    if(vec.size()<=1){ // less than one exists expressions
        return;
    }

    root->children.clear();
    sort(vec.begin(),vec.end(),comp);

    string role=vec[0]->name;
    int no=vec[0]->number;
    int i=1;
    while(i<vec.size()){
        if(role==vec[i]->name){
            no=max(no,vec[i]->number);
            i++;
        }else{
            Node* node = new Node();
            node->type="exists";
            node->name=role;
            node->number=no;
            root->children.push_back(node);
            role=vec[i]->name;
            no=vec[i]->number;
        }
    }
    Node* node = new Node();
    node->type="exists";
    node->name=role;
    node->number=no;
    root->children.push_back(node);
            
    // add the concepts back to the children
    for(int i=0;i<others.size();i++){
        root->children.insert(root->children.begin(),others[i]);
    }
    //cout << "3 finished" << flush << endl;
    
}

bool removeDuplicates(Node* a,Node* b){
    if(a->type=="concept" && a->type==b->type && a->name==b->name){
        return true;
    }else if(a->type=="fills" && a->type==b->type && a->name==b->name && a->constant==b->constant){
        return true;
    }
    return false;
}

bool sortChildren(Node* a,Node* b){
    if(a->type<b->type){
        return true;
    }else if(a->type==b->type){
        if(a->name<=b->name){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
    return false;
}

void shrinkNode(Node* root){
    Node* child=root->children[0];
    root->children.clear();
    root->type=child->type;
    root->name=child->name;

    if(child->type=="all")
        root->children.push_back(child->children[0]);
    else if(child->type=="exists")
        root->number=child->number;
    else if(child->type=="fills")
        root->constant=child->constant;
    else if(child->type=="concept"){

    }else
    {
        //cout << "something wrong in shrink Node" << endl;
    }

}


void normalise(Node* root){
    //cout << "nomral " << flush << endl;
    if(root->type=="concept")
        return;
    flattenAnd(root);        
    combineAll(root);
    combineExists(root);

    // removing duplicates...
    sort(root->children.begin(),root->children.end(),sortChildren);
    vector<Node*>::iterator dup=unique(root->children.begin(),root->children.end(),removeDuplicates);
    root->children.resize(distance(root->children.begin(), dup));

    // shrink Node if only one child
    if(root->children.size()==1){
        shrinkNode(root);
    }

    //cout << "nomral finished" << flush << endl;
    
}

