from pygtrie import StringTrie
from pygtrie import Trie
from collections import deque

ESCAPE = '<$>'

def makeKey(prefix, end):
    return prefix + [end] if prefix else [end]

def findSiblings(trie, key):
    return dict([(x[0][-1], x[1]) for x in trie.items(key[:-1]) if len(x[0]) == len(key)])

def findSiblingLabels(trie, key):
    return [x[0][-1] for x in trie.items(key[:-1]) if len(x[0]) == len(key)]

def ppmc(text, k):
    context = []
    trie = Trie()
    unfound = 256.
    for c in text:
        # Find probability
        unusable = set()
        found = False

        for i in range(min(len(context), k) + 1):
            prefix = context[i:]
            key = makeKey(prefix, c)
            
            if trie.has_key(key):
                found = True
                
                siblings = findSiblings(trie, key)
                siblings[ESCAPE] -= len([sib for sib in siblings if sib in unusable])
                count = sum([siblings[sib] for sib in siblings if sib not in unusable])
                
                print "%s, %1.5f" % ("' '" if c == ' ' else c, trie[key] / float(count))
                break
            else:
                siblings = set(findSiblingLabels(trie, key)).difference(unusable)
                
                if len(siblings) > 0:
                    siblings = findSiblings(trie, key)
                    siblings[ESCAPE] -= len([sib for sib in siblings if sib in unusable])
                    count = sum([siblings[sib] for sib in siblings if (sib not in unusable)])

                    if count != 0:
                        key = makeKey(prefix, ESCAPE)
                        print "%s, %1.5f" % (ESCAPE, siblings[ESCAPE] / float(count))
                
                unusable.update(siblings)
                if ESCAPE in unusable:
                    unusable.remove(ESCAPE)
        
        if not found:
            print "%s, %1.5f" % ("' '" if c == ' ' else c, 1./unfound)
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
    return trie

def main():
    f = open('input-dapatil.txt', 'r')
    text = f.read()
    f.close()

    ppmc(text, 3)

if __name__ == '__main__':
    main()
