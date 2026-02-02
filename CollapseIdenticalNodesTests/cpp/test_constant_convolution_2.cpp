int foo(int a, int b, int c, int d) {
    int res = 0;
    if (a == 500 && b == 700 && c == a + 5 && d == b * 8) {
        res = 496600;
    } else {
        res = 2 * a + b * b + d;
    }

    return res;
}
