import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
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
    if request.method == "GET":

        # Get current user data
        user_id = session["user_id"]
        user_data = db.execute(
            "SELECT * FROM users WHERE id = ?", user_id
        )

        # Get stock data
        cash = user_data[0]["cash"]
        name = None
        symbol = None
        shares = None
        price = None
        total = 0
        sum_price = 0

        display = db.execute(
            """
            SELECT company_name, symbol, SUM(shares) AS total_shares, price
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            """, user_id
        )

        # Hide dashboard if no data
        if len(display) == 0:
            no_display = True
            return render_template("index.html", username=user_data[0]["username"], cash=cash, total=cash, no_display=no_display)
        no_display = False

        # Calculate user's total balance
        for stock in display:
            stock_symbol = lookup(stock["symbol"])
            current_price = stock_symbol["price"]
            sum_price += current_price * stock["total_shares"]
        total = cash + sum_price

        return render_template(
            "index.html", username=user_data[0]["username"], cash=cash,
            display=display, lookup=lookup, total=total, no_display=no_display
        )

    return redirect("login.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        # Check symbol
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Invalid symbol", 400)
        symbol = symbol.upper()
        stock = lookup(symbol)
        if not stock:
            return apology("Invalid symbol", 400)

        # Check shares
        try:
            shares = request.form.get("shares")
            if not shares:
                return apology("Invalid number of shares", 400)
            shares = int(shares)
            if shares <= 0:
                return apology("Invalid number of shares", 400)
        except ValueError:
            return apology("Invalid number of shares", 400)

        # Get current user data
        user_id = session["user_id"]
        user_data = db.execute(
            "SELECT * FROM users WHERE id = ?", user_id
        )
        if len(user_data) == 0:
            return apology("No user data", 400)

        before_cash = user_data[0]["cash"]

        # Calculate purchase
        price = stock["price"]
        total_price = shares * price
        after_cash = before_cash - total_price

        # Extract company name
        full_company_name = stock["name"]
        parts = full_company_name.split(" Inc.")
        company_name = parts[0]

        # If not enough cash
        if after_cash <= 0:
            return apology("Not enough money", 400)

        # Update "users" table
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", after_cash, user_id
        )

        # Get user's stock data
        user_stocks = db.execute(
            "SELECT * FROM transactions WHERE user_id = ? AND symbol = ?",
            user_id, symbol
        )

        # If no identical stock
        if len(user_stocks) == 0:

            # Update "transactions" table
            db.execute(
                """
                INSERT INTO transactions (user_id, symbol, company_name, shares, price)
                VALUES (?, ?, ?, ?, ?)
                """, user_id, symbol, company_name, shares, price
            )

            # Update "history" table
            transaction_type = "buy"
            db.execute(
                """
                INSERT INTO history (user_id, transaction_type, symbol, shares, price)
                VALUES (?, ?, ?, ?, ?)
                """, user_id, transaction_type, symbol, shares, price
            )

            return redirect("/")

        # If there is at least one identical stock

        # Get updated shares amount
        updated_shares = int(user_stocks[0]["shares"]) + shares

        # Update transactions table
        db.execute(
            """
            UPDATE transactions
            SET shares = ?, price = ?
            WHERE user_id = ? AND symbol = ?
            """, updated_shares, price, user_id, symbol
        )

        # Update "history" table
        transaction_type = "buy"
        db.execute(
            """
            INSERT INTO history (user_id, transaction_type, symbol, shares, price)
            VALUES (?, ?, ?, ?, ?)
            """, user_id, transaction_type, symbol, shares, price
        )

        return redirect("/")

    else:
        # If symbol was passed
        symbol = request.args.get("symbol", "")

        if not symbol:
            return render_template("buy.html")

        return render_template("buy.html", symbol=symbol, selected_symbol=symbol)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    if request.method == "GET":

        # Get current user data
        user_id = session["user_id"]
        user_data = db.execute(
            "SELECT * FROM users WHERE id = ?", user_id
        )

        # Get stock data
        transaction_type = None
        symbol = None
        shares = None
        price = None
        time = None
        dashboard = db.execute(
            """
            SELECT transaction_type, symbol, shares, price, shares * price AS total_price, time
            FROM history
            WHERE user_id = ?
            """, user_id
        )

        # Ensure there is data to show
        if len(dashboard) == 0:
            return apology("No transaction history", 200)

        # Get data
        transaction_type = dashboard[0]["transaction_type"]
        symbol = dashboard[0]["symbol"]
        shares = dashboard[0]["shares"]
        price = dashboard[0]["price"]
        total_price = dashboard[0]["total_price"]
        time = dashboard[0]["time"]

    return render_template(
        "history.html", dashboard=dashboard, transaction_type=transaction_type,
        symbol=symbol, shares=shares, price=price, time=time,
        total_price=total_price
    )


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
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
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
        # Get stock data
        stock = lookup(symbol)
        if stock:
            return render_template("quoted.html", symbol=stock["symbol"], name=stock["name"], price=stock["price"])
        else:
            return apology("Invalid symbol", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # Forget any user_id
    session.clear()

    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)

        # Create variables
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Compare password and confirmation
        if confirmation != password:
            return apology("invalid confirmation", 400)

        # Avoid username already existing
        rows = db.execute("SELECT username FROM users WHERE username = ?", username)
        if len(rows) > 0:
            return apology("username is already taken, try another", 400)

        # Add new user
        cash = "10000"
        hashed_password = generate_password_hash(password, method='scrypt', salt_length=16)
        rows = db.execute(
            "INSERT INTO users (username, hash, cash) VALUES (?, ?, ?)", username, hashed_password, cash
        )

        # Redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # Get user data
        user_id = session["user_id"]
        symbol = request.form.get("symbol")
        user_data = db.execute(
            "SELECT * FROM users WHERE id = ?", user_id
        )

        dashboard = db.execute(
            """
            SELECT id, symbol, SUM(shares) AS total_shares
            FROM transactions
            WHERE user_id = ? AND symbol = ?
            GROUP BY symbol
            """, user_id, symbol
        )

        if len(dashboard) == 0:
            return apology("Nothing to sell", 200)

        # Get user's symbol data
        for stock in dashboard:
            if symbol == stock["symbol"]:
                user_symbol_shares = stock

        # Check shares
        try:
            shares = int(request.form.get("shares"))
            if shares <= 0 or shares > user_symbol_shares["total_shares"]:
                return apology("Invalid number of shares", 400)
        except ValueError:
            return apology("Invalid number of shares", 400)

        # Get current balance
        before_cash = user_data[0]["cash"]

        # Calculate sale
        stock = lookup(symbol)
        price = stock["price"]
        total_price = shares * price
        after_cash = before_cash + total_price

        # Update "transactions" table
        after_shares = user_symbol_shares["total_shares"] - shares
        transactions = db.execute(
            "SELECT * FROM transactions WHERE user_id = ?", user_id
        )
        if len(transactions) == 0:
            return apology("No transactions", 400)

        temp_shares = shares
        for i in range(len(transactions)):
            if symbol == transactions[i]["symbol"]:

                row_id = transactions[i]["id"]

                # If no shares after update left
                if after_shares == 0:
                    db.execute("DELETE FROM transactions WHERE id = ?", row_id)
                    break
                if temp_shares == transactions[i]["shares"]:
                    db.execute("DELETE FROM transactions WHERE id = ?", row_id)
                    break

                elif temp_shares < transactions[i]["shares"]:
                    left_shares = transactions[i]["shares"] - temp_shares
                    db.execute("UPDATE transactions SET shares = ? WHERE id = ?",
                               left_shares, row_id)
                    break

                else:  # shares > transactions[i]["shares"]
                    db.execute("DELETE FROM transactions WHERE id = ?", row_id)
                    difference = temp_shares - transactions[i]["shares"]
                    temp_shares = difference

        # Update "users" table
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", after_cash, user_id
        )

        # Update "history" table
        transaction_type = "sell"
        db.execute(
            """
            INSERT INTO history (user_id, transaction_type, symbol, shares, price)
            VALUES (?, ?, ?, ?, ?)
            """, user_id, transaction_type, symbol, shares, price
        )

        return redirect("/")
    else:

        # Symbol was provided
        symbol = request.args.get("symbol")

        user_id = session["user_id"]

        if not symbol:

            # Group user's stocks
            dashboard = db.execute(
                """
                SELECT symbol, SUM(shares) AS total_shares
                FROM transactions
                WHERE user_id = ?
                GROUP BY symbol
                """, user_id
            )

            if len(dashboard) == 0:
                return apology("Nothing to sell", 200)

            symbol = dashboard[0]["symbol"]

            return render_template("sell.html", dashboard=dashboard, symbol=symbol)

        # Get data
        dashboard = db.execute(
            """
            SELECT symbol, SUM(shares) AS total_shares
            FROM transactions
            WHERE user_id = ? AND symbol != ?
            GROUP BY symbol
            """, user_id, symbol
        )
        if len(dashboard) > 1:
            dashboard = db.execute(
                """
                SELECT symbol, SUM(shares) AS total_shares
                FROM transactions
                WHERE user_id = ? AND symbol != ?
                GROUP BY symbol
                """, user_id, symbol
            )
            return render_template("sell.html", dashboard=dashboard, symbol=symbol, selected_symbol=symbol)

        if len(dashboard) == 0:
            dashboard = db.execute(
                """
                SELECT symbol, SUM(shares) AS total_shares
                FROM transactions
                WHERE user_id = ?
                GROUP BY symbol
                """, user_id
            )
            return render_template("sell.html", dashboard=dashboard, symbol=symbol, selected_symbol=symbol)

        return render_template("sell.html", dashboard=dashboard, selected_symbol=symbol)


