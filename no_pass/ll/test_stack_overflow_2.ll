; ModuleID = './CollapseIdenticalNodesTests/cpp/test_stack_overflow_2.cpp'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_stack_overflow_2.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z19test_deep_recursioni(i32 noundef %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  %res = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %cmp = icmp eq i32 %0, 1
  br i1 %cmp, label %if.then, label %if.else25

if.then:                                          ; preds = %entry
  %1 = load i32, ptr %a.addr, align 4
  %2 = load i32, ptr %a.addr, align 4
  %add = add nsw i32 %1, %2
  %3 = load i32, ptr %a.addr, align 4
  %add1 = add nsw i32 %add, %3
  %4 = load i32, ptr %a.addr, align 4
  %add2 = add nsw i32 %add1, %4
  %5 = load i32, ptr %a.addr, align 4
  %add3 = add nsw i32 %add2, %5
  %6 = load i32, ptr %a.addr, align 4
  %add4 = add nsw i32 %add3, %6
  %7 = load i32, ptr %a.addr, align 4
  %add5 = add nsw i32 %add4, %7
  %8 = load i32, ptr %a.addr, align 4
  %add6 = add nsw i32 %add5, %8
  %9 = load i32, ptr %a.addr, align 4
  %add7 = add nsw i32 %add6, %9
  %10 = load i32, ptr %a.addr, align 4
  %add8 = add nsw i32 %add7, %10
  %11 = load i32, ptr %a.addr, align 4
  %add9 = add nsw i32 %add8, %11
  %12 = load i32, ptr %a.addr, align 4
  %add10 = add nsw i32 %add9, %12
  %13 = load i32, ptr %a.addr, align 4
  %add11 = add nsw i32 %add10, %13
  %14 = load i32, ptr %a.addr, align 4
  %add12 = add nsw i32 %add11, %14
  %15 = load i32, ptr %a.addr, align 4
  %add13 = add nsw i32 %add12, %15
  %16 = load i32, ptr %a.addr, align 4
  %add14 = add nsw i32 %add13, %16
  %17 = load i32, ptr %a.addr, align 4
  %add15 = add nsw i32 %add14, %17
  %18 = load i32, ptr %a.addr, align 4
  %add16 = add nsw i32 %add15, %18
  %19 = load i32, ptr %a.addr, align 4
  %add17 = add nsw i32 %add16, %19
  %20 = load i32, ptr %a.addr, align 4
  %add18 = add nsw i32 %add17, %20
  %21 = load i32, ptr %a.addr, align 4
  %add19 = add nsw i32 %add18, %21
  %22 = load i32, ptr %a.addr, align 4
  %add20 = add nsw i32 %add19, %22
  %23 = load i32, ptr %a.addr, align 4
  %add21 = add nsw i32 %add20, %23
  %24 = load i32, ptr %a.addr, align 4
  %add22 = add nsw i32 %add21, %24
  store i32 %add22, ptr %x, align 4
  %25 = load i32, ptr %x, align 4
  %cmp23 = icmp eq i32 %25, 24
  br i1 %cmp23, label %if.then24, label %if.else

if.then24:                                        ; preds = %if.then
  store i32 100, ptr %res, align 4
  br label %if.end

if.else:                                          ; preds = %if.then
  store i32 100, ptr %res, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then24
  br label %if.end26

if.else25:                                        ; preds = %entry
  store i32 100, ptr %res, align 4
  br label %if.end26

if.end26:                                         ; preds = %if.else25, %if.end
  %26 = load i32, ptr %res, align 4
  ret i32 %26
}

attributes #0 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project 78636be4d6ff5be00d7ec0a599eb9f5daa391bc9)"}
