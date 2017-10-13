from random import randint
from math import log, ceil

def table(n, m):
	return [[] for i in range(n)]

class SegTree:
	def __init__(self, data, f, I=0):
		self.data = data
		self.f = f
		self.origSize = self.n = len(data)
		self.I = I;
		self.ln = int(ceil(log(self.n, 2)))
		self.data += [I for i in range(2**self.ln - self.n)]
		self.n = len(data)
		self.t = t = table(self.n, int(self.ln)+1)
		for i in range(self.n):
			self.t[i].append(data[i])
		for y in range(1, self.ln+1):
			for x in range(0, self.n, 1<<y):
				d = x+( (1<<y)//2 )
				t[x].append(f(t[x][y-1], t[d][y-1]))

	def get(self, i, j):
		s = self.I
		while i < j:
			for size, idx in [(1<<h, h) for h in range(self.ln)]:
				if idx >= len(self.t[i]):
					break
				if i + size > j:
					p = idx-1
					break
			idx -= 1
			size //= 2
			s = self.f(s, self.t[i][idx])
			i += size

		return s

	def search(self, v):

		i = 0
		k = len(self.t[0])-1
		s = 0
		while k > -1:
			while i+2**k < self.n and s + self.t[i][k] < v:
				s += self.t[i][k]
				i += 2**k
			k -= 1
			
		return i

	def __getitem__(self, i):
		return self.get(0, i+1)

	def __len__(self):
		return self.origSize

	def set(self, x, v):
		self.t[x][0] = self.data[x] = v;
		for y in range(1, self.ln+1):
			xx = x-(x&((1<<y)-1))
			self.t[xx][y] = self.f(self.t[xx][y-1], self.t[xx+(1<<(y-1))][y-1])

if __name__ == '__main__':
	l = [randint(0, 20) for i in range(14)]
	print(l)
	print()
	sg = SegTree(l, lambda x,y: x+y, 0)

	sg.set(5, 4)
	sg.set(6, 4)
	sg.set(7, 4)
	print(sg.get(3,10), sum(l[3:10]))
