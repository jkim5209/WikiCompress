import numpy as np
import io
import math
import re
import subprocess
import sys

TEXT_BODY_RE = "<title>((.|\n)*?)</title>((.|\n)*?)<text xml:space=\"preserve\">((.|\n)*?)</text>"
CATEGORY_RE = "\[\[Category:(.*)\]\]"
def get_body_text():
    file = "./enwik8"
    f = io.open(file, mode="r", encoding="utf-8")
    f_str = f.read()
    text_body_re = re.compile(TEXT_BODY_RE)
    category_re = re.compile(CATEGORY_RE)
    text_bodies = []
    article_file = open("clean_articles.txt", "w")
    category_file = open("clean_categories.txt", "w")
    titles_file = open("clean_titles.txt", "w")
    for text_match in text_body_re.finditer(f_str):
        title = text_match.group(1)
        body = text_match.group(5)
        if '#REDIRECT' in body.upper():
            continue
        # print(title)
        # print(body)
        titles_file.write(title + '\n')
        categories = []
        for category_match in category_re.finditer(body):
            category = category_match.group(1)
            if '|' in category:
                category = category.split('|')[0]
            categories.append(category)
        category_file.write('|'.join(categories) + "\n")
        text_file = open("text_body_file.txt", "w")
        text_file.write(body)
        text_file.close()
        perl_script = subprocess.check_output("perl clean_words.pl text_body_file.txt", shell=True)
        article_file.write(perl_script.decode('utf-8') + "\n")
    article_file.close()
    titles_file.close()
    category_file.close()
    
if __name__ == '__main__':
    get_body_text()