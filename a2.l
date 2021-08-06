%{
	#include<bits/stdc++.h>
	using namespace std;
	#include "y.tab.h" 
	

	extern string name;
	extern string constant;
	extern int number;
	
%}

%%

"AND"						{return AND;}
"ALL"						{return ALL;}
"FILLS"						{return FILLS;}
"EXISTS"					{return EXISTS;}

[0-9]+						{number = atoi(yytext);return INTEGER;}
[A-Z][A-Za-z0-9]*			{name = yytext;return NAME;}
[a-z][A-Za-z0-9]*			{constant = yytext;return CONSTANT;}

"["		{ return('[');}
"]"		{ return(']');}
":"		{return(':');}
[ \t\v\n\f]	;
.			{printf("Unknown Character:%d\n",*yytext);}

%%

int yywrap(void) {
	return 1;
}

void yyerror(string s) {
    cout << s << endl;
}

static YY_BUFFER_STATE my_string_buffer;

int my_scan_string(const char *s) {
    // insist on cleaning up any existing buffer before setting up a new one
    if (my_string_buffer != NULL) return -1;

    // Set up and switch to a buffer for scanning the contents of the
    // specified string.  A copy of the string will be made.
    my_string_buffer = yy_scan_string(s);
    return (my_string_buffer == NULL) ? -1 : 0;
}

void my_cleanup(void) {
    // No effect if my_string_buffer is NULL
    yy_delete_buffer(my_string_buffer);
    // ... but avoid trying to free the same buffer twice
    my_string_buffer = NULL;
}