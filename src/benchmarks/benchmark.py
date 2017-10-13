from __future__ import print_function
from sys import argv
from random import randint, random, shuffle
from bisect import bisect_left
from segtree import SegTree
import time

ORDER_COMMUNITIES = True

class Row:
    def __init__(self, mat, row):
        self.mat = mat
        self.row = row
    def __getitem__(self, col):
        if (self.row, col) not in self.mat:
            return 0
        return self.mat.get(self.row, col)
    def __setitem__(self, col, value):
        self.mat.set(self.row, col, value)
    
class Mat:
    def __init__(self, n, m):
        self.n = n
        self.m = m
        self._data = {}
    def get(self, a, b):
        if b < a:
            a, b = b, a
        self._data[(a, b)]
    def set(self, a, b, v):
        if b < a:
            a, b = b, a
        self._data[(a, b)] = v
    def __getitem__(self, row):
        return Row(self, row)
    def __contains__(self, c):
        a, b = c
        if b > a:
            a, b = b, a
        return (a, b) in self._data
    def __iter__(self):
        keys = list(self._data.keys())
        keys = sorted(keys)
        return iter([(k, self._data[k]) for k in keys])

N = None
degree = None
pRewire = None
ncomm = None

a = 1
while a < len(argv):
    if argv[a] == '-N':
        N = int(argv[a+1])
    elif argv[a] == '-k':
        degree = int(argv[a+1])
    elif argv[a] == '-prewire':
        pRewire = float(argv[a+1])
    elif argv[a] == '-ncomm':
        ncomm = int(argv[a+1])
    a += 2

graph = [[] for i in range(N)]
adj = Mat(N, N)

if ORDER_COMMUNITIES:
    clusters = [[] for i in range(ncomm)]
    for i in range(N):
        clusters[randint(0, ncomm-1)].append(i)

s = 0
for c in range(len(clusters)):
    clusters[c] = list(range(s, s+len(clusters[c])))
    s += len(clusters[c])

size = N

search_time = 0
for curcomm, l in enumerate(clusters):
    print('Community', curcomm, len(l))
    N = len(l)
    count = [degree] * N
    sg = SegTree(count, lambda x,y: x+y, 0)
    c = 0
    for i in range(len(l)-1):
        adj.set(l[i], l[i+1], 1)
        c += 1

    while c < int(degree / 2 * len(l)):
        s = sg[N-1]
        
        if s <= 1:
            print('No edges left')
            break

        r = random()
        start_time = time.time()
        idx1 = sg.search(r*s)
        sg.set(idx1, sg.data[idx1]-1)
        elapsed_time = time.time() - start_time
        search_time += elapsed_time
        
        if random() < pRewire:
            while True:
                rc = randint(0, len(clusters)-2)
                if rc == curcomm:
                    rc += 1
                nd = clusters[rc][randint(0, len(clusters[rc])-1)]
                if (l[idx1], nd) not in adj:
                    adj.set(l[idx1], nd, 1)
                    c += 1
                    break
                else:
                    sg.set(idx1, sg.data[idx1]+1)
        else:
            r = random()
            
            s = sg[N-1]

            start_time = time.time()
            idx2 = sg.search(r*s)
            elapsed_time = time.time() - start_time
            search_time += elapsed_time
            sg.set(idx2, sg.data[idx2]-1)

            if idx1 != idx2 and (l[idx1], l[idx2]) not in adj:
                adj.set(l[idx1], l[idx2], 1)
                c += 1
            else:
                sg.set(idx1, sg.data[idx1]+1)
                sg.set(idx2, sg.data[idx2]+1)

    #print(search_time)
N = size

edges = set()
f = open("network.dat", 'w')
for k,v in adj:
    edges.add(k)
    edges.add(k[::-1])

for k in sorted(edges):
    f.write("%d\t%d\n" % (k[0]+1, k[1]+1) )
f.close()

f = open("community.dat", 'w')
m = [0] * N
for curcomm, l in enumerate(clusters):
    for i in l:
        m[i] = curcomm
for c in range(N):
    f.write("%d %d\n" % (c+1, m[c]+1) )
f.close()
