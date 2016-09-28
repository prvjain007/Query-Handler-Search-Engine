/*
1-The following code is developed for Information Retrieval-CS(469) course taught in
  BITS-Pilani for an assignment for handling wild card queries and Spell queries.
2-This code handles suffix queries as well as prefix queries.
3-Data structure for storing dictionary element is Ternary Trie which
  effectively handles wild card queries with less amount of extra space because
  it has only three child nodes in comparison with 26 children of R-way trie.
4-Prefix queries traverse through the ternary trie and when it mismatches it
  gives all the words which could be found by further traversing.
5-To handle suffix query a reverse Ternary trie is constructed and traversal
  is done in a similar way like prefix query.
6-For the query it also gives the longest matching sub sequences.
7-The third option of this code computes Levenshtein Distance between
  two strings and they are stored in a priority queue to extract the elements
  with minimum Levenshtein Distance.
8-To compute Levenshtein Distance a dynamic programming method is used which
  recursively stores the minimum distance between the optimal subproblems and
  the solution of those subproblems gives the optimal solution of final problem.
9-Another method of computing Levenshtein distance is to compute the Levenshtein
  distance between root with all other strings and a BK tree could be used to store
  the words of dictionary which could reduce the computation time of the test string
  with each and every word and traverse like a tree.
*/


#include <bits/stdc++.h>
#include <ctime>
using namespace std;


//Structure Declaration for Ternary Trie
struct Ternary_Trie_Forward
{
	Ternary_Trie_Forward* left;
	Ternary_Trie_Forward* mid;
	Ternary_Trie_Forward* right;
	int IDF_Weight=0;
	char CharToBeAdded;
};

struct Ternary_Trie_Reverse
{
	Ternary_Trie_Reverse* left;
	Ternary_Trie_Reverse* mid;
	Ternary_Trie_Reverse* right;
	int IDF_Weight=0;
	char CharToBeAdded;
};

//Integers to hold Number of prefix and suffix respectively
int nop=0,nos=0;

Ternary_Trie_Forward* ForwardTrie_Root;
Ternary_Trie_Reverse* ReverseTrie_Root;


string QueryString,Suggested_String,TopString;

int Weight_Int;


Ternary_Trie_Forward* ForwardTrieNode(char CharToBeAdded)
{
	Ternary_Trie_Forward *CurrentNode = (Ternary_Trie_Forward*)malloc(sizeof(Ternary_Trie_Forward));
	CurrentNode->IDF_Weight = 0;
	CurrentNode->left = NULL;
    CurrentNode->right = NULL;
    CurrentNode->mid = NULL;
    CurrentNode->CharToBeAdded = CharToBeAdded;
    return CurrentNode;
}

Ternary_Trie_Reverse* ReverseTrieNode(char CharToBeAdded)
{
	Ternary_Trie_Reverse *CurrentNode = (Ternary_Trie_Reverse*)malloc(sizeof(Ternary_Trie_Reverse));
	CurrentNode->IDF_Weight = 0;
	CurrentNode->left = NULL;
    CurrentNode->right = NULL;
    CurrentNode->mid = NULL;
    CurrentNode->CharToBeAdded = CharToBeAdded;
    return CurrentNode;
}


//Priority Queue to give top results for wildcard queries
class PriorityQueue
{
    string SuggestedString;
    int weight;


public:
    PriorityQueue(int d,string n="1"):SuggestedString(n), weight(d)  {}

    friend bool operator <(const PriorityQueue& TestObject1, const PriorityQueue& TestObject2)
    {
        if(TestObject2.weight>TestObject1.weight)
            return true;
        return false;
    }
    friend string operator !( const PriorityQueue& TestObject)
    {
        return TestObject.SuggestedString;
    }
};


//Priority Queue to give least distance strings
class LD_PriorityQueue
{
    string Spell_String;
    int LDistance;


public:
    LD_PriorityQueue(int d,string n="1"):Spell_String(n), LDistance(d)  {}

    friend bool operator <(const LD_PriorityQueue& QueryString, const LD_PriorityQueue& FileString)
    {
        if(FileString.LDistance<QueryString.LDistance)
            return true;
        return false;
    }

    friend string operator !( const LD_PriorityQueue& TestObject)
    {
        return TestObject.Spell_String;
    }

};

