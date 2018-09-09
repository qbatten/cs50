import crypt, sys

def main():
	if not len(sys.argv) == 2:
		print("Usage: python crack.py hash")
		sys.exit(0)
	hashIn = sys.argv[1]
	salt = hashIn[0:2]
	key = list(".")
	keylen = len(key)

	while True:
		#check current key guess
		if hashIn == crypt.crypt("".join(key), salt):
			print("".join(key))
			sys.exit(0)

		#increment up one
		keylen = len(key)
		key = increment(key, 1, keylen)


def increment(key, place, keylen):
	"""increments the given key in the specified and  to the left if necessary; according to the range of chars that crypt accepts"""
	#init vars
	i = keylen - place
	tmp = ord(key[i]) #current char

	#check whether we need to maek our key guess longer (if so, do it)
	tmpZ = 0
	for j in range(keylen):
		if key[j] == "z":
			tmpZ += 1
		else:
			break
	if tmpZ == keylen:
		print(key)
		if keylen == 1:
			keyN = list("..")
		elif keylen == 2:
			keyN = list("...")
		elif keylen == 3:
			keyN = list("....")
		elif keylen == 4:
			keyN = list(".....")
		elif keylen == 5:
			print("reached end")
			exit(1)

	# special cases bc of the char's that crypt can take as input
	elif tmp == 57:
		keyN = replace(key,chr(65),i)
	elif tmp == 90:
		keyN = replace(key, chr(97), i)
	# if the char we're changing got to the end of range, reset it to "." and also increment the char to its left
	elif tmp == 122:
		keyN = replace(key, chr(46), i)
		keyN = increment(keyN, place + 1, keylen)
		if place > keylen or place < 0:
			print("Place overrun in increment!")
			exit(1)
	# if it's not a special case, just increase the char's unicdoe number by 1
	else:
		keyN = replace(key, chr(tmp + 1), i)

	return keyN


def replace(key, new, index):
	"""replaces a given value in the key"""
	if len(key) == 0:
		keyN = list(new)
	else:
		keyN = key[0:(index)] + key[index + 1:]
		keyN.insert(index, new)
	return keyN

if __name__ == '__main__':
   main()