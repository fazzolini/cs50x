import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        # initialize tokenizer
        self.tokenizer = nltk.tokenize.TweetTokenizer()
        
        # create the set of positive words
        self.positives = set()
        with open(positives) as lines:
            for line in lines:
                if (not line.startswith(";")) and (not line.startswith("\n")):
                    line = line.strip("\n ")
                    self.positives.add(line.lower())
                    
        # create the set of negative words
        self.negatives = set()
        with open(negatives) as lines:
            for line in lines:
                if (not line.startswith(";")) and (not line.startswith("\n")):
                    line = line.strip("\n ")
                    self.negatives.add(line.lower())
        
        # check number of words in each set of words       
        # print("positives: {}".format(len(self.positives)))
        # print("negatives: {}".format(len(self.negatives)))


    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        # initialize
        tokens = self.tokenizer.tokenize(text)
        score = 0
        
        # assess each word in text
        for t in tokens:
            if t.lower() in self.positives:
                score += 1
            if t.lower() in self.negatives:
                score -= 1
        
        # return total score for text
        return score
