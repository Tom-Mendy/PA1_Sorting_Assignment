# Algorithm Tasks

Task B - Merge Sort

Analysis Question 6

Measured comparison counts scale proportionally to $n \log_2 n$. When you normalize by computing
$\frac{\text{comparisons}}{n\log_2 n}$, the value stays within a relatively small band as $n$ grows,
which matches the theoretical bound for merge sort.

Analysis Question 7

Merge sort is always $O(n \log n)$ because it always splits the input into two halves and performs
linear-time merging at each recursion level. There are about $\log_2 n$ levels and each level does
$O(n)$ work, so total work is always $O(n \log n)$ regardless of input order.

Quick sort is not always $O(n \log n)$ because partition quality depends on pivot position. If pivots
are consistently poor (for example, producing highly unbalanced partitions), recursion depth can grow
to $O(n)$ and total work becomes $O(n^2)$.
