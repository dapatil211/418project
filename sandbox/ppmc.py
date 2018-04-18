from pygtrie import StringTrie
from pygtrie import Trie
from collections import deque
import math

ESCAPE = '<$>'

def makeKey(prefix, end):
    return prefix + [end] if prefix else [end]

def findSiblings(trie, key):
    return dict([(x[0][-1], x[1]) for x in trie.items(key[:-1]) \
            if len(x[0]) == len(key)])

def findSiblingLabels(trie, key):
    return [x[0][-1] for x in trie.items(key[:-1]) if len(x[0]) == len(key)]

def ppmc(text, k):
    context = []
    trie = Trie()
    unfound = 256.
    probs = []
    chars = []
    for c in reversed(text):
        # Find probability
        unusable = set()
        found = False

        for i in range(min(len(context), k) + 1):
            prefix = context[i:]
            key = makeKey(prefix, c)
            
            if trie.has_key(key):
                found = True
                
                siblings = findSiblings(trie, key)
                siblings[ESCAPE] -= len([sib for sib in siblings \
                        if sib in unusable])
                count = sum([siblings[sib] for sib in siblings \
                        if sib not in unusable])
                
                prob = trie[key] / float(count)
                printChar = "' '" if c == ' ' else c
                
                probs.append(prob)
                chars.append(printChar)
                break
            else:
                siblings = set(findSiblingLabels(trie, key)).difference(unusable)
                
                if len(siblings) > 0:
                    siblings = findSiblings(trie, key)
                    siblings[ESCAPE] -= len([sib for sib in siblings \
                            if sib in unusable])
                    count = sum([siblings[sib] for sib in siblings \
                            if (sib not in unusable)])

                    if count != 0:
                        key = makeKey(prefix, ESCAPE)

                        prob = siblings[ESCAPE] / float(count)
                        probs.append(prob)
                        chars.append(ESCAPE)
                        
                
                unusable.update(siblings)
                if ESCAPE in unusable:
                    unusable.remove(ESCAPE)
        
        if not found:
            prob = 1./unfound
            printChar = "' '" if c == ' ' else c
            probs.append(prob)
            chars.append(printChar)
            unfound -= 1
        
        # Update Trie
        for i in range(min(len(context), k) + 1):
            prefix = context[i:]
            key = makeKey(prefix, c)
            if trie.has_key(key):
                trie[key] += 1
            else:
                trie[key] = 1
                escapeKey = makeKey(prefix, ESCAPE)
                if trie.has_key(escapeKey):
                    trie[escapeKey] += 1
                else:
                    trie[escapeKey] = 1
                
        context.append(c)
        if len(context) > k:
            context = context[1:]
    return chars, probs

def calculateEntropy(probs):
    entropy = 0
    for prob in probs:
        entropy += math.log(prob, 2)
    return -entropy

def main():
    printProbs = False

    f = open('input-dapatil.txt', 'r')
    text = f.read()
    f.close()

    #for k in [1, 2, 3, 4, 5]:
    for k in [3]:
        print "%d Context" % k
        chars, probs = ppmc(text, k)
        if printProbs:
            for (c, p) in zip(chars, probs):
                print "%s, %1.5f" % (c, p)
        print("Total bits: %1.5f" % calculateEntropy(probs))
    
if __name__ == '__main__':
    main()

