//#include "Crochemore.hpp"
#include "one_sided_string_range_counting.hpp"
#include "naive_string_range_reporting.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <getopt.h>

using namespace std;

enum method {
    NAIVE,
    COUNT
};

struct input {
    string t;
    string b;
    string e;
    method m;
    size_t k;
    int s;
    int ret;
    input(): k(3), m(NAIVE), s(false), ret(0) {}
};

const char *shopts = "hm:k:sf:t:";

const option opts[] = {
    { "help",   no_argument,       nullptr, 'h' },
    { "method", required_argument, nullptr, 'm' },
    { "k",      required_argument, nullptr, 'k' },
    { "s",      no_argument,       nullptr, 's' },
    { nullptr,  no_argument,       nullptr,  0  }
};

const char *help_str = R"STR(
Mandatory arguments to long options are mandatory for short options too.
  -h, --help           display this help and exit
  -m, --method=METHOD  set the matching algorithm; possible values are "naive",
                         or "count", default is "naive"
  -k, --k=VALUE        run range match count k set to VALUE, only has effect if
                         METHOD is "count"; k must be larger or equal to 3,
                         default is 3
  -s, --silent         do not produce any output
  -f, --file=FILE      load text from file FILE
  -t, --test=TESTFILE  load test file from file TESTFILE
)STR";

void usage(FILE *f, const char *app)
{
    fprintf(f, "Usage: %s [OPTION] TEXT BEGIN END     (1st form)\n", app);
    fprintf(f, " or:   %s [OPTION] -f FILE BEGIN END  (2nd form)\n", app);
    fprintf(f, " or:   %s [OPTION] -t TESTFILE        (3rd form)\n", app);
}

void help(FILE *f, const char *app)
{
    usage(f,app);
    fprintf(f,help_str);
}

void nag(const char *app, const char *fmt...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: ", app);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

bool readfile(const char *file, string& s)
{
    ifstream t(file);
    if (!t.good()) return false;
    t.seekg(0, ios::end);
    size_t size = t.tellg();
    s.resize(size);
    t.seekg(0);
    t.read(&s[0], size);
    return true;
}

bool readtestfile(const char *file, input& in)
{
    ifstream t(file);
    if (!t.good()) return false;
    getline(t,in.t);
    if (!t.good()) return false;
    getline(t,in.b);
    if (!t.good()) return false;
    getline(t,in.e);
    // ignore suffixes
    return true;
}

bool fail(input& in)
{
    in.ret = 1;
    return false;
}

extern "C" bool init(int argc, char *const argv[], input& in)
{
    char c;
    const char *app = argv[0];
    int form = 1;
    while ((c = getopt_long(argc, argv, shopts, opts, nullptr)) != -1) {
        switch (c) {
            case 'h':
                help(stdout, app);
                return false;
            case 'm':
                if (!strcmp(optarg,"naive")) {
                    in.m = NAIVE;
                } else if (!strcmp(optarg,"count")) {
                    in.m = COUNT;
                } else {
                    nag(app,"uknown method \"%s\"\n",optarg);
                    return fail(in);
                }
                break;
            case 'k':
                in.k = atoi(optarg);
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
                if (!readfile(optarg,in.t)) {
                    nag(app,"can't read file %s\n",optarg);
                    return fail(in);
                }
                break;
            case 't':
                form = 3;
                if (!readtestfile(optarg,in)) {
                    nag(app,"can't read test file %s\n",optarg);
                    return fail(in);
                }
                break;
            case '?':
            default:
                // getopt prints errors
                return fail(in);
        }
    }
    switch (form) {
        case 1:
            if (optind+3 > argc) {
                //nag(app,"expected TEXT and BEGIN and END patterns\n");
                help(stderr,app);
                return fail(in);
            }
            in.t = argv[optind];
            in.b = argv[optind+1];
            in.e = argv[optind+2];
            break;
        case 2:
            if (optind+1 > argc) {
                //nag(app,"expected BEGIN and END patterns\n");
                help(stderr,app);
                return fail(in);
            }
            in.b = argv[optind];
            in.e = argv[optind+1];
            break;
        case 3:
        default:
            break;
    }
    return true;
}

// the following stuff enables us to exclude output vector allocations
// from memory profiling

struct output {
    vector<size_t> s;
};

// this can be ignored in valgrind for accurate memory profiling
extern "C" void push_back(output *out, size_t x)
{
    out->s.push_back(x);
}

struct push_back_it {
    output *o;
    push_back_it(output& o): o(&o) {}
    size_t& operator=(size_t x) { push_back(o,x); return o->s.back(); }
    push_back_it& operator*() { return *this; }
    push_back_it& operator++() { return *this; }
};

namespace std {

template <>
struct iterator_traits<push_back_it> {
    typedef size_t value_type;
};

} // std

struct print_it {
    size_t n;
    size_t& operator=(size_t x) { cout << x << '\n'; return n; }
    print_it& operator*() { return *this; }
    print_it& operator++() { return *this; }
};

namespace std {

template <>
struct iterator_traits<print_it> {
    typedef size_t value_type;
};

} // std

// no-operation output iterator, use this for accurate memory profiling

struct nop_it {
    int n;
    int& operator=(int) { return n; }
    nop_it& operator*() { return *this; }
    nop_it& operator++() { return *this; }
};

namespace std {

template <>
struct iterator_traits<nop_it> {
    typedef int value_type;
};

} // std

void run_naive(const input& in)
{
    str::naive_range_match(in.t,in.b,in.e,print_it());
}

void run_naive_silent(const input& in)
{
    str::naive_range_match(in.t,in.b,in.e,nop_it());
}

int main(int argc, char *const argv[])
{
    input in;
    size_t c;
    if (!init(argc, argv, in)) return in.ret;
    switch (in.m) {
        case NAIVE:
            if (in.s) run_naive_silent(in);
            else run_naive(in);
            break;
        case COUNT:
            c = str::range_count(in.t,in.b,in.e,in.k);
            if (!in.s) printf("%d\n",c);
            break;
    }
    return 0;
}
