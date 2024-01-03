PyClause
==============
PyClause is a library for easy and efficient usage and learning of symbolic rules for knowledge graphs.

# Documentation
Documentation of library and features [here](https://pyclause.readthedocs.io/en/latest/index.html). <br>
All usable options [here](config-default.yaml) 


# Installation
git clone https://github.com/Nzteb/PyClause \
cd PyClause \
pip install -e .

# Quickstart
```python
from c_clause import QAHandler, Loader
from clause.config.options import Options

# define a knowledge graph
# alternatively specify file path or use indices
data = [
    ("anna", "livesIn", "london"),
    ("anna", "learns", "english"),
    ("bernd", "speaks", "french")
]

# define rules, or specify file path
# num_predictions \t support \t rule string
rules = [
    "20" + "\t"  + "10"  + "\t" + "0.5" + "\t" + "speaks(X,Y) <= learns(X,Y)",
    "40" + "\t"  + "35"  + "\t" + "0.875" + "\t" + "speaks(X,english) <= livesIn(X,london)",
]

# define options, handlers and load data
opts = Options()
opts.set("qa_handler.aggregation_function", "noisyor")

loader = Loader(options=opts.get("loader"))
loader.load_data(data)
loader.load_rules(rules)
qa = QAHandler(options=opts.get("qa_handler"))

# define query: (anna, speaks, ?) 
query = [("anna", "speaks")]
qa.calculate_answers(queries=query, loader=loader, direction="tail")

# outputs [("english", 0.8667 )] 
print(qa.get_answers(as_string=True)[0])


```


# Entity and Relation Representation

All definitions hold independently for entities and relations. <br>
<strong>Tokens:</strong> The strings that describe entities/relations in the data files such as train.txt. In some repositories these are termed 'Identifier'. <br>
<strong>Idx's:</strong> Integers internally used. Tokens are mapped to Idx's by e.g. an entity_map or relation_map <br>
<strong>Names:</strong> Optional additional strings that describe entities and relations in a human understandable way. <br>

# Supported rule types

Let ```h, b1, b2, b3``` be relation token strings. <br>
Let ```X A B C D E F G H .. Y``` be variables <br>
Let ```c,d``` be entity token strings. <br>

### B Rules
Examples<br>
```
h(X,Y) <= b1(X,A), b2(A,Y)
h(X,Y) <= b1(A,X), b2(A,Y)
h(X,Y) <= b1(X,A), b2(A,B), b3(B,Y)
```

### Uc Rules
Examples<br>
```
h(X,c) <= b1(X,d)
h(c,Y) <= b1(Y,c)
h(c,Y) <= b1(c,Y)

h(X,c) <= b1(A,X), b2(A,d)
h(c,Y) <= b1(Y,A), b2(d,A)
h(c,Y) <= b1(A,Y), b2(A,d)

h(c,Y) <= b1(A,Y), b2(A,B), b3(A,d)
```


### Ud Rules
Examples<br>
```
h(X,c) <= b1(X,A)
h(c,Y) <= b1(Y,A)
h(c,Y) <= b1(A,Y)

h(X,c) <= b1(A,X), b2(A,B)
h(c,Y) <= b1(Y,A), b2(B,A)
h(c,Y) <= b1(A,Y), b2(A,B)

h(c,Y) <= b1(A,Y), b2(A,B), b3(A,C)
```


### Zero (Z) Rules
This rule type is used directed. The first rule only makes prediction in tail direction given a head=X and vice versa for the second rule. <br><br>
Examples<br>
```
h(X,c) <= 
h(c,Y) <= 
```

### Uxxc Rules
This rule type only has one body atom.<br>
Examples<br>
```
h(X,X) <= b1(X,d) 
h(X,X) <= b1(d,X) 
```

### Uxxd Rules
This rule type only has one body atom.<br>
Examples<br>
```
h(X,X) <= b1(X,A) 
h(X,X) <= b1(A,X) 
```




# Run tests (temporary)


Download [this](https://www.dropbox.com/scl/fi/75py3esgpsn6f370ap7q5/test-data.zip?rlkey=7i136d7d7194x2tvoxqsv0e36&dl=0), unpack, and put the folders into data/ <br>
Then, from the base directory run
```pytest -s```.



