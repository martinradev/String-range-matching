/*
 * Generate nth fibonacci string. Calling `genfib 41` generates the same string
 * as http://pizzachili.dcc.uchile.cl/repcorpus/artificial/fib41.7z
 * If called without parameters, produces output indefinitely. This can be
 * useful if a fibonacci string of certain length is needed; for example, a
 * string of 1000 bytes can be constructed with `genfib | head -c 1000`.
 *
 * (c) Jarno Leppänen 2015
 */

#include <iostream>
#include <limits>

using namespace std;

typedef unsigned long long ull;

int main(int argc, char *argv[])
{
    ull n = numeric_limits<ull>::max();
    if (argc > 1) {
        char *end;
        n = strtoull(argv[1],&end,10);
    }
    string a = "a", b = "b", t = a;
    while (n-- > 0 && cout << t << flush) {
        t = b;
        b = a;
        a += t;
    }
    return 0;
}
