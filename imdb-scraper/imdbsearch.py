#!/usr/bin/env python3

from bs4 import BeautifulSoup

# this script lists imdb titles.

# input format:
# imdbsearch.py --type="movies,series" --genre="thriller" --min-rating=""

# if no input is given, this output is generated:
# wrong number of arguments
# usage: imdbsearch.py --type="" --genre="" --min-rating=""

# --type defaults to movies and series
# --genre defaults to all genres
# --min-rating defaults to 6

# search options
title_type = ["movies", "series"]
genre = "thriller"
min_rating = 7
min_votes = 5000

link = "https://www.imdb.com/search/title/?title_type=tv_movie&user_rating=7.0,10.0&num_votes=5000,10000000&genres=thriller"