priority_queue <PriorityQueue> PriorityQueuel;
priority_queue <LD_PriorityQueue> Suffix_Queue;
priority_queue <PriorityQueue> Prefix_Queue;



//Recursive function to find Prefix
void Prefix_Suggestor(Ternary_Trie_Forward* ForwardTrie_Root, char* temp_arr, int Index)
{

    if (ForwardTrie_Root)
    {
        Prefix_Suggestor(ForwardTrie_Root->left, temp_arr, Index);


        temp_arr[Index] = ForwardTrie_Root->CharToBeAdded;
        if (ForwardTrie_Root->IDF_Weight)
        {
            temp_arr[Index+1] = '\0';
            Suggested_String=QueryString;

            //printf( "%Suggested_String ", temp_arr);
            //cout<<ForwardTrie_Root->wt<<endl;
            Suggested_String.append(temp_arr);
            //cout<<Suggested_String;
            nos++;
            //cout<<"The suffix results are " <<Suggested_String<<" "<<ForwardTrie_Root->IDF_Weight<<endl;
            PriorityQueuel.push(PriorityQueue(ForwardTrie_Root->IDF_Weight, Suggested_String));

        }


        Prefix_Suggestor(ForwardTrie_Root->mid, temp_arr, Index + 1);


        Prefix_Suggestor(ForwardTrie_Root->right, temp_arr, Index);


    }

}



void Suffix_Suggestor(Ternary_Trie_Reverse* ReverseTrie_Root, char* temp_arr1, int Index)
{
    //cout<<"hi";
    if (ReverseTrie_Root)
    {

        Suffix_Suggestor(ReverseTrie_Root->left, temp_arr1, Index);


        temp_arr1[Index] = ReverseTrie_Root->CharToBeAdded;
        if (ReverseTrie_Root->IDF_Weight)
        {
            temp_arr1[Index+1] = '\0';
            Suggested_String=QueryString;

            //printf( "%Suggested_String ", temp_arr);
            //cout<<ForwardTrie_Root->wt<<endl;
            Suggested_String.append(temp_arr1);
            //cout<<Suggested_String;
            reverse(Suggested_String.begin(),Suggested_String.end());
            nop++;
            //cout<<"Prefix results are " <<Suggested_String<<" "<<ReverseTrie_Root->IDF_Weight<<endl;
            Prefix_Queue.push(PriorityQueue(ReverseTrie_Root->IDF_Weight, Suggested_String));

        }


        Suffix_Suggestor(ReverseTrie_Root->mid, temp_arr1, Index + 1);


        Suffix_Suggestor(ReverseTrie_Root->right, temp_arr1, Index);


    }

}


//Calling function to generate different answers to queries
void Prefix_Generator(Ternary_Trie_Forward* ForwardTrie_Root)
{
    char temp_arr[100100];
    Prefix_Suggestor(ForwardTrie_Root, temp_arr, 0);
    cout<<nos<<" suffix queries are found"<<endl;
    cout<<"Enter number of results you want to see by IDF ranks"<<endl;
    int cou=0,zzz;
    cin>>zzz;
    while(cou <zzz&& !PriorityQueuel.empty())
    {
        TopString=!PriorityQueuel.top();
        cout<<TopString<<endl;
        PriorityQueuel.pop();
        cou++;
    }
    while(!PriorityQueuel.empty())
    {
        PriorityQueuel.pop();
    }
    nos=0;
}



void Suffix_Generator(Ternary_Trie_Reverse* ReverseTrie_Root)
{
    char temp_arr1[100100];
    Suffix_Suggestor(ReverseTrie_Root, temp_arr1, 0);
    cout<<nop<<" prefix queries are found"<<endl;
    cout<<"Enter number of results you want to see by IDF ranks"<<endl;
    int cou=0,zzz;
    cin>>zzz;
    while(cou<zzz&& !Prefix_Queue.empty())
    {
        TopString=!Prefix_Queue.top();
        cout<<TopString<<endl;
        Prefix_Queue.pop();
        cou++;
    }

    nop=0;
}



