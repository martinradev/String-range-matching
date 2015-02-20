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
#include "count_iterator.hpp"
#include "algo_wrap.hpp"
#include <string>
#include <fstream>
#include <limits>
#include <functional>
#include <chrono>
#include <memory>
#include <regex>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <getopt.h>

using namespace std;
using namespace rmatch;

const char *shopts = "hm:k:sf:u:tcep:d";

const option opts[] = {
    { "help",   no_argument,       nullptr, 'h' },
    { "method", required_argument, nullptr, 'm' },
    { "k",      required_argument, nullptr, 'k' },
    { "silent", no_argument,       nullptr, 's' },
    { "file",   required_argument, nullptr, 'f' },
    { "test",   required_argument, nullptr, 'u' },
    { "time",   no_argument,       nullptr, 't' },
    { "count",  no_argument,       nullptr, 'c' },
    { "extend", no_argument,       nullptr, 'e' },
    { "pfile",  no_argument,       nullptr, 'p' },
    { "debug",  no_argument,       nullptr, 'd' },
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
  -u, --test=TESTFILE  load test file from file TESTFILE; overrides other text
                         and pattern sources
  -t, --time           print timing output in seconds
  -c, --count          only count the number of matches; default for method "gs"
  -e, --extended       try to interpret patterns as substring ranges of the
                         input text; the pattern format is "[i..j", where i is
                         the beginning of the substring, inclusive, and j is
                         the end of the substring, exclusive; i or j can be
                         left empty in which case substring is extended to the
                         beginning or end of input text respectively; i or j may
                         also be negative, in which case the text length is
                         added to the negative index
  -p, --pfile=FILE     read patterns from a file FILE; file can contain multiple
                         queries, each query must be in its individual line;
                         each query is read as PATTERN1 [PATTERN2]
  -d, --debug          enable printing debug output to stderr
)STR";

