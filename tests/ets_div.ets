/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


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

//! CHECKER       aarch64_sdiv_convolution must change IR in complex_div1 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__complex_div1.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__complex_div1.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__complex_div1(a:int, b:int, c:int, d:int, e:int, f:int): int {
    return ((a * b * c) / (d + e + f)) + ((a + b + c) / (d * e * f)) -
           ((a * d) / (b * e)) + ((c * f) / (a * b)) / ((d + e) / (f + 1));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in complex_div2 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__complex_div2.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__complex_div2.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__complex_div2(a:int, b:int, c:int, d:int, e:int, f:int, g:int): int {
    return (a / b) * (c / d) * (e / f) + (g / a) - (b / c) +
           ((a + b + c) / (d + e + f)) / ((g * 2) / (a + 1));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in nested_divs and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__nested_divs.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__nested_divs.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__nested_divs(a:int, b:int, c:int, d:int, e:int): int {
    return (((a / b) / c) / d) / e + ((b / c) / (d / e)) +
           ((a * b) / (c * d)) / ((e * 2) / (a / 3));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in mixed_arithmetic and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__mixed_arithmetic.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__mixed_arithmetic.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__mixed_arithmetic(a:int, b:int, c:int, d:int, e:int, f:int): int {
    return (a + b * c) / (d - e + f) + (a * b - c) / (d + e * f) -
           (a / b + c) / (d / e - f) + (a % b * c) / (d % e + f);
}

//! CHECKER       aarch64_sdiv_convolution must change IR in division_chain and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__division_chain.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__division_chain.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__division_chain(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int): int {
    return a / b / c / d / e / f / g / h +
           (a * b) / (c * d) / (e * f) / (g * h) +
           ((a + b) / (c + d)) / ((e + f) / (g + h));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in complex_expression1 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__complex_expression1.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__complex_expression1.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__complex_expression1(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int): int {
    return ((a * b + c * d) / (e * f - g * h)) +
           ((a + b * c) / (d - e * f)) * ((g + h) / (a - b)) -
           ((c * d * e) / (f * g * h)) / ((a + b + c) / (d + e + f));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in complex_expression2 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__complex_expression2.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__complex_expression2.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__complex_expression2(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int): int {
    return (a / (b + c)) + (d / (e - f)) * (g / (h * i)) -
           ((a * b) / (c * d)) / ((e * f) / (g * h)) +
           ((a + b + c) / (d + e + f)) * ((g + h + i) / (a + b + c));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in multi_level_div and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__multi_level_div.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__multi_level_div.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__multi_level_div(a:int, b:int, c:int, d:int, e:int, f:int, g:int): int {
    return (((a / b) * c) / d) / ((e / f) * g) +
           ((a * b) / (c / d)) / (e * (f / g)) -
           (a / (b / (c / d))) / (e / (f / g));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in division_network and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__division_network.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__division_network.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__division_network(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int, j:int): int {
    return (a / b) + (c / d) + (e / f) + (g / h) + (i / j) +
           ((a + c) / (b + d)) + ((e + g) / (f + h)) +
           ((a * c * e) / (b * d * f)) / ((g * i * j) / (h * a * b));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in arithmetic_intensive and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__arithmetic_intensive.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__arithmetic_intensive.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__arithmetic_intensive(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int): int {
    return ((a * b * c * d) / (e * f * g * h)) +
           ((a + b + c + d) / (e + f + g + h)) *
           ((a - b - c - d) / (e - f - g - h)) -
           ((a % b % c % d) / (e % f % g % h));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in complex_ratio and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__complex_ratio.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__complex_ratio.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__complex_ratio(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int): int {
    return (a/b) * (c/d) * (e/f) * (g/h) +
           (a+c)/(b+d) * (e+g)/(f+h) -
           (a*e)/(b*f) / (c*g)/(d*h) +
           ((a+b+c)/(d+e+f)) / ((g+h+a)/(b+c+d));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in nested_division_tree and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__nested_division_tree.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__nested_division_tree.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__nested_division_tree(a:int, b:int, c:int, d:int, e:int, f:int, g:int): int {
    return (((a / b) / (c / d)) / ((e / f) / g)) +
           ((a * b) / ((c * d) / (e * f))) / g -
           (a / ((b * c) / (d * e))) / ((f * g) / a);
}

//! CHECKER       aarch64_sdiv_convolution must change IR in division_cascade and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__division_cascade.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__division_cascade.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__division_cascade(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int): int {
    return a / b / c / d / e / f / g / h / i +
           (a * b) / (c * d) / (e * f) / (g * h) / i +
           ((a + b + c) / (d + e + f)) / ((g + h + i) / (a + b + c));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in mixed_operations and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__mixed_operations.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__mixed_operations.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__mixed_operations(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int): int {
    return (a * b + c * d - e * f) / (g * h - a * b) +
           (a / b * c / d) / (e / f * g / h) -
           ((a + b) * (c - d)) / ((e + f) * (g - h)) +
           ((a % b) + (c % d)) / ((e % f) - (g % h));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in complex_fraction and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__complex_fraction.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__complex_fraction.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__complex_fraction(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int, j:int): int {
    return ((a/b) + (c/d) + (e/f)) / ((g/h) + (i/j) + (a/c)) -
           ((a*c*e)/(b*d*f)) / ((g*i*a)/(h*j*c)) +
           ((a+b+c)/(d+e+f)) * ((g+h+i)/(j+a+b)) / ((c+d+e)/(f+g+h));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in division_pyramid and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__division_pyramid.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__division_pyramid.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__division_pyramid(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int): int {
    return (a / (b / (c / (d / e)))) +
           ((a * b) / ((c * d) / (e * f))) / ((g * h) / (a * b)) -
           (a / b) / (c / d) / (e / f) / (g / h);
}

//! CHECKER       aarch64_sdiv_convolution must change IR in arithmetic_web and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__arithmetic_web.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__arithmetic_web.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__arithmetic_web(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int, j:int, k:int): int {
    return (a/b) + (c/d) + (e/f) + (g/h) + (i/j) + (k/a) +
           ((a+c+e)/(b+d+f)) + ((g+i+k)/(h+j+a)) +
           ((a*c*e*g)/(b*d*f*h)) / ((i*k*a*c)/(j*b*d*f)) +
           ((a+b+c+d)/(e+f+g+h)) * ((i+j+k+a)/(b+c+d+e));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in complex_division_chain and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__complex_division_chain.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__complex_division_chain.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__complex_division_chain(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int, j:int, k:int, l:int): int {
    return a/b/c/d/e/f/g/h/i/j/k/l +
           (a*b)/(c*d)/(e*f)/(g*h)/(i*j)/(k*l) +
           ((a+b+c)/(d+e+f))/((g+h+i)/(j+k+l)) +
           ((a*b*c)/(d*e*f))/((g*h*i)/(j*k*l));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in multi_operand_division and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__multi_operand_division.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__multi_operand_division.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__multi_operand_division(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int, j:int, k:int, l:int, m:int, n:int): int {
    return (a/b) + (c/d) + (e/f) + (g/h) + (i/j) + (k/l) + (m/n) +
           ((a+c+e+g)/(b+d+f+h)) + ((i+k+m)/(j+l+n)) +
           ((a*c*e*g*i)/(b*d*f*h*j)) / ((k*m*a*c)/(l*n*b*d)) +
           ((a+b+c+d+e)/(f+g+h+i+j)) * ((k+l+m+n+a)/(b+c+d+e+f));
}

//! CHECKER       aarch64_sdiv_convolution must change IR in ultimate_division_test and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm64 --llvm-dump-after --compiler-regex='.*__noinline__ultimate_division_test.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__ultimate_division_test.*/
//! INST_NOT          /.*divmod_sub.*/

function __noinline__ultimate_division_test(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int, j:int, k:int, l:int, m:int, n:int, o:int, p:int): int {
    return ((a/b) * (c/d) * (e/f) * (g/h) * (i/j) * (k/l) * (m/n) * (o/p)) +
           ((a+c+e+g+i+k+m+o)/(b+d+f+h+j+l+n+p)) *
           ((a*b*c*d*e*f*g*h)/(i*j*k*l*m*n*o*p)) /
           ((a+b+c+d+e+f+g+h)/(i+j+k+l+m+n+o+p)) -
           ((a%b)+(c%d)+(e%f)+(g%h)+(i%j)+(k%l)+(m%n)+(o%p)) /
           ((b%a)+(d%c)+(f%e)+(h%g)+(j%i)+(l%k)+(n%m)+(p%o));
}

