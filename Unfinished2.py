import sys
import re

from pyspark import SparkContext

def flatMapFunc(document):
    """
    Takes in document, which is a key, value pair, where document[0] is the
    document ID and document[1] is the contents of the document.
    HINT: You need to keep track of three things, word, document ID, and the
    index inside of the document, but you are working with key, value pairs.
    Is there a way to combine these three things and make a key, value pair?
    """
    """ Your code here. """
    flat_map = []
    words_set = set(re.findall(r"\w+", document[1]))
    words = re.findall(r"\w+", document[1])
    for word1 in words_set:
        i = -1
        for word2 in words:
            i = i + 1
            if word1 == word2:
                flat_map.append((word1 + '  ' + document[0], str(i)))
    return flat_map
    # return re.findall(r"\w+", document[1])

def mapFunc(arg):
    """ Your code here. """
    return (arg[0], arg[1])
    # return (arg, arg)

def reduceFunc(arg1, arg2):
    """ Your code here. """
    return arg1 + ',' +arg2
    # return arg1

def index(file_name, output="spark-wc-out-index"):
    sc = SparkContext("local[8]", "Index")
    file = sc.sequenceFile(file_name)

    indices = file.flatMap(flatMapFunc) \
                  .map(mapFunc) \
                  .reduceByKey(reduceFunc) \
                  .sortByKey()

    indices.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        index(argv[1])
    else:
        index(argv[1], argv[2])
