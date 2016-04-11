# A Simple Regular Expression Engine
Apr. 8, 2016

## Overview
Basic idea here is using [Thompson's construction](https://en.wikipedia.org/wiki/
Thompson%27s_construction) to transform the regular expression into an equivalent
NFA (nondeterministic finite automate) and then match string with that NFA. In
reality, the NFA would be further transformed into a DFA and optimize it (minimize
the state amount), but due the limited time I have, I would stop at the NFA stage.

## Architecture
At this point, I only finished the low level NFA part. There are two main class: **NFAState**
and **NFASimulator**. Each NFAState represents a node in NFA and the NFASimulator
just maintains those node(like the *Mechanism and Policy* strategy in OS!). Although
looks like a simple directed graph, it has some more complex features. The most
important one is that we may need to "merge" two state or even two NFA sometimes,
as the Thompson's algorithm parse the regular expression recursively and each recursion
need a "reduction" or "merge" operation.

## Future Work
- The recursive parsing of regex with Thompson's algorithm
- Warp the back-end and let it work like *grep* in Linux
- More test
