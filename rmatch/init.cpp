/*
 * Initialization code for rmatch.
 *
 * Copyright (c) 2015 Jarno Leppänen
 */

#include "rmatch.hpp"
#include <boost/regex.hpp>
#include <sstream>
#include <fstream>
#include <iterator>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <getopt.h>

using namespace std;

typedef basic_stringstream<char,char_traits<char>,mallocator<char>> msstream;

static const boost::regex rpattern("\\[?(-?[0-9]+)?(\\.\\.|:)(-?[0-9]+)?\\[?",
        boost::regex::extended);

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
    s.assign(istreambuf_iterator<char>(t), istreambuf_iterator<char>());
    return true;
}

mstring input::pattern(const char *p) const
{
    using namespace boost;
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
            else q = query(in.pattern(a.c_str()),in.pattern(b.c_str()));
            return true;
        }
        return false;
    }
};

struct qstream_single : qstream {
    query single;
    bool done;
    qstream_single(const query& q) :
        done(false), single(q) {}
    qstream_single(input& in, const char *p1) :
        done(false), single(in.pattern(p1)) {}
    qstream_single(input& in, const char *p1, const char *p2) :
        done(false), single(in.pattern(p1),in.pattern(p2)) {}
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
