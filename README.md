# TaoTopK
该算法为本人新研究的一种TopK快速算法，其最大特点算法复杂度低，通常情形下比较次数略为大于样本总数N。与小堆TopK算法相比较，
N 10000000，K 100，最坏情况下Heap TopK 需要clock num：11358，而本算法clock num:273.
