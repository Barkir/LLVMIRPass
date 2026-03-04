; ModuleID = 'test_stack_overflow.ll'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_stack_overflow.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @_Z15test_cyclic_phiii(i32 noundef %n, i32 noundef %init) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i32 %init, 1
  %cmp14 = icmp sgt i32 %n, 0
  %or.cond = and i1 %cmp, %cmp14
  br i1 %or.cond, label %for.body.preheader, label %if.end

for.body.preheader:                               ; preds = %entry
  %xtraiter = and i32 %n, 7
  %0 = icmp ult i32 %n, 8
  br i1 %0, label %if.end.loopexit.unr-lcssa, label %for.body.preheader.new

for.body.preheader.new:                           ; preds = %for.body.preheader
  %unroll_iter = and i32 %n, 2147483640
  br label %for.body

for.body:                                         ; preds = %for.body, %for.body.preheader.new
  %y.06 = phi i32 [ 1, %for.body.preheader.new ], [ %add.7, %for.body ]
  %x.05 = phi i32 [ 0, %for.body.preheader.new ], [ %add.6, %for.body ]
  %niter = phi i32 [ 0, %for.body.preheader.new ], [ %niter.next.7, %for.body ]
  %add = add nsw i32 %y.06, %x.05
  %add.1 = add nsw i32 %add, %y.06
  %add.2 = add nsw i32 %add.1, %add
  %add.3 = add nsw i32 %add.2, %add.1
  %add.4 = add nsw i32 %add.3, %add.2
  %add.5 = add nsw i32 %add.4, %add.3
  %add.6 = add nsw i32 %add.5, %add.4
  %add.7 = add nsw i32 %add.6, %add.5
  %niter.next.7 = add i32 %niter, 8
  %niter.ncmp.7 = icmp eq i32 %niter.next.7, %unroll_iter
  br i1 %niter.ncmp.7, label %if.end.loopexit.unr-lcssa, label %for.body, !llvm.loop !6

if.end.loopexit.unr-lcssa:                        ; preds = %for.body, %for.body.preheader
  %y.06.lcssa.ph = phi i32 [ poison, %for.body.preheader ], [ %add.6, %for.body ]
  %y.06.unr = phi i32 [ 1, %for.body.preheader ], [ %add.7, %for.body ]
  %x.05.unr = phi i32 [ 0, %for.body.preheader ], [ %add.6, %for.body ]
  %lcmp.mod.not = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod.not, label %if.end, label %for.body.epil

for.body.epil:                                    ; preds = %if.end.loopexit.unr-lcssa, %for.body.epil
  %y.06.epil = phi i32 [ %add.epil, %for.body.epil ], [ %y.06.unr, %if.end.loopexit.unr-lcssa ]
  %x.05.epil = phi i32 [ %y.06.epil, %for.body.epil ], [ %x.05.unr, %if.end.loopexit.unr-lcssa ]
  %epil.iter = phi i32 [ %epil.iter.next, %for.body.epil ], [ 0, %if.end.loopexit.unr-lcssa ]
  %add.epil = add nsw i32 %y.06.epil, %x.05.epil
  %epil.iter.next = add i32 %epil.iter, 1
  %epil.iter.cmp.not = icmp eq i32 %epil.iter.next, %xtraiter
  br i1 %epil.iter.cmp.not, label %if.end, label %for.body.epil, !llvm.loop !8

if.end:                                           ; preds = %if.end.loopexit.unr-lcssa, %for.body.epil, %entry
  %result.0 = phi i32 [ 0, %entry ], [ %y.06.lcssa.ph, %if.end.loopexit.unr-lcssa ], [ %y.06.epil, %for.body.epil ]
  ret i32 %result.0
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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !9}
!9 = !{!"llvm.loop.unroll.disable"}
