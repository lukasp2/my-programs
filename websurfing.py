#!/usr/bin/env python3

import requests
from bs4 import BeautifulSoup

tmptext = """
[<div class="_1rZYMD_4xY3gRcSS3p8ODO" style="color:#1A1A1B">35.2k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO _25IkBM0rRUqWX5ZojEMAFQ" style="color:#1A1A1B">35.2k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO" style="color:#1A1A1B">47.2k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO _25IkBM0rRUqWX5ZojEMAFQ" style="color:#1A1A1B">47.2k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO" style="color:#1A1A1B">26.3k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO _25IkBM0rRUqWX5ZojEMAFQ" style="color:#1A1A1B">26.3k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO" style="color:#1A1A1B">46.5k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO _25IkBM0rRUqWX5ZojEMAFQ" style="color:#1A1A1B">46.5k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO" style="color:#1A1A1B">19.4k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO _25IkBM0rRUqWX5ZojEMAFQ" style="color:#1A1A1B">19.4k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO" style="color:#1A1A1B">24.8k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO _25IkBM0rRUqWX5ZojEMAFQ" style="color:#1A1A1B">24.8k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO" style="color:#1A1A1B">13.7k</div>, <div class="_1rZYMD_4xY3gRcSS3p8ODO _25IkBM0rRUqWX5ZojEMAFQ" style="color:#1A1A1B">13.7k</div>]
"""

#r = requests.get('https://www.reddit.com/')
soup = BeautifulSoup(tmptext, 'html.parser')

results = soup.find_all('div', attrs={'class':'_1rZYMD_4xY3gRcSS3p8ODO'})

nums = []
for result in results:
    nums.append(''.join(result)[0:-1])

tot = 0
for num in nums:
    num = float(num)
    tot += num
    print(num, "k")

print("total upvotes: ", tot, "k")

