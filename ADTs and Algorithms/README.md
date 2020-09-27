# MultiValue Maps

Many data types concern a single value (e.g. a hash table), so that a string acts as both the key (by which we search for the data) and also as the object we need to store (the value).
This assignment create an abstract data type - MultiValue Map (MVM) - to store the key-value pairs. 
The definition of an MVM ADT is given in mvm.h and mvm.c, and a file to test it is given in testmvm.c.

# Rhymes
In the usual place is a dictionary which, for every word, lists the phonemes (a code for a distinct unit of sound) used to pronounce that word in American English. 
In the text file the word itself, and its phonemes, are separated by a ‘#’). For instance, BOY#B OY1 shows that the word BOY has two phonemes : B and OY1.
This assignment will use the MutliValue Map written in above assignment to create two maps. An MVM map1 stores the word (as the key) and its final n phonemes as a single string (the value). 
Now map2 stores the word (value), keyed by its final n phonemes as a single string. 
Looking up the phonemes associated with a word can be done using the word as a key 1 (via map1), and looking up a word given its phonemes can be achieved using map2.
The number of phonemes specified for the specific rhyming is given via the command line
