# hcc
Horn Clause Compiler


## Requirements
  - flex (`apt install flex`)
  - bison (`apt install bison`)
  - (optional) python3 for testing and plotting (`apt install python3 python3-pip`)

## Build
  - `make`
  - old build scripts are deprecated and should not be used 
  - if anyone wants to build this on Windows, good luck 

## Compiling a file 
 - `./hcc < test.pl`
 - out contains the flow model 
 - out.tab contains the symbol table

## Plotting a flow model
 - debugging larger flow models can be quite tedious. 
 - we've created a python script that plots the network graph for a compiled flow model (`out`) 
 - Intall python dependencies: `pip3 install -r plot/requirements`
 - You might also need to install `tkinter`: `apt install python3-tk` 
 - `plot/plot.py out` plots the graph for file out
 - layout is not perfect; but hopefully it helps a bit
 - networkx does not support controlling exit/entry positions of edges (afaik)
   - i.e., we can not model the left/right input/output


## Testing
 - to don't break the compiler, when implementing new functionality, we've implemented a test framework
 - `make test` compiled the files in `test/data/in` and compares the output to the expected output in `test/data/out`
 - in case of a mismatch it outputs the file names
 - in case of success it outputs nothing 
