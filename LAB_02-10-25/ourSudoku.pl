/*File: Puzzles/sudokuSolve.pro Author: Mihaela Malita
Title:  SUDOKU Solver. From: http://www.sudoku.com/
A sort of Magic Square 9 x9
all integers: 1-9 fill on lines and columns (no repeats)
also blocks (9) do not repeat
SWI predicates: permutation/2  and is_set/1
**************************************************************/
%% start:- write('Enter known positions?'),read(S),solve(S).

start:-S=[[_, _, _, 3, 4, 2, _, _, _],
	  [5, 4, _, _, 7, _, _, 8, _],
	  [_, _, 2, _, _, 5, 4, _, 6],
	  [_, 6, _, 2, _, _, _, _, _],
	  [3, _, 8, _, _, _, 2, _, 4],
	  [_, _, _, _, _, 8, _, 7, _],
	  [6, _, 9, 1, _, _, 5, _, _],
	  [_, 3, _, _, 8, _, _, 1, 9],
	  [_, _, _, 5, 3, 9, _, _, _]],
          solve(S).

write_square(S):- nl,forall(member(X,S),(write(X),nl)),nl.
/*
two lines are different if they do not have any elements on the same position.
?- different([a,b],[b,a]).
true
?- different([a,b],[a,a]).
false
----------------------------------*/


/*
Collect non variables from lines:
?- notvars([_,2,_,_,5,_,7,_,9,1],L).
L = [2, 5, 7, 9, 1]
----------------------------------*/

%% Define possible numbers

is_set_vertical([[], [], [], [], [], [], [], [], []]).

is_set_vertical(S):-
Nine = [1,2,3,4,5,6,7,8,9],
    S = [[H1|T1],[H2|T2],[H3|T3],[H4|T4],[H5|T5],[H6|T6],[H7|T7],[H8|T8],[H9|T9]],
    permutation([H1,H2,H3,H4,H5,H6,H7,H8,H9], Nine),
    is_set([H1,H2,H3,H4,H5,H6,H7,H8,H9]),
    is_set_vertical([T1, T2, T3, T4, T5, T6, T7, T8, T9]).

solve(S):-
Nine = [1,2,3,4,5,6,7,8,9],
        %S = [L1,L2,L3,L4,L5,L6,L7,L8,L9],  %% Assign rows
        forall(member(X,S),(permutation(X,Nine),is_set(X))),
        write_square(S), is_set_vertical(S), write_square(S).


