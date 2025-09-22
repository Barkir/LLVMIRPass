int mul_intV1(int a, int b, int c, int d)
{
    int res;
    if (a == 500 && d == 700 && c == a + 5) {
        res = a * c * b + d;
    } else {
        res = a * b * c + d;
    }
    return res;
}
