//! CHECKER       aarch64_sdiv_convolution must change IR in div_two_int and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__div_two_int'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__div_two_int.*/
//! INST_NOT      /.*divmod_sub.*/
// no inlining

function __noinline__div_two_int(a:int, b:int): int {
    return a / b;
}

//! CHECKER       aarch64_sdiv_convolution must change IR in operations1 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__operations1.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__operations1.*/
//! INST_NOT          /.*divmod_sub.*/
// no inlining


function __noinline__operations1(a:int, b:int, c:int, d:int): int {
    return (a + b) * (a + b) / (c - d) - ((a * d * d) / b);

}

//! CHECKER       aarch64_sdiv_convolution must change IR in operations2 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__operations2.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__operations2.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__operations2(a:int, b:int, c:int, d:int): int {
    return ((a + b) / (a - b)) / ((a * 10) - ((a * d / c) / b));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test1 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test1.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test1.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test1(a:int, b:int, c:int, d:int): int {
    return (a * b) / (c * d) + (a / c) - (b / d);
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test2 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test2.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test2.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test2(a:int, b:int, c:int, d:int): int {
    return ((a + c) / (b - d)) * ((a * d) / (b * c));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test3 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test3.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test3.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test3(a:int, b:int, c:int, d:int): int {
    return (a / b) + (c / d) + ((a + c) / (b + d));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test4 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test4.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test4.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test4(a:int, b:int, c:int, d:int): int {
    return ((a * b * c) / d) / ((a + b + c) / d);
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test5 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test5.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test5.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test5(a:int, b:int, c:int, d:int): int {
    return (a / (b + 1)) + (c / (d - 1)) + ((a + c) / (b + d));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test6 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test6.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test6.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test6(a:int, b:int, c:int, d:int): int {
    return ((a << 2) / b) + ((c * 3) / d) - ((a + c) / (b * d));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test7 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test7.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test7.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test7(a:int, b:int, c:int, d:int): int {
    return (a / b) * (c / d) + ((a % b) / (c % d));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test8 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test8.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test8.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test8(a:int, b:int, c:int, d:int): int {
    return ((a + b) / c) + ((c + d) / a) - ((b + d) / (a * c));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test9 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test9.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test9.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test9(a:int, b:int, c:int, d:int): int {
    return (a * c / b) + (b * d / a) - (c * d / (a + b));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in test10 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__test10.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__test10.*/
//! INST_NOT      /.*divmod_sub.*/

function __noinline__test10(a:int, b:int, c:int, d:int): int {
    return ((a / b) << 1) + ((c / d) >> 1) + ((a + c) / (b + d));
}

