import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

SELLID = 0
BUYID = 1

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    userId = session.get("user_id")
    stockRows = db.execute("SELECT * FROM stocks WHERE id = :userId", userId = userId)
    userRows = db.execute("SELECT * FROM users WHERE id = :userId", userId = userId)
    # collect vars
    cash = userRows[0]["cash"]
    username = userRows[0]["username"]
    totalvalue = cash
    head = ("Stock", "Current Price", "Shares Owned", "Total Value")
    stocks = list()
    stocks.append(head)

    for i in range(len(stockRows)):
        if not stockRows[i]["shares"] == 0:
            result = lookup(stockRows[i]["symbol"])
            currPrice = result["price"]
            if not currPrice:
                return apology("lookup error", 403)
            positionValue = currPrice * stockRows[i]["shares"]
            positionValue = int(positionValue)
            stocks.append((stockRows[i]["symbol"], usd(currPrice), stockRows[i]["shares"], usd(positionValue)))
            totalvalue = totalvalue + positionValue
            totalvalue = usd(totalvalue)
    # total
    cash = usd(cash)
    # stocks object (list of strings, each one says "10 shares of AAPL")
    return render_template("index.html", username = username, cash = cash, totalvalue = totalvalue, stocks = stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # stock vars setup
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        result = lookup(symbol)

        if not shares > 0:
            return apology("shares must be a positive integer", 403)

        if result == None:
            return apology("couldn\'t find that stock. is the symbol correct?", 403)

        purchPrice = result["price"]
        totalPrice = purchPrice * shares

        # user vars setup
        userId = session.get("user_id")
        userRows = db.execute("SELECT * FROM users WHERE id = :userId;", userId = userId)
        userName = userRows[0]["username"]
        cashAvailable = userRows[0]["cash"]

        # check that they have enough cash
        if totalPrice > cashAvailable:
            return apology("sorry, you don't have enough money to buy that", 403)

        # save that purchase in the database
        # update transactions
        db.execute("INSERT INTO transactions VALUES (:userId, :symbol, :shares, :price, CURRENT_TIMESTAMP, :transactType)", userId = userId, symbol = symbol, shares = shares, price = purchPrice, transactType = BUYID)

        # update stocks
        stockRows = db.execute("SELECT * FROM stocks WHERE id = :userId AND symbol = :symbol", userId = userId, symbol = symbol)
        if not stockRows:
            db.execute("INSERT INTO stocks VALUES (:userId, :symbol, :shares)", userId = userId, symbol = symbol, shares = shares)
        elif len(stockRows) == 1:
            newShares = int(stockRows[0]["shares"]) + shares
            db.execute("UPDATE stocks SET shares = :newShares WHERE id = :userId AND symbol = :symbol", newShares = newShares, userId = userId, symbol = symbol)
        else:
            return apology("There was an error in the database.", 403)

        # and deduct the price from their available cash
        newCash = cashAvailable - totalPrice
        db.execute("UPDATE users SET cash=:newCash WHERE id=:userId;", newCash = newCash, userId = userId)


        return redirect("/")

    if request.method == "GET":
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userId = session.get("user_id")
    rows = db.execute("SELECT * FROM transactions WHERE id = :userId ORDER BY time DESC", userId = userId)
    userRows = db.execute("SELECT * FROM users WHERE id = :userId", userId = userId)
    username = userRows[0]["username"]
    head = ("Stock", "Shares", "Price", "Transaction Date", "Buy/Sell")
    stocks = list()
    stocks.append(head)

    for i in range(len(rows)):
        if rows[i]["transact_type"] == 0:
            transactType = "Sell"
        else:
            transactType = "Buy"
        stocks.append((rows[i]["symbol"], rows[i]["shares"], usd(rows[i]["price"]), rows[i]["time"], transactType))

    # stocks object (list of strings, each one says "10 shares of AAPL")
    return render_template("history.html", username = username, stocks = stocks)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        symbol = request.form.get("symbol")

        ## Possible TODO: do I need to scrub this string more?
        result = lookup(symbol)

        if result == None:
            return apology("couldn\'t find that stock. is the symbol correct?", 403)

        return render_template("quoted.html", symbol=result["symbol"], price=result["price"])

    elif request.method == "GET":
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("email"):
            return apology("must provide valid email", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirm-password field (confirmation) was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 403)

        # Ensure password and confirmation match
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("your passwords do not match, please try again", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username OR email = :email",
                          username=request.form.get("username"), email=request.form.get("email"))

        # Ensure username does not already exist
        if len(rows) != 0:
            return apology("sorry, that username exists already or someone used that email for an account", 200)

        # Hash pwd
        hashedpass = generate_password_hash(request.form.get("password"))

        # INSERT new username and hashed pwd
        db.execute("INSERT INTO users (id,username,hash,email) VALUES (NULL,:username,:hashedpass,:email)",
                    username=request.form.get("username"), hashedpass=hashedpass, email=request.form.get("email"))

        # Redirect user to login page
        return render_template("login.html")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        # collect initial vars
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        userId = session.get("user_id")
        stocksRows = db.execute("SELECT * FROM stocks WHERE id = :userId AND symbol = :symbol", userId = userId, symbol = symbol)
        result = lookup(symbol)
        if len(result) < 1:
            return apology("sorry, that stock isn't in our database.", 403)
        sellPrice = result["price"]

        # check whether they have any shares of that stock
        if not stocksRows[0]["symbol"] == symbol:
            return apology("sorry, you don't own any shares of that stock.", 403)

        # check whether they have enough shares of that stock
        if stocksRows[0]["shares"] < shares:
            return apology("sorry, you don't own that many shares of this stock", 403)

        # otherwise, they can sell it!
        # update cash in users
        cashRows = db.execute("SELECT cash FROM users JOIN stocks ON users.id = stocks.id WHERE users.id = :userId AND stocks.symbol = :symbol", userId = userId, symbol = symbol)
        newCash = cashRows[0]["cash"] + sellPrice
        db.execute("UPDATE users SET cash=:newCash WHERE id=:userId", newCash = newCash, userId = userId)

        # update shares in stocks
        newShares = int(stocksRows[0]["shares"]) - shares
        db.execute("UPDATE stocks SET shares = :newShares WHERE symbol = :symbol AND id = :userId",  newShares = newShares, symbol = symbol,userId = userId)

        # add transaction to transactions
        db.execute("INSERT INTO transactions VALUES (:userId, :symbol, :shares, :price, CURRENT_TIMESTAMP, :transactType)", userId = userId, symbol = symbol, shares = shares, price = sellPrice, transactType = SELLID)
        return redirect("/")


    if request.method == "GET":
        return render_template("sell.html")

@app.route("/change", methods=["GET", "POST"])
def change():
    """Get stock quote."""
    if request.method == "POST":
        currPass = request.form.get("currpass")
        if not currPass:
            return render_template("change.html")
        else:
            newPass = request.form.get("newpass")
            newConf = request.form.get("newconf")
            username = request.form.get("username")
            email = request.form.get("email")

            if not newPass == newConf:
                return apology("those passwords do not match", 403)

            rows = db.execute("SELECT * FROM users WHERE username = :username AND email=:email", username=username, email=email)
            if not rows:
                return apology("we can't find that account", 403)

            #oldPass = rows[0]["hash"]
            if not check_password_hash(rows[0]["hash"], currPass):
                return apology("your old password didnt match", 403)

            hNewPass = generate_password_hash(newPass)

            db.execute("UPDATE users SET hash = :newHash WHERE id=:userId", newHash = hNewPass, userId = rows[0]["id"])

            return redirect("/index")

    if request.method == "GET":
        return render_template("change.html")

def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
