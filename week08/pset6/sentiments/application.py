from flask import Flask, redirect, render_template, request, url_for

import sys
import os

import helpers
from analyzer import Analyzer

N_TWEETS = 100

app = Flask(__name__)

def get_scores(tweets):
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)
    
    # initial scores
    n_pos = 0.0
    n_neg = 0.0
    n_neut = 0.0
    
    # iterate throught tweets
    for t in tweets:
        score = analyzer.analyze(t)
        if score > 0.0:
            n_pos += 1
        elif score < 0.0:
            n_neg += 1
        else:
            n_neut += 1
            
    # final scores
    n_pos /= N_TWEETS
    n_neg /= N_TWEETS
    n_neut /= N_TWEETS
    
    return n_pos, n_neg, n_neut

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, count=N_TWEETS)

    # TODO
    # color print
    positive, negative, neutral = get_scores(tweets)

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
