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

@app.route("/")
@login_required
def index():
    return apology("index not implemented as of yet")

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    return apology("TODO")

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
    """Get stock quote."""
    return apology("TODO")

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

        ## INSERT the new user into users, storing a hash of the user’s password, not the password itself
        pwd_hash=pwd_context.encrypt(request.form.get("password"))
        
        db.execute("INSERT INTO users (username, hash) VALUES(:uname, :pwd)", uname=anon, pwd=pwd_hash)
        
        ## 2 options if register successful
            ## then automatically login (good user experience)
            ## redirect to login (worse user experience, easier to implement)

        # # redirect user to home page
        # return redirect(url_for("index"))
        return redirect(url_for("login"))
        # return apology("{}, you are Anon now!".format(anon))
    else:
        # if GET then offer to register
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return apology("TODO")