@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():

    # Get user data
    user_id = session["user_id"]
    user_data = db.execute(
        "SELECT * FROM users WHERE id = ?", user_id
    )
    if len(user_data) == 0:
        return apology("No such user", 400)
    username = user_data[0]["username"]
    cash = user_data[0]["cash"]

    if request.method == "POST":
        form_type = request.form.get("form_type")

        if form_type == "change_password":

            # Ensure forms were submitted
            if not request.form.get("current_password"):
                return apology("Must provide current password", 403)

            # Ensure password was submitted
            elif not request.form.get("new_password"):
                return apology("Must provide new password", 403)

            # Ensure username exists and password is correct
            current_password = request.form.get("current_password")
            if len(user_data) != 1 or not check_password_hash(
                user_data[0]["hash"], current_password
            ):
                return apology("invalid password", 403)

            # Update password
            new_password = request.form.get("new_password")
            hashed_password = generate_password_hash(new_password, method='scrypt', salt_length=16)
            db.execute(
                "UPDATE users SET hash = ? WHERE id = ?", hashed_password, user_id
            )

            return render_template("profile.html", username=username, cash=cash)

        elif form_type == "add_cash":

            # Ensure form was submitted
            if not request.form.get("add_cash"):
                return apology("Must provide cash amount", 403)

            add_cash = request.form.get("add_cash")

            # Cash must be from 1 to 5000 at a time
            if int(add_cash) < 1 or int(add_cash) > 5000:
                return apology("Invalid amount of cash", 400)

            # Add cash
            cash += int(add_cash)
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?", cash, user_id
            )

            return render_template("profile.html", username=username, cash=cash)
    return render_template("profile.html", username=username, cash=cash)
