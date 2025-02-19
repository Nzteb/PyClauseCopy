
# <img src="https://raw.githubusercontent.com/symbolic-kg/PyClause/master/docs/logo/PyClause/proc/PyClause.png" width="171" height="57" style="margin-right: 10px;" alt="PyClause">


PyClause is a library for easy and efficient usage and learning of symbolic knowledge graph rules of the following form.

```bash

citizenOf(X,Y) <= bornIn(X,A), locatedIn(A,Y)
speaks(X,english) <= lives(X,london)

```

PyClause provides a wide range of rule application functionalities such as query answering and triple scoring. Rules from any system can be used as long as their syntax aligns. PyClause also provides wrappers to directly use and configure the rule miners [AMIE](https://github.com/dig-team/amie) and [AnyBURL](https://web.informatik.uni-mannheim.de/AnyBURL/) from Python.  

## Documentation and Examples
Documentation of the library and all features, rule syntax, data downloads, and a tutorial about rules can be found [here](https://pyclause.readthedocs.io/en/latest/index.html). <br>
All usable default options for the configuration files can be found in [clause/config-default.yaml](clause/config-default.yaml) <br>
We provide runnable examples in [examples](examples/). <br>

## Installation
PyClause only uses a few lightweight Python dependencies and runs under Windows and Linux systems. Linux users need to have a c++ (14) compiler installed. Windows users need to have C++ build tools installed. PyClause requires Microsoft Visual C++ 14.0 or newer. <br>

We recommend using a fresh Conda environment with Python 3.7+. 

### 1) Install from codebase
For running our examples and using our data directories or working with the code
```
git clone https://github.com/symbolic-kg/PyClause
cd PyClause
pip install -e .
```

### 2) Install as a package
For using PyClause in your own Python project and as a dependency. When running our examples, you have to set your own correct paths to data and rule files. <br>
install:

```
pip install git+https://github.com/symbolic-kg/PyClause.git
```

as dependency:
```
## in setup.py

setup(
  name             = "MyProject",
  install_requires = ["PyClause @ git+https://github.com/symbolic-kg/PyClause.git#egg=PyClause"],
  python_requires  = ">=3.7"
)

```

## Quickstart
After installing PyClause with any of the two options, run the following code from anywhere.
```python
from c_clause import QAHandler, Loader
from clause import Options

# ***Example for Query Answering***

# define a knowledge graph
# alternatively, specify file path or use arrays + indices
data = [
    ("anna", "livesIn", "london"),
    ("anna", "learns", "english"),
    ("bernd", "speaks", "french")
]
# define rules, or specify file path
rules = [
     "speaks(X,Y) <= learns(X,Y)",
     "speaks(X,english) <= livesIn(X,london)",
     "speaks(X,english) <= speaks(X,A)"
]
# define rule stats: num_preds, support
stats = [
    [20, 10],
    [40, 35],
    [50, 5],
]
# define options, handlers and load data
opts = Options()
opts.set("qa_handler.aggregation_function", "noisyor")

loader = Loader(options=opts.get("loader"))
loader.load_data(data)
loader.load_rules(rules=rules, stats=stats)

qa = QAHandler(options=opts.get("qa_handler"))
# define query: (anna, speaks, ?); alternatively, use indices
queries = [("anna", "speaks")]
qa.calculate_answers(queries=queries, loader=loader, direction="tail")
# outputs [("english", 0.867 )] 
print(qa.get_answers(as_string=True)[0])

# define query: (?, speaks, english); alternatively, use indices
queries = [("english", "speaks")]
qa.calculate_answers(queries=queries, loader=loader, direction="head")
# outputs [('anna', 0.867), ('bernd', 0.001)] 
print(qa.get_answers(as_string=True)[0])

```

## Run tests (for developers, linux)


Download [this](https://www.dropbox.com/scl/fi/75py3esgpsn6f370ap7q5/test-data.zip?rlkey=7i136d7d7194x2tvoxqsv0e36&dl=0), unpack, and put the folders into data/ <br>
Then, from the base directory run
```pytest -s```.

## How to cite
If you use PyClause, please cite <br>

```
@inproceedings{betz2024pyclause,
  title={PyClause-Simple and Efficient Rule Handling for Knowledge Graphs},
  author={Betz, Patrick and Galarraga, Luis and Ott, Simon and Meilicke, Christian and Suchanek, Fabian M and Stuckenschmidt, Heiner},
  booktitle={IJCAI, demo track},
  year={2024},
  publisher={Ijcai.org}
}
```
If you use AMIE or AnyBURL from within PyClause, please refer to the publications as given on the respective webpages.


## Colophon

Have you spotted it? <br>

 <img src="https://raw.githubusercontent.com/symbolic-kg/PyClause/master/docs/logo/PyClause/proc/PyClause-begriffsschrift-transparent.png" width="214" height="75" style="margin-right: 10px;">

The PyClause logo is a reminiscence of Freges Begriffsschrift, which has been published in the year 1879. Even though Frege is not well known in computer science, in his book Begriffsschrift he developed a calculus using an uncommon notation, which is essentially second-order logic with identity (an extension of first-order logic). <br>

The blue lines in the logo are Freges way to express an implication. If we would write A at the end of the first line and B at the end of the second line, this would correspond to a logical rule A <= B, which can again be understood as a clause with a positive and a negative literal. The round dint at the beginning is Freges way to express universal quantification. For more details we refer to the [Wikipedia](https://en.wikipedia.org/wiki/Begriffsschrift) article or the book from 1879 entitled Begriffsschrift.
