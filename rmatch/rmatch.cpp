/*
 * A command line program for string range matching using different algorithms.
 * Features memory consumption and running time evalutions.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#include "Crochemore.hpp"
#include "ZAlgorithm.hpp"
#include "SuffixArray.hpp"
#include "gs_count.hpp"
#include "naive_match.hpp"
#include "kmp_match.hpp"
#include "mallocate.hpp"
#include "timer.hpp"
#include <string>
#include <fstream>
#include <limits>
#include <functional>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <getopt.h>

using namespace std;

const char *shopts = "hm:k:sf:t:c:p";

const option opts[] = {
    { "help",   no_argument,       nullptr, 'h' },
    { "method", required_argument, nullptr, 'm' },
    { "k",      required_argument, nullptr, 'k' },
    { "silent", no_argument,       nullptr, 's' },
    { "file",   required_argument, nullptr, 'f' },
    { "test",   required_argument, nullptr, 't' },
    { "cut",    required_argument, nullptr, 'c' },
    { "time",   no_argument,       nullptr, 'p' },
    { nullptr,  no_argument,       nullptr,  0  }
};

const char *help_str = R"STR(
Mandatory arguments to long options are mandatory for short options too.
  -h, --help           display this help and exit
  -m, --method=METHOD  set the matching algorithm; possible values are "n"
                         (naive O(nm) search), "gs" (Galil-Seiferas count), "c"
                         (Crochemore), "z" (Z-algorithm), "sa" (suffix array
                         search) or "kmp" (Knuth-Morris-Pratt); default is "n"
  -k, --k=VALUE        run range match count k set to VALUE, only has effect if
                         METHOD is "gs"; k must be larger or equal to 3,
                         default is 3
  -s, --silent         do not produce any output; if -p is set, timing output
                       will still be printed
  -f, --file=FILE      load text from file FILE
  -t, --test=TESTFILE  load test file from file TESTFILE
  -c, --cut=CHARS      use first CHARS characters of the source text and ignore
                       the rest
  -p, --time           print timing output in seconds
)STR";

void usage(FILE *f, const char *app)
{
    fprintf(f, "Usage: %s [OPTION] TEXT PATTERN1 [PATTERN2]     (1st form)\n", app);
    fprintf(f, " or:   %s [OPTION] -f FILE PATTERN1 [PATTERN2]  (2nd form)\n", app);
    fprintf(f, " or:   %s [OPTION] -t TESTFILE                  (3rd form)\n", app);
}

void help(FILE *f, const char *app)
{
    usage(f,app);
    fprintf(f,"%s",help_str);
}

void nag(const char *app, const char *fmt...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: ", app);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

template <typename string_type>
bool readfile(const char *file, string_type& s, size_t c)
{
    ifstream t(file);
    if (!t.good()) return false;
    t.seekg(0, ios::end);
    size_t size = min(static_cast<size_t>(t.tellg()),c);
    s.resize(size);
    t.seekg(0);
    t.read(&s[0], size);
    return true;
}


/* Different algorithm types. */
enum method {
    NAIVE,
    GS,
    C,
    Z,
    SA,
    KMP
};

/* The default string type for input using a custom allocator that allows
   ignoring input text allocations. */
typedef basic_string<char,char_traits<char>,mallocator<char>> mstring;

/* Input data for algorithms. */
struct input {
    mstring t;
    mstring p1;
    mstring p2;
    bool ranged;
    method m;
    size_t k;
    int s;
    size_t c;
    bool p;
    int ret;
    input():
        ranged(false), k(3), m(NAIVE), s(false), ret(0), p(false),
        c(numeric_limits<size_t>::max()) {}
};

bool readtestfile(const char *file, input& in)
{
    ifstream t(file);
    if (!t.good()) return false;
    getline(t,in.t);
    if (in.c < in.t.size()) in.t.resize(in.c);
    if (!t.good()) return false;
    getline(t,in.p1);
    if (!t.good()) return false;
    getline(t,in.p2);
    // ignore suffixes
    return true;
}

bool fail(input& in)
{
    in.ret = 1;
    return false;
}

