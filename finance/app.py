import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, dt

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["datetime"] = dt

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


def get_cash():
    return db.execute("SELECT cash FROM users WHERE id=?", session["user_id"])[0]["cash"]


def check_symbol_and_shares(request):
    if not request.form["symbol"]:
        return apology("symbol not provided", 400)
    if not request.form["shares"]:
        return apology("shares not provided", 400)
    try:
        shares = int(request.form["shares"])
    except (ValueError, TypeError):
        return apology("shares must be an integer", 400)
    print(f"{shares}")
    if shares <= 0:
        return apology("shares must be a positive integer", 400)


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
    # db.execute("SELECT symbol, SUM(shares) as shares FROM transactions WHERE user_id=? GROUP BY symbol")
    shares = db.execute("SELECT symbol, count FROM shares WHERE user_id=?", session["user_id"])

    rows = []
    grand_total = 0
    for share in shares:
        quote = lookup(share["symbol"])
        rows += [
            {
                "symbol": share["symbol"],
                "name": quote["name"],
                "shares": share["count"],
                "price": quote["price"],
                "total": share["count"] * quote["price"]
            }
        ]
        grand_total += share["count"] * quote["price"]
    cash = get_cash()
    grand_total += cash

    return render_template("index.html", rows=rows, cash=cash, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    print(f"buy: {session['user_id']}")
    if request.method == "POST":
        error = check_symbol_and_shares(request)
        if error:
            return error

        quote = lookup(request.form["symbol"])
        if not quote:
            return apology("symbol not found", 400)

        shares = float(request.form["shares"])
        price = quote["price"] * shares

        cash = get_cash()

        # Check if user has enough cash
        cash_remaining = cash - price
        if cash_remaining <= 0:
            return apology("you don't have enough cash", 400)

        # Add transaction to DB
        db.execute(
            "INSERT INTO transactions (user_id, type, price, shares, symbol) VALUES (?,'P', ?, ?, ?)",
            session["user_id"],
            quote["price"],
            shares,
            quote["symbol"])

        # Update user shares in DB (Upsert)
        db.execute(
            "INSERT INTO shares (user_id, symbol, count) VALUES (?, ?, ?) ON CONFLICT(user_id, symbol) DO UPDATE SET count=count+?",
            session["user_id"],
            quote["symbol"],
            shares,
            shares)

        # Deduce price from user cash
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            cash_remaining,
            session["user_id"])

        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute(
        "SELECT symbol, shares, price, type, timestamp FROM transactions WHERE user_id=? ORDER BY timestamp DESC",
        session["user_id"])
    return render_template("history.html", rows=rows)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        print(f"login: {session['user_id']}")

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
        if not request.form["symbol"]:
            return apology("symbol not provided", 400)

        quote = lookup(request.form["symbol"])
        if not quote:
            return apology("symbol not found", 400)

        return render_template("quoted.html",
                               name=quote["name"],
                               price=quote["price"],
                               symbol=quote["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        # Check username
        if not request.form["username"]:
            return apology("username not provided", 400)
        # Check username
        if not request.form["password"]:
            return apology("password not provided", 400)
        # Check username inexistance
        user = db.execute("SELECT * FROM users WHERE username = ?", request.form["username"])
        if len(user) != 0:
            return apology("username already exists", 400)
        # Check that passwords are identical
        if (request.form["password"] != request.form["confirmation"]):
            return apology("passwors don't match", 400)

        # Register user
        db.execute(
            "INSERT INTO users (username, hash) VALUES (?,?)",
            request.form['username'],
            generate_password_hash(request.form['password']))

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":
        error = check_symbol_and_shares(request)
        if error:
            return error

        quote = lookup(request.form["symbol"])
        if not quote:
            return apology("symbol not found", 400)

        # Check if the user has (enough) shares
        count = db.execute(
            "SELECT count FROM shares WHERE user_id=? AND symbol=?",
            session["user_id"],
            quote["symbol"])
        if len(count) == 0:
            return apology(f"you do not have shares from {quote['symbol']}", 400)
        shares = int(request.form["shares"])
        count = int(count[0]["count"])
        if count < shares:
            return apology(f"you do not have enough shares from {quote['symbol']}", 400)

        # Remove share if count = 0
        if count == shares:
            print(f"count: {count}, shares: {shares}")
            db.execute("DELETE FROM shares WHERE user_id=? AND symbol=?",
                       session["user_id"],
                       quote["symbol"])

        price = quote["price"] * shares
        new_cash = get_cash() + price

        # Add transaction to DB
        db.execute(
            "INSERT INTO transactions (user_id, type, price, shares, symbol) VALUES (?,'S', ?, ?, ?)",
            session["user_id"],
            quote["price"],
            shares,
            quote["symbol"])

        # Update user shares in DB (Upsert) if not deleted
        if count > shares:
            db.execute(
                "INSERT INTO shares (user_id, symbol, count) VALUES (?, ?, ?) ON CONFLICT DO UPDATE SET count=count-?",
                session["user_id"],
                quote["symbol"],
                shares,
                shares)

        # Add price from user cash
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?",
            new_cash,
            session["user_id"])

        return redirect("/")

    else:
        shares = db.execute("SELECT symbol FROM shares WHERE user_id=?",
                            session["user_id"])
        return render_template("sell.html", shares=shares)