Ternary_Trie_Forward* Prefix_Char_Checker(Ternary_Trie_Forward* node, int Index)
{
    //darr=d;

	if (node == NULL)
        {
            return NULL;
            //baap.c=c;
        }

        //string b;
        char CharToBeAdded=QueryString.at(Index);
        //strcat(b,c);
        //cout<<c;

        if(CharToBeAdded< node->CharToBeAdded)
            return Prefix_Char_Checker(node->left, Index);
        else if (CharToBeAdded> node->CharToBeAdded)
            return Prefix_Char_Checker(node->right, Index);
        else if (Index<QueryString.length()-1)
            return Prefix_Char_Checker(node->mid, Index+1);
        else
            return node;



}


//Gives answer to the different queries
int Final_Prefix_result()
{

    Ternary_Trie_Forward* node=Prefix_Char_Checker(ForwardTrie_Root, 0);
    if(node==NULL)
    {
        cout<<"Sorry no suffix string available";
        return 0;
    }
    else if(node->mid!=NULL)
    {
        if(node->IDF_Weight!=0)
            cout<<"Query "<<QueryString<<" matches exactly"<<endl;
        Prefix_Generator(node->mid);
        return 1;
    }
    cout<<"Your query "<<QueryString<<" is in the docs and no other suffixes are available"<<endl;
    return 1;
}



Ternary_Trie_Reverse* Suffix_Char_Checker(Ternary_Trie_Reverse* x1, int Index)
{
	if (x1 == NULL)
        {
            return NULL;
            //baap.c=c;
        }
        //string b;
        char CharToBeAdded=QueryString.at(Index);
        //strcat(b,c);
        //cout<<c;
        if(CharToBeAdded < x1->CharToBeAdded)
            return Suffix_Char_Checker(x1->left, Index);
        else if (CharToBeAdded > x1->CharToBeAdded)
            return Suffix_Char_Checker(x1->right, Index);
        else if (Index < QueryString.length()-1)
            return Suffix_Char_Checker(x1->mid, Index+1);
        else
            return x1;


}



int Final_Suffix_result()
{
    Ternary_Trie_Reverse* x1=Suffix_Char_Checker(ReverseTrie_Root, 0);
    if(x1==NULL)
    {
        reverse(QueryString.begin(),QueryString.end());
        cout<<"No prefix query for "<< QueryString <<endl;
        return 0;
    }
    else if(x1->mid!=NULL)
    {
        if(x1->IDF_Weight!=0)
            cout<<"This matches the query exactly"<<endl;
        Suffix_Generator(x1->mid);
        return 1;
    }
    reverse(QueryString.begin(),QueryString.end());
    cout<<"Your query "<<QueryString<<" is in the docs without any suffixes"<<endl;
    return 1;
}



Ternary_Trie_Forward* MakeForwardTree(Ternary_Trie_Forward *node, int Index)
{
    char CharToBeAdded=QueryString.at(Index);
	if (node == NULL)
        {
            node= ForwardTrieNode(CharToBeAdded);
            node->CharToBeAdded=CharToBeAdded;
        }

        if(CharToBeAdded<node->CharToBeAdded)
            node->left= MakeForwardTree(node->left, Index);
        else if (CharToBeAdded>node->CharToBeAdded)
            node->right= MakeForwardTree(node->right, Index);
        else if (Index<QueryString.length()-1)
            node->mid= MakeForwardTree(node->mid, Index+1);
        else
            node->IDF_Weight=Weight_Int;
        return node;
}
void MakeForwardTree() {
    ForwardTrie_Root = MakeForwardTree(ForwardTrie_Root, 0);
}


Ternary_Trie_Reverse* MakeReverseTree(Ternary_Trie_Reverse *node, int Index)
{
    char CharToBeAdded=QueryString.at(Index);
	if (node == NULL)
        {
            node= ReverseTrieNode(CharToBeAdded);
            node->CharToBeAdded=CharToBeAdded;
        }

        if(CharToBeAdded<node->CharToBeAdded)
            node->left= MakeReverseTree(node->left, Index);
        else if (CharToBeAdded>node->CharToBeAdded)
            node->right= MakeReverseTree(node->right, Index);
        else if (Index<QueryString.length()-1)
            node->mid= MakeReverseTree(node->mid, Index+1);
        else
            node->IDF_Weight=Weight_Int;
        return node;
}

void MakeReverseTree()
{
    ReverseTrie_Root = MakeReverseTree(ReverseTrie_Root, 0);
}


