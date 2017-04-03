from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# check if string is an int
def is_int(s):
    try: 
        int(s)
        return True
    except ValueError:
        return False

@app.route("/")
@login_required
def index():
    # retreive current cash for user
    anon_cash = usd(db.execute("SELECT cash FROM users WHERE id = :u_id", u_id=session["user_id"])[0]['cash'])
    
    #retreive all transactions of user
    anon_transactions = db.execute("SELECT * FROM transactions WHERE user_id = :u_id", u_id=session["user_id"])
    
    # make a dict with sybbols and total values
    stocks_summarized = {}
    for t in anon_transactions:
        if t["symbol"] not in stocks_summarized:
            stocks_summarized[t["symbol"]] = t["quantity"]
        else:
            stocks_summarized[t["symbol"]] += t["quantity"]
            
    # now create final list to be displayed on index
    stocks = []
    for symbol in sorted(stocks_summarized.keys()):
        symbol_data = lookup(symbol)
        
        symbol_dict = {}
        symbol_dict["symbol"] = symbol
        symbol_dict["name"] = symbol_data["name"]
        symbol_dict["shares"] = stocks_summarized[symbol]
        symbol_dict["price"] = usd(symbol_data["price"])
        symbol_dict["total"] = usd(stocks_summarized[symbol] * symbol_data["price"])
        
        stocks.append(symbol_dict)
        
    
    # stocks = [{"symbol": "AAPL",
    #             "name": "Apple",
    #             "shares": 10,
    #             "price": 143,
    #             "total": 1430}]
    return render_template("index.html", stocks=stocks, cash=anon_cash)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # validate symbol
        stock_data = lookup(request.form.get("symbol"))
        if not stock_data:
            return apology("no data for this symbol")
        elif len(stock_data) != 3:
            return apology("something wrong with data provider")
        else:
            name = stock_data["name"]
            price = stock_data["price"]
            symbol = stock_data["symbol"]
            
        # validate number of shares
        if not is_int(request.form.get("n_shares")) or int(request.form.get("n_shares")) < 1:
            return apology("number of shares must be", "a positive integer")
        else:
            n_shares = int(request.form.get("n_shares"))
        
        # retreive user cash
        user_cash = db.execute("SELECT cash FROM users WHERE id = :u_id", u_id=session["user_id"])[0]['cash']
        
        # calculate transaction value
        trans_value = n_shares * price
        
        # make sure they have enough cash
        if user_cash < trans_value:
            return apology("insufficient funds")
        
        # make record
        db.execute("INSERT INTO transactions (user_id, symbol, quantity, price) VALUES (:u_id, :symbol, :qty, :price)", \
        u_id=session["user_id"], symbol=symbol, qty=n_shares, price=price)
        
        # update user cash
        db.execute("UPDATE users SET cash = cash - :t_value WHERE id = :u_id", t_value=trans_value, u_id=session["user_id"])
        
        # redirect the user to the index
        return redirect(url_for("index"))
    else:
        # return apology("buy template...")
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        stock_data = lookup(request.form.get("symbol"))
        if not stock_data:
            return apology("no data for this symbol")
        elif len(stock_data) != 3:
            return apology("something wrong with data provider")
        else:
            name = stock_data["name"]
            price = stock_data["price"]
            symbol = stock_data["symbol"]
        return render_template("quoted.html", name=name, price=price, symbol=symbol)
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        # TO DO
        # require a username, apology if blank or the username already exists
        if not request.form.get("username"):
            return apology("must provide username")
        
        # store username (lowercase) for later use
        anon = request.form.get("username").lower()
        
        # check if username already exists
        users = db.execute("SELECT * FROM users WHERE username = :username", username=anon)
        if len(users) != 0:
            return apology("such anon already exists")
            
        # password cannot be empty
        if not request.form.get("password") and not request.form.get("password2"):
            return apology("must provide password")
        
        # password twice, apology if blank or do not match
        if not request.form.get("password") or \
        not request.form.get("password2") or \
        request.form.get("password") != request.form.get("password2"):
            return apology("passwords do not match")

        ## INSERT new user into users, storing hash of user’s password, not password itself
        pwd_hash=pwd_context.encrypt(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES(:uname, :pwd)", uname=anon, pwd=pwd_hash)
        
        ## 2 options if register successful
            ## automatically login (good user experience)
            ## redirect to login (worse user experience, easier to implement)
            
        # log the user in automatically
        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=anon)

        # ensure username exists and password is correct (just in case)
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), pwd_hash):
            return apology("something went wrong")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]
        
        # redirect user to home page
        return redirect(url_for("index"))
    else:
        # if GET then offer to register
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return apology("TODO")
