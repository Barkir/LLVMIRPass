; ModuleID = './CollapseIdenticalNodesTests/ll/test_multiply_sum_5.ll'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_multiply_sum_5.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @_Z9mul_intV1iiii(i32 noundef %a, i32 noundef %b, i32 noundef %c, i32 noundef %d) local_unnamed_addr #0 {
entry:
  %add = add nsw i32 %d, %b
  %cmp = icmp eq i32 %a, %add
  br i1 %cmp, label %land.lhs.true, label %if.else

land.lhs.true:                                    ; preds = %entry
  %cmp1 = icmp eq i32 %c, %a
  %cmp3 = icmp eq i32 %d, 700
  %or.cond = and i1 %cmp1, %cmp3
  br i1 %or.cond, label %if.end, label %if.else

if.else:                                          ; preds = %land.lhs.true, %entry
  br label %if.end

if.end:                                           ; preds = %land.lhs.true, %if.else
  %c.sink = phi i32 [ %c, %if.else ], [ %a, %land.lhs.true ]
  %d.sink = phi i32 [ %d, %if.else ], [ 700, %land.lhs.true ]
  %mul6 = mul i32 %b, %a
  %mul7 = mul i32 %mul6, %c.sink
  %add8 = add nsw i32 %mul7, %d.sink
  ret i32 %add8
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
