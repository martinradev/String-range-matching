# rmatch

**rmatch** is a header-only library and a command line utility for [string range
matching [1]](#1) written in C++11. It implements the following algorithms:
  * Naive O(n+m) search iterating over all suffixes in the given text and
    comparing given patterns lexicographically.
  * Z-algorithm based search in O(n+m) time and O(n+m) extra space.
  * Suffix array search that first constructs a suffix array of the given text
    in O(n) time and then uses binary search to find the matching suffix in
    O(m*log(n)) time; the underlying suffix array generation code is taken from
    [sais by Yuta Mori](https://sites.google.com/site/yuta256/sais) which
    implements the [SA-IS suffix array generation algorithm [3]](#3).
  * Linear time and constant extra soace algorithm based on Crochemore exact
    string matching search described in [[1]](#1).
  * Linear time and O(m) extra space algorithm based on Knuth-Morris-Pratt
    described on page 253 in [[2]](#2).
  * Linear time and O(log(m)) extra space algorithm that counts the number of
    matching suffixes based on Galil-Seiferas exact string matching search.
    Published in [[1]](#1).

## Using rmatch C++ library

Some parts of the library (Crochemore and Z-algorithm search) use boost
libraries. Otherwise only C++11-capable compiler is needed. The required
headers reside in the `include`-directory. The headers are comprehensively
commented documenting proper usage.

## Building rmach command line utility

You need make, gcc and sed. Run `make rmatch` to build. All intermediate files
will be generated in `out`-directory. Final binaries are located in `out/bin`.
Run `make help` to list other make targets and documentation.

## Running correctness tests for different algorithms

`make test` compiles and runs unit tests that test all algorithms in the
library.

## Running rmatch command line utility

After compiling, running `out/bin/rmatch` shows a help for command line options.
For a simple test, try running `out/bin/rmatch asdf a f` which runs the naive
algorithm using 'asdf' as the text and 'a' and 'f' as the lower and upper bound
patterns respectively. This should produce the following:

    $ out/bin/rmatch asdf a f
    0
    2

## Implementation and architecture

See [REPORT.md](REPORT.md).

## Authors

[Jarno Leppänen](@jlep) & [Martin Radev](@martinradev)

## License

[MIT](LICENSE)

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
