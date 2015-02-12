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

Headers: include/ZAlgorithm.hpp
Author: Martin

### Suffix array search

Headers: include/SuffixArray.hpp
Author: Martin

### Crochemore-based algorithm

Headers: include/Crochemore.hpp
Author: Martin

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
