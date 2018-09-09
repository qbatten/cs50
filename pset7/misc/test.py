from cs50 import SQL
from helpers import lookup
# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")
userId = 1

rows = db.execute("SELECT id, symbol, shares FROM stocks WHERE id = 3")
#rows = db.execute("SELECT id, symbol, shares, sum(shares) share FROM stocks GROUP BY symbol HAVING id = :userId AND sold = NULL", userId = userId)

for i in range(len(rows)):
	currP = lookup(rows[i]["symbol"])
	#import pdb; pdb.set_trace()
	currPrice = currP["price"]
	print(rows[i]["symbol"], "; ", rows[i]["shares"], "; ", currPrice)