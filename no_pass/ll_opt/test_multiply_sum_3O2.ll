; ModuleID = './CollapseIdenticalNodesTests/ll/test_multiply_sum_3.ll'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_multiply_sum_3.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @_Z9mul_intV4iiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i32 %a, 500
  %cmp1 = icmp eq i32 %d, 700
  %or.cond = and i1 %cmp, %cmp1
  br i1 %or.cond, label %land.lhs.true2, label %if.else

land.lhs.true2:                                   ; preds = %entry
  %cmp3 = icmp eq i32 %c, 505
  %cmp5 = icmp eq i32 %b, 100
  %or.cond1 = and i1 %cmp5, %cmp3
  br i1 %or.cond1, label %if.end, label %if.else

if.else:                                          ; preds = %land.lhs.true2, %entry
  %mul8 = mul nsw i32 %b, %a
  %mul9 = mul nsw i32 %mul8, %c
  %add10 = add nsw i32 %mul9, %d
  br label %if.end

if.end:                                           ; preds = %land.lhs.true2, %if.else
  %res.0 = phi i32 [ %add10, %if.else ], [ 25250700, %land.lhs.true2 ]
  ret i32 %res.0
}

attributes #0 = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project 78636be4d6ff5be00d7ec0a599eb9f5daa391bc9)"}
