from cs50 import get_int
import sys

def main():
	# get credit card number
	numInt = get_int("Number: ")

	# initialize vars
	numStr = str(numInt)
	numLength = len(numStr)
	cardType = 0 # 1 = amex, 2 = master, 3 = visa

	# check length
	cardType = checkLen(numStr, numLength)

	# Luhn's alg
	if  not luhns(numStr, numLength):
		print("INVALID")
		sys.exit(0)
	else:
		if cardType == 1:
			print("AMEX")
		elif cardType == 2:
			print("MASTERCARD")
		elif cardType == 3:
			print("VISA")

def checkLen(numStr, numLength):
	""" check length of input number, return cardtype or exit if it's not the length of a credit card number"""
	if numLength == 15 and numStr[0] == '3' and (numStr[1] == '4' or numStr[1] == '7'):
		return 1
		VALID += 1
	elif numLength == 16 and numStr[0] == '5' and numStr[1] > '0' and numStr[1] < '6':
		return 2
		VALID += 1
	elif (numLength == 13 or numLength == 16) and numStr[0] == '4':
		return 3
		VALID += 1
	else:
		print("INVALID")
		sys.exit(0)

def luhns(numStr, numLength):
	"""check input using Luhn's algorithm; return True if the input is valid"""
	exclude = []
	result = 0
	i = numLength - 2

	while i >= 0:
		tmp = 2 * int(numStr[i])
		if tmp > 9:
			tmp = (tmp % 10) + 1
		result = result + tmp
		exclude.append(i)
		i = i - 2

	for j in range(numLength):
		if not j in exclude:
			result = result + int(numStr[j])
	if result % 10 == 0:
		return True
	else:
		return False


if __name__ == '__main__':
   main()