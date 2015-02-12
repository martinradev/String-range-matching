# Implementation and architecture report

The project is split into two parts, a header-only C++11 library and a command
line utility that utilizes the library and allows the user to run different
algorithms and load input texts and patterns from different sources. The utility
also features timing tools and uses separate memory allocation functions for
input/output strings and allocations done within algorithm execution which
allows monitoring the algorithm memory consumption with external tools like
valgrind.

## Algorithm implementations

The algorithms are implemented within library header files in
`include`-directory. All implementations are templated and use iterators for
input text, patterns and output to allow generic usage.

### Naive algorithm

Headers: include/naive_match.hpp
Author: Jarno

### Z-algorithm based search

* **Headers**: [naive_match.hpp](include/ZAlgorithm.hpp)
* **Author**: Martin
In general, the Z algorithm creates an array where Z[i] corresponds to the longest common prefix between the string and the i-th suffix. A generalized string would be one for which we have T = A$B where A and B are two strings and $ is a symbol which is not contained in either. When we want to determine all of the suffixes of a given string B which are smaller than another string A, we can utilize the information computed by the Z algorithm. Namely, if 
Z[i] = K where i corresponds to string B, then we know that the first K characters of A and B[t..) match where t = i - |A| - 1. This means that A[K] != B[t+1] and we can just compare the characters normally. Note, that in C/C++ ending of a string is signified by a zero character \0 (having value of 0) and we can simply ignore checking for the end.   
It has an O(N+M) space complexity and O(N+M) time complexity.

### Suffix array search

* **Headers**: [naive_match.hpp](include/SuffixArray.hpp)
* **Author**: Martin
It builds a suffix array using Yuta Mori's SAIS implementation [3], an inverse suffix array and an lcp array. The time and space complexities for a text T are O(|T|). Lower and upper bound range queries are also implemented which use binary search. The worst case running time for an input of a pattern P and a text T is O(|P| * log(|T|)), but O(|P| + log(|T|)) on average.

### Crochemore-based algorithm

* **Headers**: [naive_match.hpp](include/Crochemore.hpp)
* **Author**: Martin
It find the suffixes in a text T which are lexicographically smaller than a given pattern P. It uses O(1) extra space and runs in O(|T| + |P|) time. It utilizes string combinatorics about the pattern to comptute the next jump to make in the text.
The algorithm is described in full detail in [1].


### Algorithm based on Knuth-Morris-Pratt

Headers: include/Crochemore.hpp
Author: Jarno

### Counting algorithm based on Galil-Seiferas

Headers: gs_count.hpp, gs_count_detail.hpp
Author: Jarno

## References

<a name="1"></a>[[1] J. Kärkkäinen, D. Kempa, S. Puglisi: String Range Matching. 
Proceedings of the 25th Symposium on Combinatorial Pattern Matching (CPM), pp.   
232-241, 2014.](http://dx.doi.org/10.1007/978-3-319-07566-2_24)                  
                                                                                 
<a name="2"></a>[[2] J. Kärkkäinen: Fast BWT in small space by blockwise suffix  
sorting.  Theoretical Computer Science 387, pp. 249–257,                         
2007.](http://dx.doi.org/10.1016/j.tcs.2007.07.018)                              
                                                                                 
<a name="3"></a>[[3] G. Nong, S. Zhang, W. H. Chan: Linear Suffix Array          
Construction by Almost Pure Induced-Sorting. Data Compression Conference, p.     
193, 2009.](http://dx.doi.org/10.1016/j.tcs.2007.07.018)