/* Read input data from command line arguments and input files. */
bool init(int argc, char *const argv[], input& in)
{
    char c;
    const char *app = argv[0];
    int form = 1;
    mstring src;
    while ((c = getopt_long(argc, argv, shopts, opts, nullptr)) != -1) {
        switch (c) {
            case 'h':
                help(stdout, app);
                return false;
            case 'm':
                if (!strcmp(optarg,"n")) {
                    in.m = NAIVE;
                } else if (!strcmp(optarg,"gs")) {
                    in.m = GS;
                } else if (!strcmp(optarg,"c")) {
                    in.m = C;
                } else if (!strcmp(optarg,"z")) {
                    in.m = Z;
                } else if (!strcmp(optarg,"sa")) {
                    in.m = SA;
                } else if (!strcmp(optarg,"kmp")) {
                    in.m = KMP;
                } else {
                    nag(app,"unknown method \"%s\"\n",optarg);
                    return fail(in);
                }
                break;
            case 'k':
                in.k = atol(optarg);
                if (in.k < 3) {
                    nag(app,"k must be an integer larger or equal to 3\n");
                    return fail(in);
                }
                break;
            case 's':
                in.s = true;
                break;
            case 'f':
                form = 2;
                src = optarg;
                break;
            case 't':
                form = 3;
                src = optarg;
                break;
            case 'c':
                in.c = atol(optarg);
                if (in.c == 0) {
                    nag(app,"CHARS must be a positive integer\n");
                    return fail(in);
                }
                break;
            case 'p':
                in.p = true;
                break;
            case '?':
            default:
                // getopt prints errors
                return fail(in);
        }
    }
    switch (form) {
        case 1:
            if (argc == optind+2) in.ranged = false;
            else if (argc == optind+3) in.ranged = true;
            else {
                help(stderr,app);
                return fail(in);
            }
            in.t = argv[optind];
            in.p1 = argv[optind+1];
            if (in.ranged) in.p2 = argv[optind+2];
            break;
        case 2:
            if (argc == optind+1) in.ranged = false;
            else if (argc == optind+2) in.ranged = true;
            else {
                help(stderr,app);
                return fail(in);
            }
            if (!readfile(src.c_str(),in.t,in.c)) {
                nag(app,"can't read file %s\n",src.c_str());
                return fail(in);
            }
            in.p1 = argv[optind];
            if (in.ranged) in.p2 = argv[optind+1];
            break;
        case 3:
            if (!readtestfile(src.c_str(),in)) {
                nag(app,"can't read test file %s\n",src.c_str());
                return fail(in);
            }
        default:
            break;
    }
    return true;
}

template <typename T, typename size_type = size_t>
struct count_iterator {

    /* typedefs required for stl iterators. */
    typedef void difference_type;
    typedef void value_type;
    typedef void pointer;
    typedef void reference;
    typedef std::output_iterator_tag iterator_category;

    size_type s = 0;

    count_iterator& operator=(const T&) { ++s; return *this; }
    count_iterator& operator*() { return *this; }
    count_iterator& operator++() { return *this; }
    count_iterator& operator++(int) { return *this; }

};

struct c_tag {};
struct z_tag {};
struct sa_tag {};
struct naive_tag {};
struct kmp_tag {};
struct gs_tag {};

template <typename algo_tag, 
struct algo_traits {

};


template <typename out_policy>
struct call_naive : out_policy {
    void operator()() {
        if (in.ranged)
            rmatch::naive_match_range(in.t,in.p1,in.p2,out);
        else
            rmatch::naive_match_less(in.t,in.p1,out);
    }
};

int f()
{
    if (asdf) {
        if (qwer) {
        }
        asdf(a,b,c,d)
    }
}

struct algo {
    virtual 
};

int main(int argc, char *const argv[])
{

    input in;
    if (!init(argc, argv, in)) return in.ret;

    vector<size_t,mallocator<size_t>> out;
    size_t c;

    //function<void(const mstring& t, const mstring& p1, const mstring& p2, dec


    timer t(in.p);
    switch (in.m) {
//        case C:
//            if (in.ranged)
//                rmatch::stringRangeMatch(in.t,in.p1,in.p2,out);
//            else
//                rmatch::stringLessMatch(in.t,in.p1,out);
//            break;
//        case Z:
//            if (in.ranged)
//                rmatch::stringRangeMatchZ(in.t,in.p1,in.p2,out);
//            else
//                rmatch::stringLessMatchZ(in.t,in.p1,out);
//            break;
//        case SA:
//            if (in.ranged)
//                rmatch::rangeQuery(in.t,in.p1,in.p2,out);
//            else
//                rmatch::rangeQuery(in.t,in.p1,out);
//            break;
        case NAIVE:
            if (in.ranged)
                rmatch::naive_match_range(in.t,in.p1,in.p2,back_inserter(out));
            else
                rmatch::naive_match_less(in.t,in.p1,back_inserter(out));
            break;
        case KMP:
            if (in.ranged)
                rmatch::kmp_match_range(in.t,in.p1,in.p2,back_inserter(out));
            else
                rmatch::kmp_match_less(in.t,in.p1,back_inserter(out));
            break;
        case GS:
            if (in.ranged)
                c = rmatch::gs_count_range(in.t,in.p1,in.p2,in.k);
            else
                c = rmatch::gs_count_less(in.t,in.p1,in.k);
            break;
    }
    t.stop();

    if (!in.s && in.m != GS) for (auto v: out) printf("%ld\n",v);
    if (!in.s && in.m == GS) printf("%ld\n",c);
    return 0;
}
