#include<bits/stdc++.h>
using namespace std;

typedef struct Node{
        string type;
        string name;
        string constant;
        int number;
        vector<struct Node*> children;
    }Node;

// void normalise(Node* root);     // main function - this function calls below 3
// void flattenAnd(Node* root);    // flattening and arguments 
// void combineAll(Node* root);    
// void combineExists(Node* root);

// void traverse(Node* root,int n); // for printing the parse tree
// void traverseStringPrint(Node* root,int n,string &s);

// // Dont bother, Util fucntions
// Node* combineConcepts(vector<Node*> vec);
// void someDamnFunction(Node* root,vector<Node*> &temp);
// void shrinkNode(Node* root);
// // these are comparators
// bool comp(Node* a,Node* b);
// bool removeDuplicates(Node* a,Node* b);
// bool sortChildren(Node* a,Node* b);

    
// // /*********************** definitions *****************************/
// // // careful it's clumsy down there..

// void traverse(Node* root,int n){
//     if(root->type=="exists"){
//         cout << root->type << " " << root->name << " " << root->number << endl;
//     }else if(root->type=="fills"){
//         cout << root->type << " " << root->name << " " << root->constant << endl;
//     }else{
//         cout << root->type << " " << root->name << " " << root->children.size() << endl;
//     }
//     for(int i=0;i<root->children.size();i++){
//         for(int j=0;j<n;j++) cout << "\t";
//         traverse(root->children[i],n+1);
//     }
// }

// void traverseStringPrint(Node* root,int n, string &s){
//     transform(root->type.begin(), root->type.end(), root->type.begin(), ::toupper);
//     if(root->type=="EXISTS"){
//         cout << "["<<root->type << " " << root->name << " " << root->number <<"] ";
//         s+="["+root->type + " " + root->name + " " + to_string(root->number) +"] ";
//     }else if(root->type=="FILLS"){
//         cout<< "[" << root->type << " " << root->name << " " << root->constant << "] ";
//         s+="[" + root->type + " " + root->name + " " + root->constant + "] ";
//     }else if(root->type=="CONCEPT"){
//         cout << " "<<root->name;
//         s+=" "+root->name;
//     }
//     else if(root->type=="ALL"){
//         cout << "[" << root->type << " ";
//         s+="[" + root->type + " ";
//         cout << root->name << " ";
//         s+=root->name + " ";
//         for(int i=0;i<root->children.size();i++){
//             // for(int j=0;j<n;j++) cout << "\t";
//             traverseStringPrint(root->children[i],n+1,s);
//         }
//         cout << "]";
//         s+="]";
//     }
//     else{
//         cout << "[" << root->type << " ";
//         s+="[" + root->type + " ";
        
//         for(int i=0;i<root->children.size();i++){
//             // for(int j=0;j<n;j++) cout << "\t";
//             traverseStringPrint(root->children[i],n+1,s);
//         }
//         cout << "]";
//         s+="]";
//     }
// }


// void flattenAnd(Node* root){
//     cout << "1 "  << flush << endl;
//     vector<Node*> vec;
//     queue<Node*> que;
//     que.push(root);
//     while(!que.empty()){
//         Node* t=que.front();
//         que.pop();
//         if(t->type=="and"){
//             for(int i=0;i<t->children.size();i++){
//                 que.push(t->children[i]);
//             }
//         }else{
//             vec.push_back(t);
//         }
//     }

//     root->children=vec; 
//     cout << "1 finished"  << flush << endl;
    
// }

// bool comp(Node* a,Node* b){
//     if(a->name<b->name)
//         return true;
//     return false;
// }

// Node* combineConcepts(vector<Node*> vec){
//     Node* node = (Node*)malloc(sizeof(Node));
//     node->type="and";
//     node->name="Concepts";
//     for(int i=0;i<vec.size();i++){
//         node->children.push_back(vec[i]->children[0]);
//     }
//     return node;
// }

// void someDamnFunction(Node* root,vector<Node*> &temp){

//     if(temp.size()==1){
//         normalise(temp[0]->children[0]);
//         root->children.push_back(temp[0]);
//     }else{
//         Node* andNode=combineConcepts(temp);
//         normalise(andNode);
//         Node* roleNode = (Node*)malloc(sizeof(Node));
//         roleNode->type="all";
//         roleNode->name=temp[0]->name;
//         roleNode->children.push_back(andNode);
//         root->children.push_back(roleNode);
//     }
//     temp.clear();
// }

