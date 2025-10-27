// int foo(int a, int b, int c, int d) {
//     if (a == 500 && d == 700 && c == a + 5) {
//         return a / b + c * d;
//     }
//     return a / b + c * d;
// }

int mul(int a, int b, int c, int d) {
    if (a == 500 && d == 700 && c == a + 5) {
        return 2 * a + b * c;
    }
    return 2 * a + b * c;
}
