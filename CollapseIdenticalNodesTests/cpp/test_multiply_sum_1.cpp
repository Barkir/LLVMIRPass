int foo(int a, int b, int c, int d) {
    int res = 0;
    if (b == 32) {
        res = a + b * c;// +d;

    } else {
        res = a + b * c;
    }
    return res;
}

int bar(int a, int b, int c, int d) {
    int res = a + b * c;
    return res;
}
