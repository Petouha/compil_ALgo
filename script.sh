make
./test > expr.asm < "$1"
asipro expr.asm expr 2> err.asipro.log 
sipro expr
