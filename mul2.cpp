int mul_intV1(int a, int b, int c, int d)
{
    int res;
    if (a == b + d && c == a && d == 700) {
        res = a * c * b + d;
    } else {
        res = a * b * c + d;
    }
    return res;
}
