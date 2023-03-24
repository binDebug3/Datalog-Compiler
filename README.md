# Datalog Compiler

## BYU CS 236 Discrete Structures
Over the course of a semester I built a Datalog Scanner, Parser, Relational Database, Datalog interpreter and then optimized 
the rule evaluation. More information can be found <a href="https://faculty.cs.byu.edu/~barker/cs236/labs/labs.php">here</a>. The descriptions that below are taken 
verabutum from the class website. Each part of the project builds upon the previous parts.

### Data Scanner
Write a scanner that reads a sequence of characters from a text file, identifies the datalog language tokens found in the file, and outputs each token. 
(<a href="https://faculty.cs.byu.edu/~barker/cs236/labs/lab1-extra.html">Part 1</a>) 

### Data Parser
Write a parser that reads a datalog program from a text file, builds a data structure that represents the datalog program, and outputs the contents 
of the datalog program data structure. (<a href="https://faculty.cs.byu.edu/~barker/cs236/labs/lab2-extra.html">Part 2</a>) 

### The Relational Database
Write an interpreter that uses relational database operations to evaluate the queries in a Datalog Program. For this project, use only the 
facts to evaluate the queries. The evaluation of rules will be added in the next project. (<a href="https://faculty.cs.byu.edu/~barker/cs236/labs/lab3-extra.html">Part 3</a>) 

### The Datalog Interpreter
Add join and union operations to the relational database from the last project and use the new operations to evaluate the rules in a Datalog program.
(<a href="https://faculty.cs.byu.edu/~barker/cs236/labs/lab4-extra.html">Part 4</a>) 

### Optimizing Rule Evaluation
Build the dependency graph for the rules in a Datalog program and use depth-first search on the graph to improve the evaluation of the rules.
(<a href="https://faculty.cs.byu.edu/~barker/cs236/labs/lab5-extra.html">Part 5</a>)