void Read_File_For_Trie()
{

	 ifstream theFile("files.txt");

//	 int idNumber;
	// string name;
	 float FloatWt;

	 while(theFile >> QueryString  >> FloatWt)
	 {

        Weight_Int=(int)(FloatWt*100000);

        MakeForwardTree();
        reverse(QueryString.begin(), QueryString.end());
        MakeReverseTree();
	 }


}




int Calculate_LD(string Query_String, string Test_String)
{

    if (Query_String == Test_String)
        return 0;
    if (Query_String.length() == 0)
        return Test_String.length();
    if (Test_String.length() == 0)
        return Query_String.length();



    int vector1[Test_String.length() + 1];
    int vector2[Test_String.length() + 1];


    for (int i = 0; i < Test_String.length()+1; i++)
        vector1[i] = i;

    for (int i = 0; i < Query_String.length(); i++)
    {

        vector2[0] = i + 1;


        for (int j = 0; j < Test_String.length(); j++)
        {
            int cost = (Query_String[i] == Test_String[j]) ? 0 : 1;
            vector2[j + 1] = min(vector2[j] + 1,min( vector1[j + 1] + 1, vector1[j] + cost));
        }


        for (int j = 0; j < Test_String.length()+1; j++)
            vector1[j] = vector2[j];
    }

    return vector2[Test_String.length()];
}


void ReadFile_For_LD(string WrongString)
{

	 ifstream theFile("files.txt");
	 string Test_String;
	 float FloatWt;

    //cout<<b<<len_s;
    while(theFile >> Test_String  >> FloatWt)
    {
    int Index=Test_String.length(),LD_Between_Strings;
    //cout<<Test_String<<d<<endl;
    LD_Between_Strings=Calculate_LD(WrongString, Test_String);
    //cout<<Test_String<<LD_Between_Strings<<endl;
    Suffix_Queue.push(LD_PriorityQueue(LD_Between_Strings,Test_String));
    }
    int zzz=0;

    while(zzz<4&& !Suffix_Queue.empty())
    {
        TopString=!Suffix_Queue.top();
        cout<<"Did You Mean: "<<TopString<<endl;
        Suffix_Queue.pop();
        zzz++;
    }
    while(!Suffix_Queue.empty())
    {
        Suffix_Queue.pop();
    }


}

void WellWisher()
{
time_t now;
struct tm *now_tm;
int hour;

now = time(NULL);
now_tm = localtime(&now);
hour = now_tm->tm_hour;
if(hour>=4&&hour<=12)
    cout<<"                         Good Morning   "<<endl;
else if(hour>=12&&hour<=17)
    cout<<"                        Good Afternoon   "<<endl;
else if(hour>=17&&hour<=23)
    cout<<"                         Good Evening   "<<endl;
else
    cout<<"                          Soja Bhai    "<<endl;
cout<<"_____________________________________________________________"<<endl<<endl;
}

int main()
{

    WellWisher();
    cout<<"       Enter Number Of Queries- "<<"";
    //cout<<"HI Parve";
    int Case,Queries;
    cin>>Queries;
    for(int i=0;i<Queries;i++)
    {
    cout<<endl;
    cout<<"----------------|||~~~~~Query Handler~~~~~|||----------------"<<endl;
    cout<<"                  Enter 1 for Prefix query"<<endl;
    cout<<"                  Enter 2 for Suffix query"<<endl;
    cout<<"                  Enter 3 for Spell check"<<endl;

    cin>>Case;

    Read_File_For_Trie();
    //cout<<"OK"<<endl;

    if(Case==1)
    {

        cout<<"                  Enter prefix wildcard query"<<endl<<"                 ";
        cout<<" *";
        cin>>QueryString;
        reverse(QueryString.begin(),QueryString.end());
        Final_Suffix_result();
    }


    else if(Case==2)
    {
        cout<<"                  Enter suffix wildcard query"<<endl<<"                  ";
        cin>>QueryString;
        Final_Prefix_result();
    }


    else if(Case==3)
    {
        cout<<"                  Enter incorrect spelling"<<endl<<"                  ";
        cin>>QueryString;
    //string Suggested_String;
    //cin>>Suggested_String;
        //int QueryLength;
    //QueryLength=QueryString.length();
    //d=ss.length();
        ReadFile_For_LD(QueryString);
    }

    else
    {
        cout<<"----------------------Invalid Choice-------------------------"<<endl;
        continue;
    }


    }
	return 0;
}
