# Algorithm Tasks

## Task B - Merge Sort

### Analysis Question 6

Measured comparison counts scale proportionally to $n \log_2 n$. When you normalize by computing
$\frac{\text{comparisons}}{n\log_2 n}$, the value stays within a relatively small band as $n$ grows,
which matches the theoretical bound for merge sort.

### Analysis Question 7

Merge sort is always $O(n \log n)$ because it always splits the input into two halves and performs
linear-time merging at each recursion level. There are about $\log_2 n$ levels and each level does
$O(n)$ work, so total work is always $O(n \log n)$ regardless of input order.

Quick sort is not always $O(n \log n)$ because partition quality depends on pivot position. If pivots
are consistently poor (for example, producing highly unbalanced partitions), recursion depth can grow
to $O(n)$ and total work becomes $O(n^2)$.

## Task C - Heap Sort

### Analysis Question 8

Heap sort stayed in $O(n \log n)$ on all tested inputs, but on random data it was slower than quick
sort in practice.

Random-input timings (largest size):

1. Quick sort at $n=32000$: about 4635 us
2. Heap sort at $n=32000$: about 7789 us

So heap sort took about $\frac{7789}{4635} \approx 1.68\times$ as long on that case.

This matches the common practical behavior: heap sort does more data movement (frequent swaps while
sifting), has less cache-friendly memory access due to jumping across heap levels, and has tighter
branch-dependent control flow. Quick sort on random input tends to have better locality and fewer
expensive movements, so constants are smaller even though both are $O(n \log n)$ asymptotically.

### Analysis Question 9

Measured heap sort swap counts on random input were:

1. $n=1000$: 9107 swaps
2. $n=2000$: 20232 swaps
3. $n=4000$: 44265 swaps
4. $n=8000$: 96564 swaps
5. $n=16000$: 209177 swaps
6. $n=32000$: 450582 swaps

The normalized ratio $\frac{\text{swaps}}{n\log_2 n}$ stayed near a constant band (about $0.91$ to
$0.94$), so the observed swap count is proportional to $n \log n$ on random input.

For equal-input arrays, swaps were exactly $n-1$ in these runs (999, 1999, 3999, ...), because each
extract-max step swaps root with the end, while sift-down performs no extra swaps when all keys are
equal. So for that special case, swap growth is linear in $n$.

## Task D - Counting Sort

### Analysis Question 10

Counting sort has cost $O(n + k)$, so it beats $O(n \log n)$ sorts when $k$ is not too large
compared with $n$ and the key range is dense enough to justify the count array.

From measurements on non-unique input with $\text{maxVal}=1000$ ($k=1000$):

1. At $n=32000$, counting sort was about 351.667 us
2. At $n=32000$, merge sort was about 6875.333 us
3. At $n=32000$, heap sort was about 7764.000 us

So with small fixed $k$, counting sort was around $19.6\times$ faster than merge sort and
$22.1\times$ faster than heap sort on this run.

To see when $k$ becomes impractical, I swept $k$ at fixed $n=32000$:

1. $k=1000$: 340.333 us, count array 4004 bytes
2. $k=10000$: 407.333 us, count array 40004 bytes
3. $k=100000$: 946.333 us, count array 400004 bytes
4. $k=1000000$: 7430.000 us, count array 4000004 bytes

At $k=10^6$, counting sort time was roughly the same as merge sort at this $n$, so the practical
advantage largely disappeared. In general, counting sort becomes impractical when $k$ is large enough
that both memory $(k+1)$ and initialization/scan time for the count array dominate.

### Analysis Question 11

Counting sort is stable when placing right-to-left because equal values are written in reverse
encounter order into the final open positions, which restores their original left-to-right order.
More concretely, if two equal keys appear at indices $i < j$, the element at $j$ is placed first into
the last slot for that key, then the element at $i$ goes into the previous slot, so the final order is
still $i$ before $j$.

If you place left-to-right using the same prefix-sum decrement logic, equal keys get reversed: earlier
occurrences consume later slots and later occurrences move in front of them. That makes the sort
unstable.

## Task E - Radix Sort

### Analysis Question 12

For LSD radix sort, complexity is $O(d\times(n+b))$, where $d$ is number of digit passes and $b$ is
base. Choosing base changes both $d$ and per-pass cost:

1. Larger base: fewer passes ($d$ decreases), but bigger counting array and more per-pass memory work
2. Smaller base: more passes ($d$ increases), but each pass is cheaper and uses less extra memory

So the tradeoff is passes vs per-pass overhead/cache behavior.

In this task, base 10 and $\text{maxVal}=99999$ gives computed $d=5$. Measured at $n=32000$:

1. Radix (base 10): 2191.333 us
2. Merge sort: 7062.333 us
3. Heap sort: 7786.667 us
4. Counting sort with $k=99999$: 934.667 us

This shows radix was clearly faster than comparison sorts for this key range, but still slower than
single-pass counting sort because counting sort pays one $O(n+k)$ pass while radix pays about 5 passes
of stable digit sorting.

### Analysis Question 13

Each digit pass must be stable so that ordering decided by less-significant digits is preserved when
sorting by the next digit.

Counterexample (base 10, two-digit numbers):

1. Input: [21, 22, 11, 12]
2. After stable ones-digit pass: [21, 11, 22, 12]
3. Tens-digit groups are now 1x then 2x if pass is stable, giving final [11, 12, 21, 22]

If the tens-digit pass is unstable, elements with tens digit 1 (11, 12) might be reordered to
[12, 11], and tens digit 2 (21, 22) might become [22, 21], producing [12, 11, 22, 21], which is not
fully sorted.
