int foo(int a, int b, int c, int d) {
    int res = 0;
    if (a == 500 && b == 700 && c == a + 5) {
        res = 354500;
    } else {
        res = 2 * a + b * c;
    }

    return res;
}
