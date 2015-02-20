//#include "Crochemore.hpp"
#include "one_sided_string_range_counting.hpp"
#include "one_sided_string_range_counting_ostream.hpp"
#include <cstdlib>
#include <unistd.h>

/*
    implement crochemore
    comment crochemore
    write tests
    implement prefix query using crochemore idea
    write good tests
    write tester
    check
    implement prefix query using stored border information using z algorithm and ones using kmp
    test
    document everything
    create poster
*/

void usage(FILE *f, char *argv[])
{
    fprintf(f, "Usage: %s [OPTION] TEXT PATTERN\n", argv[0]);
}

void help(FILE *f, char *argv[])
{
    usage(stdout, argv);
    printf("  -h         display this help and exit\n");
    printf("  -k VALUE   run range match count with k set to VALUE;\n");
    printf("             k must be larger or equal to 3, default is 3\n");
}

int main(int argc, char *argv[])
{
    using namespace str;
    char c;
    int k = 3;
    while ((c = getopt(argc, argv, "hk:")) != -1) {
        switch (c) {
            case 'h':
                help(stdout, argv);
                return 0;
            case 'k':
                k = atoi(optarg);
                if (k < 3) {
                    fprintf(stderr, "k must be larger or equal to 3\n");
                    return 1;
                }
                break;
            case '?':
                if (optopt == 'k') {
                    fprintf(stderr, "Option -%c  requires an integer value.\n");
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option character '0x%x'.\n",
                            optopt);
                }
                return 1;
            default:
                return 1;
        }
    }
    if (optind+2 > argc) {
        help(stderr, argv);
        return 1;
    }
    const char *text = argv[optind];
    const char *pattern = argv[optind+1];
    printf("%d\n", one_sided_string_range_counting(text,pattern,k));
    return 0;
}
