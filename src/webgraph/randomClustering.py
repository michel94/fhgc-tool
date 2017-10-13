import sys
from random import randint

comm = sys.argv[0]
C = int(sys.argv[1])
f2 = sys.argv[2]
print(f2)
N = 0
with open(comm, 'r') as f:
	N = len([0 for r in f])

with open(f2, 'w') as f:
	for i in range(N):
		f.write("%d\n" % randint(0, C))