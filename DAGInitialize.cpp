#include <bits/stdc++.h>
using namespace std;

int main()
{
	ofstream myfile;
	myfile.open("DAG.py");
	myfile<< "import graphviz" << endl << "from graphviz import Digraph" << endl << "dot = Digraph(format='png')" << endl << "dot.attr(size=\"9,9\")"<<endl<<"dot.attr(bgcolor = \"#fffdd0\")"<<endl;

	map <string, char> NameToChar;
	map <char, string> CharToName;

	char arsenal[36] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9','0'};
	int posArsenal = 0;
	string line;
	bool First = true;
	ifstream taxonomy ("taxonomy.txt");
	if(taxonomy.is_open())
	{
		while(getline(taxonomy,line))
		{
			if(line.find(',') != string::npos)
			{
				string child = line.substr(line.find(",") + 1); 
				string parent = line.substr(0, line.find(",", 0));
				if(First)
				{
					myfile << "dot.edges([";
				}
				if(First)
				{
					myfile << "'"<<NameToChar[parent] << NameToChar[child] << "'";
					First = false;
				}
				else
				{
					myfile << ",'"<<NameToChar[parent] << NameToChar[child] << "'";
				}
			}
			else
			{
				NameToChar.insert(make_pair(line,arsenal[posArsenal]));
				CharToName.insert(make_pair(arsenal[posArsenal],line));
				myfile << "dot.node('" << NameToChar[line] << "','" << line << "')"<<endl;
				posArsenal++;
			}
		}
	}

	myfile << "])" << endl << "print(dot.source)" << endl << "dot.render(\"Taxonomy\", view=True)";

}