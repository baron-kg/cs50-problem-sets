# export API_KEY=pk_f41c1d64a8e347cb9d749d9fbc7f1394

import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Initialize data to be displayed
    userID = session.get("user_id")

    userStockData = db.execute("SELECT stock_symbol, shares FROM user_stocks WHERE user_id = ?", userID)
    for stock in userStockData:
        currentStockInfo = lookup(stock["stock_symbol"])
        stock["stock_price"] = currentStockInfo["price"]
        stock["holding_value"] = stock["shares"] * stock["stock_price"]

    # Extract and remember users cash amount
    getUserCash = db.execute("SELECT cash FROM users WHERE id = ?", userID)
    userCash = getUserCash[0]["cash"]

    # Add up the total value of the users shares
    totalStockValue = 0
    for stock in userStockData:
        totalStockValue += stock["holding_value"]

    # Update stock data to show dollar signs
    for stock in userStockData:
        stock["stock_price"] = usd(stock["stock_price"])
        stock["holding_value"] = usd(stock["holding_value"])

    # Add up cash and share value
    grandTotal = usd(userCash + totalStockValue)
    userCash = usd(userCash)

    return render_template("/index.html", userStockData=userStockData, userCash=userCash, grandTotal=grandTotal)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        # Check if input is blank
        if not request.form.get("symbol"):
            return apology("Please enter stock symbol")

        # Check if stock symbol exists
        if not lookup(request.form.get("symbol")):
            return apology("Stock symbol doesen't exist")

        # Check if entered shares are valid
        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Enter valid amount of shares", 400)
        if not shares > 1:
            return apology("Enter a positive number of stocks")
        if not isinstance(shares, int):
            return apology("Enter a numeric value!")

        # Initialize inputs
        stockSymbol = request.form.get("symbol")
        numOfShares = int(request.form.get("shares"))
        stockInfo = lookup(stockSymbol)
        buyer_id = session.get("user_id")

        # Calculate total purchase price
        purchaseValue = numOfShares * stockInfo["price"]

        # Check if user has enough money to purchase shares
        userCash = db.execute("SELECT cash FROM users WHERE id = ?", buyer_id)
        if not userCash[0]["cash"] > purchaseValue:
            return apology("Not enough money")

        # Log transaction
        db.execute("INSERT INTO transactions(user_id, stock_name, share_num, value, transaction_type) VALUES(?, ?, ?, ?, ?)", buyer_id, stockSymbol, numOfShares, purchaseValue, "buy")
        # Update number of stocks in users portfolio or add new stock record
        checkStock = db.execute("SELECT stock_symbol FROM user_stocks WHERE stock_symbol = ? AND user_id = ?", stockSymbol, buyer_id)
        if checkStock == []:
            db.execute("INSERT INTO user_stocks (user_id, stock_symbol, shares) VALUES (?, ?, ?)", buyer_id, stockSymbol, numOfShares)
        else:
            db.execute("UPDATE user_stocks SET shares = shares + ? WHERE stock_symbol = ? AND user_id = ?", numOfShares, stockSymbol, buyer_id)

        # Update buyers balance
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", purchaseValue, buyer_id)

        return redirect("/")
    else:
        return render_template("/buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Initialize user ID
    userId = session.get("user_id")
    userHistory = db.execute("SELECT transaction_type, stock_name, share_num, transaction_time FROM transactions WHERE user_id = ?", userId)
    return render_template("/history.html", userHistory=userHistory)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
        # Check if stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("Stock symbol not submitted", 400)

        # Initialize stock symbol and request its data using "lookup"
        stockInfo = lookup(request.form.get("symbol"))
        if not stockInfo:
            return apology("Stock symbol not found", 400)
        stockInfo["price"] = usd(stockInfo["price"])

        return render_template("quoted.html", stockInfo=stockInfo)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        # Load submited username, password and repeated password
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check if username and password fields are blank
        if username == "":
            return apology("Please enter a username")
        elif password == "":
            return apology("Please enter a password")
        elif confirmation == "":
            return apology("Please repeat your password")

        # Check if password and confirmation fields are identical
        if password != confirmation:
            return apology("Password fields do not match!")

        # Check if the username already exists in the database
        usernameExists = db.execute("SELECT username FROM users WHERE EXISTS (SELECT username FROM users WHERE username = ?)", username)
        if not usernameExists == []:
            return apology("Username taken!")

        # Hash password
        hashedPass = generate_password_hash(request.form.get("password"))

        # Insert new user into database
        db.execute("INSERT INTO users(username, hash) VALUES (?, ?)", username, hashedPass)

        return redirect("/login")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Initialize user ID
    userId = session.get("user_id")

    # Populate drop down with users stock
    ownedStocks = db.execute("SELECT stock_symbol FROM user_stocks WHERE user_id = ?", userId)

    if request.method == "POST":
        # Check for proper input
        if not request.form.get("symbol"):
            return apology("Please select sock")
        if not request.form.get("shares"):
            return apology("Please enter number of shares to sell")
        if not int(request.form.get("shares")) > 0:
            return apology("Please enter a positive number of shares to sell")

        # Initialize selected stock symbol
        stockSymbol = request.form.get("symbol")
        # Initialize number of shares to sell
        sharesToSell = int(request.form.get("shares"))
        # Initialize users number of shares
        fetchShares = db.execute("SELECT shares FROM user_stocks WHERE stock_symbol = ? AND user_id = ?", stockSymbol, userId)
        currentNumOfShares = fetchShares[0]["shares"]

        # Check if user has shares to sell
        if sharesToSell > currentNumOfShares:
            return apology("Not enough shares to sell")

        # Calculate value of shares to be sold
        currentValueOfStock = lookup(stockSymbol)
        valueSold = sharesToSell * currentValueOfStock["price"]

        # Delete stock from portfolio if selling all shares and log transaction
        if sharesToSell == currentNumOfShares:
            db.execute("DELETE FROM user_stocks WHERE stock_symbol = ? AND user_id = ?", stockSymbol, userId)
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", valueSold, userId)
            db.execute("INSERT INTO transactions (user_id, stock_name, value, share_num, transaction_type) VALUES (?, ?, ?, ?, ?)", userId, stockSymbol, valueSold, sharesToSell, "sell")
            return redirect("/")

        # Reduce share number if selling just a portion and log transaction
        if sharesToSell < currentNumOfShares:
            db.execute("UPDATE user_stocks SET shares = shares - ? WHERE stock_symbol = ? AND user_id = ?", sharesToSell, stockSymbol, userId)
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", valueSold, userId)
            db.execute("INSERT INTO transactions (user_id, stock_name, value, share_num, transaction_type) VALUES (?, ?, ?, ?, ?)", userId, stockSymbol, valueSold, sharesToSell, "sell")
            return redirect("/")
    else:
        return render_template("/sell.html", ownedStocks=ownedStocks)


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def addCash():
    """Allow user to add more cash to account"""

    if request.method == "POST":

        # Check for proper input
        if not request.form.get("cash"):
            return apology("Pleas enter cash amount")
        try:
            cashToAdd = int(request.form.get("cash"))
        except ValueError:
            return apology("Enter valid amount of cash", 400)

        # Initialize user ID and ammount to add
        userId = session.get("user_id")
        cashToAdd = request.form.get("cash")

        # Update users balance
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", cashToAdd, userId)

        return redirect("/")
    else:
        return render_template("/add_cash.html")