// void combineAll(Node* root){
//     cout << "2 "  <<  flush << endl;
//     vector<Node*> vec,others;
//     for(vector<Node*>::iterator it=root->children.begin();it!=root->children.end();it++){
//         if((*it)->type=="all"){
//             vec.push_back(*it);
//         }else{
//             others.push_back(*it);
//         }
//     }
//     if(vec.size()==0){ // no all expressions
//         return;
//     }
//     if(vec.size()==1){ // flattenAnd, as there is only one all expression
//         normalise(vec[0]->children[0]);
//         return;
//     }
//     root->children.clear();
//     sort(vec.begin(),vec.end(),comp);

//     string role=vec[0]->name;
//     vector<Node*> temp;
//     temp.push_back(vec[0]);
//     int i=1;
//     while(i<vec.size()){
//         if(role==vec[i]->name){
//             temp.push_back(vec[i]);
//             i++;
//         }else{
//             someDamnFunction(root,temp);
//             role=vec[i]->name;
//         }
//     }
//     someDamnFunction(root,temp);
            
//     // add the concepts back to the children
//     for(int i=0;i<others.size();i++){
//         root->children.insert(root->children.begin(),others[i]);
//     }
//     cout << "2 fnished"  <<  flush << endl;
    
// }


// void combineExists(Node* root){
//     cout << "3 " << flush << endl;
//     vector<Node*> vec,others;
//     for(vector<Node*>::iterator it=root->children.begin();it!=root->children.end();it++){
//         if((*it)->type=="exists"){
//             vec.push_back(*it);
//         }else{
//             others.push_back(*it);
//         }
//     }
//     if(vec.size()<=1){ // less than one exists expressions
//         return;
//     }

//     root->children.clear();
//     sort(vec.begin(),vec.end(),comp);

//     string role=vec[0]->name;
//     int no=vec[0]->number;
//     int i=1;
//     while(i<vec.size()){
//         if(role==vec[i]->name){
//             no=max(no,vec[i]->number);
//             i++;
//         }else{
//             Node* node = (Node*)malloc(sizeof(Node));
//             node->type="exists";
//             node->name=role;
//             node->number=no;
//             root->children.push_back(node);
//             role=vec[i]->name;
//             no=vec[i]->number;
//         }
//     }
//     Node* node = (Node*)malloc(sizeof(Node));
//     node->type="exists";
//     node->name=role;
//     node->number=no;
//     root->children.push_back(node);
            
//     // add the concepts back to the children
//     for(int i=0;i<others.size();i++){
//         root->children.insert(root->children.begin(),others[i]);
//     }
//     cout << "3 finished" << flush << endl;
    
// }

// bool removeDuplicates(Node* a,Node* b){
//     if(a->type=="concept" && a->type==b->type && a->name==b->name){
//         return true;
//     }else if(a->type=="fills" && a->type==b->type && a->name==b->name && a->constant==b->constant){
//         return true;
//     }
//     return false;
// }

// bool sortChildren(Node* a,Node* b){
//     if(a->type<b->type){
//         return true;
//     }else if(a->type==b->type){
//         if(a->name<=b->name){
//             return true;
//         }else{
//             return false;
//         }
//     }else{
//         return false;
//     }
//     return false;
// }

// void shrinkNode(Node* root){
//     Node* child=root->children[0];
//     root->children.clear();
//     root->type=child->type;
//     root->name=child->name;

//     if(child->type=="all")
//         root->children.push_back(child->children[0]);
//     else if(child->type=="exists")
//         root->number=child->number;
//     else if(child->type=="fills")
//         root->constant=child->constant;
//     else if(child->type=="concept"){

//     }else
//         cout << "something wrong in shrink node" << endl;

// }


// void normalise(Node* root){
//     cout << "nomral " << flush << endl;
//     if(root->type=="concept")
//         return;
//     flattenAnd(root);        
//     combineAll(root);
//     combineExists(root);

//     // removing duplicates...
//     sort(root->children.begin(),root->children.end(),sortChildren);
//     vector<Node*>::iterator dup=unique(root->children.begin(),root->children.end(),removeDuplicates);
//     root->children.resize(distance(root->children.begin(), dup));

//     // shrink node if only one child
//     if(root->children.size()==1){
//         shrinkNode(root);
//     }

//     cout << "nomral finished" << flush << endl;
    
// }

