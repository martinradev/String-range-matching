# Implementation and architecture report

The project is split into two parts, a header-only C++11 library and a command
line utility that utilizes the library and allows the user to run different
algorithms and load input texts and patterns from different sources. The utility
also features timing tools and uses separate memory allocation functions for
input/output strings and allocations done within algorithm execution which
allows monitoring the algorithm memory consumption with external tools like
valgrind.

### Who did what?

Martin implemented the test framework while Jarno did the command line utility
and the build environment. Algorithm implementation responsibilities were
divided roughly equally between both; these are specified in more detail in the
next section.

## Algorithm implementations

The algorithms are implemented within library header files in
[`include`](include)-directory. All implementations are templated and use
iterators for input text, patterns and output to allow generic use. Detailed
documentation on how to use the algorithms can be found in the respective header
files.

### Naive algorithm

  * **Headers**: [naive_match.hpp](include/naive_match.hpp)
  * **Author**: Jarno

The naive algorithm iterates over all suffixes in the text and compares the
lower and upper bound patterns lexicographically. The worst case time complexity
is O(nm), where n is the length of the text and m is the length of the larger
input pattern.

### Z-algorithm based search

* **Headers**: [ZAlgorithm.hpp](include/ZAlgorithm.hpp)  
* **Author**: Martin

In general, the Z algorithm creates an array where Z[i] corresponds to the
longest common prefix between the string and the i-th suffix. A generalized
string would be one for which we have T = A$B where A and B are two strings and
$ is a symbol which is not contained in either. When we want to determine all of
the suffixes of a given string B which are smaller than another string A, we can
utilize the information computed by the Z algorithm. Namely, if Z[i] = K where i
corresponds to string B, then we know that the first K characters of A and
B[t..) match where t = i - |A| - 1. This means that A[K] != B[t+1] and we can
just compare the characters normally. Note, that in C/C++ ending of a string is
signified by a zero character \0 (having value of 0) and we can simply ignore
checking for the end.   It has an O(N+M) space complexity and O(N+M) time
complexity.   You can read more about it
[here](http://codeforces.com/blog/entry/3107).

### Suffix array search

* **Headers**: [SuffixArray.hpp](include/SuffixArray.hpp)   
* **Author**: Martin

It builds a suffix array using Yuta Mori's SAIS implementation [[3]](#3), an
inverse suffix array and an lcp array. The time and space complexities for a
text T are O(|T|). Lower and upper bound range queries are also implemented
which use binary search. The worst case running time for an input of a pattern P
and a text T is O(|P|*log(|T|)), but O(|P|+log(|T|)) on average.

### Crochemore-based algorithm

* **Headers**: [Crochemore.hpp](include/Crochemore.hpp)
* **Author**: Martin

It find the suffixes in a text T which are lexicographically smaller than a
given pattern P. It uses O(1) extra space and runs in O(|T| + |P|) time. It
utilizes string combinatorics about the pattern to comptute the next jump to
make in the text.  The algorithm is described in full detail in [[1]](#1)


### Algorithm based on Knuth-Morris-Pratt

  * **Headers**: [kmp_match.hpp](include/kmp_match.hpp)
  * **Author**: Jarno

This algorithm is based on Knuth-Morris-Pratt exact string search
and is described on page 253 in [[2]](#2). The algorithm is implemented
as a generic STL iterator allowing the use of generic STL algorithms
and avoiding having to save large intermediate index sequences if
only a part of the results are used. The total running time iterating
over all matching suffixes takes O(n+m) time and O(m) extra space.

### Counting algorithm based on Galil-Seiferas

  * **Headers**: [gs_count.hpp](include/gs_count.hpp),
    [gs_count_detail.hpp](include/gs_count_detail.hpp)
  * **Author**: Jarno

This algorithm counts the number of matching suffixes in O(n+m)
time and O(log(m)) extra space. It is described in [[1]](#1).
The implementation tries to match the original publication format
syntactically as accurately as possible. The auxiliary functions
described in the original publication are implemented in the
[gs_count_detail.hpp](include/gs_count_detail.hpp).

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
