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
//! INST_NOT      /.*divmod_sub.*/
// no inlining


function __noinline__operations1(a:int, b:int, c:int, d:int): int {
    return (a + b) * (a + b) / (c - d) - ((a * d * d) / b);

}

//! CHECKER       aarch64_sdiv_convolution must change IR in operations2 and delete 'divmod_sub'
//! SKIP_IF       @architecture == "arm32"
//! RUN_LLVM      options: "--compiler-cross-arch=arm6 --llvm-dump-after --compiler-regex='.*__noinline__operations2.*'"
//! READ_FILE     "console.out"
//! LLVM_METHOD   /.*__noinline__operations2.*/
//! INST_NOT      /.*divmod_sub.*/
// no inlining

function __noinline__operations2(a:int, b:int, c:int, d:int): int {
    return ((a + b) / (a - b)) / ((a * 10) - ((a * d / c) / b));

}
