# Query-Handler-Search-Engine
A search engine which can handle wildcard queries, incorrect spelling and documents ranking on the basis of IDF scores of vector space model.
One can simply run the code Trie.cpp which imports the dictionary and IDF scores from the file files.txt.
The code asks the user to enter the number of queries and then the user may have the wildcard query which may be the prefix query or the suffix query.
This code uses the concept of Levenstien distance to calculate the distance between the input words and the words in the dictionary one could use the BK tree for the faster spelling correction method.
This code uses the concept of Ternary Trie which is memory efficient and have the performance capability comparable to the Hash Table.
One could use DAFSA instead of Ternary trie to implement this code which would have more efficiency than Ternary Trie
