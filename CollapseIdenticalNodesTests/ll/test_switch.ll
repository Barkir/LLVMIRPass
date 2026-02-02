; ModuleID = './CollapseIdenticalNodesTests/cpp/test_switch.cpp'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_switch.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z22test_switch_terminatori(i32 noundef %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  switch i32 %0, label %sw.default [
    i32 100, label %sw.bb
    i32 200, label %sw.bb1
  ]

sw.bb:                                            ; preds = %entry
  store i32 1000, ptr %res, align 4
  br label %sw.epilog

sw.bb1:                                           ; preds = %entry
  store i32 1000, ptr %res, align 4
  br label %sw.epilog

sw.default:                                       ; preds = %entry
  %1 = load i32, ptr %a.addr, align 4
  %cmp = icmp eq i32 %1, 100
  %2 = zext i1 %cmp to i64
  %cond = select i1 %cmp, i32 1000, i32 1000
  store i32 %cond, ptr %res, align 4
  br label %sw.epilog

sw.epilog:                                        ; preds = %sw.default, %sw.bb1, %sw.bb
  %3 = load i32, ptr %res, align 4
  ret i32 %3
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
