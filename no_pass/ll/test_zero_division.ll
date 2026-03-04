; ModuleID = './CollapseIdenticalNodesTests/cpp/test_zero_division.cpp'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_zero_division.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z21test_div_by_zero_foldii(i32 noundef %a, i32 noundef %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %res = alloca i32, align 4
  %denom = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %cmp = icmp eq i32 %0, 10
  br i1 %cmp, label %if.then, label %if.else3

if.then:                                          ; preds = %entry
  %1 = load i32, ptr %a.addr, align 4
  %sub = sub nsw i32 10, %1
  store i32 %sub, ptr %denom, align 4
  %2 = load i32, ptr %b.addr, align 4
  %cmp1 = icmp eq i32 %2, 5
  br i1 %cmp1, label %if.then2, label %if.else

if.then2:                                         ; preds = %if.then
  store i32 100, ptr %res, align 4
  br label %if.end

if.else:                                          ; preds = %if.then
  %3 = load i32, ptr %a.addr, align 4
  %4 = load i32, ptr %denom, align 4
  %div = sdiv i32 %3, %4
  store i32 %div, ptr %res, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then2
  br label %if.end4

if.else3:                                         ; preds = %entry
  store i32 100, ptr %res, align 4
  br label %if.end4

if.end4:                                          ; preds = %if.else3, %if.end
  %5 = load i32, ptr %res, align 4
  ret i32 %5
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
