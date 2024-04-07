make
./test > expr.asm < test.tex
asipro expr.asm expr 2> err.asipro.log 
sipro expr
