#!/usr/bin/env python
# coding: utf-8

# In[171]:



from collections import defaultdict 
import os
# from IPython.display import Image, display
import re
import sys

# In[140]:


def dis_exists(s):
    rest = s[8:]
    space = rest.find(" ")
    num = int(s[8:8+space])
    role = s[9+space:-1]
    return num, role
    
def dis_fills(s):
    rest = s[7:]
    space = rest.find(" ")
    role = s[7:7+space]
    constant = s[8+space:-1]
    return role,constant

def dis_all(s):
    rest = s[5:]
    space = rest.find(" ")
    role = s[5:5+space]
    concept = s[6+space:-1]
    return role, concept


# In[147]:


def category(s):
    if s[0]!='[':
        if s[:4] == "ALL":
            return "all1"
        elif s[:6] == "EXISTS":
            return "exists1"
        elif s[:5] == "FILLS":
            return "fills1"
        else:
            return "atomic"
    else:
        if s[1] == 'E':
            return "exists"
        elif s[1] == 'A':
            return "all"
        else:
            return "fills"


# In[200]:


def dis_concept(conceptList):
    at = []
    al = []
    e = []
    f = []
    for l in conceptList:
        if l == '':
             l = l
        elif category(l) == "atomic":
            at.append(l)        
        elif category(l) == "all":
            al.append(l)
        elif category(l) == "all1":
            al.append('['+l+']')
        elif category(l) == "exists":
            e.append(l)
        elif category(l) == "exists1":
            e.append('['+l+']')
        elif category(l) == "fills1":
            f.append('['+l+']')
        else:
            f.append(l)
    return at,al,e,f


# In[143]:


def find_parens(s):
    toret = {}
    pstack = []

    for i, c in enumerate(s):
        if c == '[':
            pstack.append(i)
        elif c == ']':
            if len(pstack) == 0:
                raise IndexError("No matching closing parens at: " + str(i))
            toret[pstack.pop()]=i

    if len(pstack) > 0:
        raise IndexError("No matching opening parens at: " + str(pstack.pop()))

    return toret    


# In[144]:


def dis_InputConcept(string):
    if(string[1:4]=="AND"):
        string = string[5:-1]
        all_concepts = []
        brac = find_parens(string)
        for s in range(len(string)):
            if string[s:s+3]=="ALL":
                concept = string[s-1:brac[s-1]+1]
                all_concepts.append(concept)
        for a in all_concepts:
            string = string.replace(a,"")
        brac1 = find_parens(string)
        for s in range(len(string)):
            if string[s:s+5]=="FILLS":
                concept = string[s-1:brac1[s-1]+1]
                all_concepts.append(concept)
            elif string[s:s+6] == "EXISTS":
                concept = string[s-1:brac1[s-1]+1]
                all_concepts.append(concept)
        for a in all_concepts:
            string = string.replace(a,"")
    
        while(len(string)!=0):
            while(string[0]==" "):
                if(len(string)==1):
                    return all_concepts
                string = string[1:]
            while(string[-1]==" "):
                string = string[:-1]
            space1 = string.find(" ")
            if space1==-1:
                all_concepts.append(string)
                # print (string)
                return all_concepts
            concept = string[:space1]
            # print (concept)
            all_concepts.append(concept)
            string = string.replace(concept,"")
        return all_concepts
    else:
        return [string[1:-1]]


# In[208]:


def fun(subsumer,subsumed):
    subsumer = re.sub(' +', ' ', subsumer)
    subsumed = re.sub(' +', ' ', subsumed)
    if subsumer == subsumed or subsumer=="":
        sys.exit(8)
        return 0
    elif subsumed=="":
        sys.exit(7)
        return 0
    if subsumed[0] != '[' and subsumed[-1]!=']':
        subsumed += ']'
        subsumed = '['+subsumed
    if subsumer[0] != '[' and subsumer[-1]!=']':
        subsumer += ']'
        subsumer = '['+subsumer
    # print(subsumer)
    # print(subsumed)
    subsumer1 = dis_InputConcept(subsumer)
    subsumed1 = dis_InputConcept(subsumed)
    at1,al1,ex1,fl1 = dis_concept(subsumer1)
    at2,al2,ex2,fl2 = dis_concept(subsumed1)
    all_count = 0
    ex_count = 0
    fl_count = 0
    for a in at1:
        if a not in at2:
            # print ("no match")
            sys.exit(7)
            return 0
           
    for f1 in fl1:
        r1,c1 = dis_fills(f1)
        for f2 in fl2:
            r2,c2 = dis_fills(f2)
            if r1==r2 and c1==c2:
                fl_count = fl_count+1
    if fl_count!=len(fl1):
        # print ("no match")
        sys.exit(7)
        return 0
    
    for e1 in ex1:
        fill_flag = 0
        n1,r1 = dis_exists(e1)
        for e2 in ex2:
            n2,r2 = dis_exists(e2)
            if r1==r2 and n2>=n1:
                fill_flag =1
                ex_count = ex_count+1
        if fill_flag==0 and n1==1:
            for f in fl2:
                r2,c2 = dis_fills(f)
                if r1==r2:
                    ex_count = ex_count+1
                    break;
    if ex_count!=len(ex1):
        # print ("no match")
        sys.exit(7)
        return 0
    
    
    for a1 in al1:
        r1,c1 = dis_all(a1)
        for a2 in al2:
            r2,c2 = dis_all(a2)
            if r1==r2 and fun(c1,c2)==1:
                all_count = all_count+1
    if all_count!=len(al1):
        # print ("no match")
        sys.exit(7)
        return 0
    
    # print ("match")
    sys.exit(8)
    return 1


# In[209]:


cd1 = '[AND c1 c2[FILLS r1 c3][EXISTS 6 r2][ALL r1 [AND c4 c5]]]'
cd2 = '[AND c1 c2[FILLS r1 c3][EXISTS 6 r2][ALL  r1 c5]]'
cd3 = '[AND c1 c2 [FILLS r1 c3][EXISTS 6 r2][ALL r1 [AND c4 c5]]]'
cd4 = 'c5'
cd5 = '[AND c5 c6]'

cd6 = '[AND Person [FILLS WorksAt Hospital]]'
cd7 = '[AND Person [FILLS WorksAt Army]]'
cd8 = '[AND Person [FILLS WorksAt Hospital] [FILLS WorksAt Army]]'
cd9 = '[AND Car [FILLS ManufacturedBy FordCompany]]'

def main():
	argumentList = sys.argv[1:]
	# print(argumentList[0], argumentList[1])
	return fun(argumentList[0], argumentList[1])


if __name__ == "__main__":
	s = main()
	# return s
	sys.exit()