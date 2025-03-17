:- use_module(library(clpfd)).

% Main solver predicate
solve_sudoku :-
    problem(Rows),        % Load the puzzle
    sudoku(Rows),         % Apply constraints
    maplist(label, Rows), % Find solution
    print_solution(Rows). % Display full solution

% Pretty-print the Sudoku grid
print_solution(Rows) :-
    format('+-------+-------+-------+~n'),
    print_rows(Rows, 1).

print_rows([], _).
print_rows([Row|Rest], N) :-
    format('|'),
    print_row(Row),
    (0 =:= N mod 3 -> 
        format('+-------+-------+-------+~n')
    ; true),
    N1 is N + 1,
    print_rows(Rest, N1).

print_row([A,B,C,D,E,F,G,H,I]) :-
    maplist(print_cell, [A,B,C]), format(' | '),
    maplist(print_cell, [D,E,F]), format(' | '),
    maplist(print_cell, [G,H,I]), format('|~n').

print_cell(X) :- var(X), format('  ').    % Handle variables
print_cell(X) :- nonvar(X), format(' ~w', X).

% Same puzzle definition and constraints as before
problem([
    [_, _, _, 3, 4, 2, _, _, _],
    [5, 4, _, _, 7, _, _, 8, _],
    [_, _, 2, _, _, 5, 4, _, 6],
    [_, 6, _, 2, _, _, _, _, _],
    [3, _, 8, _, _, _, 2, _, 4],
    [_, _, _, _, _, 8, _, 7, _],
    [6, _, 9, 1, _, _, 5, _, _],
    [_, 3, _, _, 8, _, _, 1, 9],
    [_, _, _, 5, 3, 9, _, _, _]
]).

sudoku(Rows) :-
    length(Rows, 9),
    maplist(length_(9), Rows),
    append(Rows, Cells),
    Cells ins 1..9,
    maplist(all_distinct, Rows),
    transpose(Rows, Columns),
    maplist(all_distinct, Columns),
    blocks(Rows).

blocks([A,B,C,D,E,F,G,H,I]) :-
    block_group(A, B, C),
    block_group(D, E, F),
    block_group(G, H, I).

block_group(A, B, C) :-
    blocks(A, B, C, 3).

blocks([], [], [], _).
blocks([A1,A2,A3|RestA],
       [B1,B2,B3|RestB],
       [C1,C2,C3|RestC], _) :-
    all_distinct([A1,A2,A3,B1,B2,B3,C1,C2,C3]),
    blocks(RestA, RestB, RestC, _).

length_(L, List) :- length(List, L).