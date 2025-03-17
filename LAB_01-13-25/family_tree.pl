%Facts
parent(gopal, ujjwal).
parent(gopal, prince).
parent(meena, balbeer).

%Rules
child(X, Y) :- parent(Y, X).
sibling(X, Y) :- parent(Z, X) , parent(Z, Y) , X \== Y.
