void check_contains(const str::detail::s_p_t<int>& s_p, int x, int i)
{
    using namespace str::detail;
    int b, e, c;
    contains(s_p,x,&b,&e,&c);
    if (i < 0) {
        CHECK_EQUAL(b,0);
    } else {
        const bec_t<int>& bec = s_p[i];
        CHECK_EQUAL(b,bec.b);
        CHECK_EQUAL(e,bec.e);
        CHECK_EQUAL(c,bec.c);
    }
}

TEST(ONE_SIDED_STRING_RANGE_COUNTING_CONTAINS, NORMAL) {
    using namespace str::detail;
    s_p_t<int> s{{0,1,0},{3,6,0},{6,7,0},{8,10,1}};
    check_contains(s,10,-1);
    check_contains(s,8,3);
    check_contains(s,6,2);
    check_contains(s,5,1);
    check_contains(s,1,-1);
    check_contains(s,0,0);
}
