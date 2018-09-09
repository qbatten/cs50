import sys, crypt

hashIn = crypt.crypt(sys.argv[2], sys.argv[1])

print(f"salt: {sys.argv[1]}\nkey: {sys.argv[2]}\nhash: {hashIn}")