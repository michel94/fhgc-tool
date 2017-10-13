from math import log

def entropy(v):
	s = sum(v)
	return -sum([i/s * log(i/s, 2) if i != 0 else 0 for i in v])

print(entropy([1, 2, 2, 4, 0, 1]))