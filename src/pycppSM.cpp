#include <bits/stdc++.h>
using namespace std;


int SM(string cd1, string cd2)
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
}

int main()
{
	string cd1 = "'Funsz'";
	string cd2 = "'[AND Funsz Jdeep]'";

    SM(cd1, cd2);
	return 0;
}