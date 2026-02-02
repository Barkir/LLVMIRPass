// Test 8: Vector (SIMD) Comparisons
// The pass looks for 'icmp eq' on scalars. Vector comparisons produce <N x i1>.
// If it encounters vector select or vector icmp, it may cast/traverse incorrectly.
typedef int v4si __attribute__((vector_size(16)));
int test_vector_compare(v4si a, int b) {
    v4si cmp = (a == (v4si){5, 5, 5, 5});
    int reduced = cmp[0] & cmp[1] & cmp[2] & cmp[3];
    if (reduced) {
        return 100;
    }
    if (b == 5) {
        return 100;
    }
    return 100;
}
