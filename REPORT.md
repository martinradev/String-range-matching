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
and the build environment. Algorithm implementation responsibilities were divided
roughly equally between both; these are specified in more detail in the next section.

## Algorithm implementations

The algorithms are implemented within library header files in
[`include`](include)-directory. All implementations are templated and use iterators for
input text, patterns and output to allow generic use. Detailed documentation on how to
use the algorithms can be found in the respective header files.

### Naive algorithm

  * **Headers**: [naive_match.hpp](include/naive_match.hpp)
  * **Author**: Jarno

The naive algorithm iterates over all suffixes in the text and compares the lower and
upper bound patterns lexicographically. The worst case time complexity is O(nm),
where n is the length of the text and m is the length of the larger input pattern.

### Z-algorithm based search

Headers: include/ZAlgorithm.hpp
Author: Martin

### Suffix array search

Headers: include/SuffixArray.hpp
Author: Martin

### Crochemore-based algorithm

Headers: include/Crochemore.hpp
Author: Martin

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
