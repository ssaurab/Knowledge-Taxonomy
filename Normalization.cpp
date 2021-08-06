#include <bits/stdc++.h>
#include "functions.h"
using namespace std;

struct Node{
	string type;
	string name;
	string constant;
	int number;
	vector <node*> children;
};




int main()
{
	vector <node*> rootchildren;
	node leaf;
	leaf.type = "concept";
	leaf.name = "Canadian";
	node lvl12;
	lvl12.type = "all";
	lvl12.name = "managedBy";
	lvl12.children = &leaf; 
	rootchildren.push_back(&lvl12);
	node lvl11,lvl13;
	lvl11.type = "concept";
	lvl11.name = "Company";
	lvl13.type = "fills";
	lvl13.name = "managedBy";
	lvl13.const = "Julie";
	rootchildren.push_back(&lvl11);
	rootchildren.push_back(&lvl13);
	node root1;
	root1.type = "and";
	root1.name = "concepts"
	root1.children = rootchildren;

	root2.type = "concept";
	root2.name = "person";

	map <string, node*> factstree;
	factstree.push_back("canCorp", &root1);
	factstree.push_back("joe", &root2);


	fill_allPropagate(factstree);

	return 0;
}