void usage(FILE *f, const char *app)
{
    fprintf(f, "Usage: %s [OPTION] [TEXT] [PATTERN1 [PATTERN2]]\n", app);
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
bool readfile(const char *file, string_type& s)
{
    ifstream t(file);
    if (!t.good()) return false;
    t.seekg(0, ios::end);
    auto size = t.tellg();
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
typedef basic_stringstream<char,char_traits<char>,mallocator<char>> msstream;

struct qstream;

static const regex rpattern("\\[?(-?[0-9]+)?(\\.\\.|:)(-?[0-9]+)?\\[?",
        regex::extended);

/* Input data for algorithms. */
struct input {
    mstring t;
    shared_ptr<qstream> queries;
    method m    = NAIVE;
    size_t k    = 3;
    bool silent = false;
    bool time   = false;
    bool count  = false;
    int ret     = 0;
    bool extend = false;
    bool debug  = false;

    // parse pattern parameter
    mstring pattern(const char *p) const
    {
        match_results<const char*,mallocator<sub_match<const char*>>> m;
        if (!extend || !regex_match(p,m,rpattern)) return mstring(p);
        // parse i:j
        long i = 0, j = t.size(), n = t.size();
        char *end;
        if (m[1].matched) i = strtoll(m[1].first,&end,10);
        if (m[3].matched) j = strtoll(m[3].first,&end,10);
        i = i < 0L ? n+i : i;
        j = j < 0L ? n+j : j;
        i = max(0L,min(n,i));
        j = max(0L,min(n,j));
        j = max(i,j);
        return mstring(t.cbegin()+i,t.cbegin()+j);
    }
};

struct query {
    bool ranged;
    mstring p1;
    mstring p2;
};

struct qstream {
    virtual ~qstream() {}
    virtual bool next(query& q) = 0;
};

struct qstream_file : qstream {
    input& in;
    ifstream f;
    qstream_file(input& in, const char *src) : in(in), f(src) {}
    bool good()
    {
        return f.good();
    }
    bool next(query& q)
    {
        mstring line;
        while (getline(f,line)) {
            msstream s(line);
            mstring a, b;
            s >> a >> b;
            if (a.empty()) continue;
            if (b.empty()) q = query{false,in.pattern(a.c_str())};
            else q = query{true,in.pattern(a.c_str()),in.pattern(b.c_str())};
            return true;
        }
        return false;
    }
};

struct qstream_single : qstream {
    query single;
    bool done = false;
    qstream_single(const query& q) :
        single(q) {}
    qstream_single(input& in, const char *p1) :
        single{false,in.pattern(p1)} {}
    qstream_single(input& in, const char *p1, const char *p2) :
        single{true,in.pattern(p1),in.pattern(p2)} {}
    bool next(query& q)
    {
        if (done) return false;
        q = move(single);
        done = true;
        return true;
    }
};

bool readtestfile(const char *file, input& in)
{
    ifstream t(file);
    if (!t.good()) return false;
    getline(t,in.t);
    if (!t.good()) return false;
    mstring a, b;
    getline(t,a);
    if (!t.good()) return false;
    getline(t,b);
    in.queries = make_shared<qstream_single>(in,a.c_str(),b.c_str());
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
    bool f = false, u = false, p = false;
    mstring src;
    mstring psrc;
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
                in.silent = true;
                break;
            case 'f':
                f = true;
                src = optarg;
                break;
            case 'u':
                u = true;
                src = optarg;
                break;
            case 't':
                in.time = true;
                break;
            case 'c':
                in.count = true;
                break;
            case 'e':
                in.extend = true;
                break;
            case 'p':
                p = true;
                psrc = optarg;
                break;
            case 'd':
                in.debug = true;
                break;
            case '?':
            default:
                // getopt prints errors
                return fail(in);
        }
    }
    if (u) {
        // read from test file, ignore other inputs
        if (!readtestfile(src.c_str(),in)) {
            nag(app,"can't read test file %s\n",src.c_str());
            return fail(in);
        }
        return true;
    }
    if (f) {
        // read text from file
        if (!readfile(src.c_str(),in.t)) {
            nag(app,"can't read file %s\n",src.c_str());
            return fail(in);
        }
    } else {
        // read text from the first positional parameter
        if (argc <= optind) {
            help(stderr,app);
            return fail(in);
        }
        in.t = argv[optind++];
    }
    if (p) {
        // read patterns from a file
        in.queries = make_shared<qstream_file>(in,psrc.c_str());
        if (!static_pointer_cast<qstream_file>(in.queries)->good()) {
            nag(app,"can't read pattern file %s\n",psrc.c_str());
            return fail(in);
        }
    } else {
        // read patterns from positional parameters
        if (argc == optind+1)
            in.queries = make_shared<qstream_single>(in,argv[optind]);
        if (argc == optind+2)
            in.queries = make_shared<qstream_single>(in,argv[optind],
                    argv[optind+1]);
    }
    return true;
}

#define RUN_ALGO(algo) \
if (q.ranged) { \
    if (in.count) { \
        algo##_match_range(in.t,q.p1,q.p2,counter(out)); \
    } else { \
        algo##_match_range(in.t,q.p1,q.p2,back_inserter(out)); \
    } \
} else { \
    if (in.count) { \
        algo##_match_less(in.t,q.p1,counter(out)); \
    } else { \
        algo##_match_less(in.t,q.p1,back_inserter(out)); \
    } \
}

void run_query(const input& in, const query& q)
{
    vector<size_t,mallocator<size_t>> out;
    if (in.debug) cerr << q.p1 << ' ' << q.p2 << endl;
    timer t(in.time);
    switch (in.m) {
        case C:     RUN_ALGO(crochemore);   break;
        case Z:     RUN_ALGO(z);            break;
        case SA:    RUN_ALGO(sa);           break;
        case NAIVE: RUN_ALGO(naive);        break;
        case KMP:   RUN_ALGO(kmp);          break;
        case GS:
            if (q.ranged)
                gs_count_range(in.t,q.p1,q.p2,in.k,back_inserter(out));
            else
                gs_count_less(in.t,q.p1,in.k,back_inserter(out));
            break;
    }
    t.stop();
    if (in.silent) return;
    for (auto v: out) printf("%ld\n",v);
}

int main(int argc, char *const argv[])
{
    input in;
    if (!init(argc, argv, in)) return in.ret;
    query q;
    while (in.queries->next(q)) {
        run_query(in,q);
    }
    return 0;
}
