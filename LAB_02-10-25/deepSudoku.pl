% Simple Sudoku Solver using Backtracking
:- use_module(library(lists)).

% Main predicate to solve and display
start :-
    initial_grid(Grid),
    solve_sudoku(Grid),
    print_sudoku(Grid).

% Sample Sudoku puzzle (0 = empty cell)
initial_grid([
    [0,0,0,3,4,2,0,0,0],
    [5,4,0,0,7,0,0,8,0],
    [0,0,2,0,0,5,4,0,6],
    [0,6,0,2,0,0,0,0,0],
    [3,0,8,0,0,0,2,0,4],
    [0,0,0,0,0,8,0,7,0],
    [6,0,9,1,0,0,5,0,0],
    [0,3,0,0,8,0,0,1,9],
    [0,0,0,5,3,9,0,0,0]
]).

% Convert 0s to variables for Prolog
prepare_grid(Grid, Prepared) :-
    maplist(convert_row, Grid, Prepared).

convert_row(Row, PreparedRow) :-
    maplist(convert_cell, Row, PreparedRow).

convert_cell(0, _).  % Empty cell becomes variable
convert_cell(N, N) :- between(1,9,N).  % Pre-filled numbers stay

% Main solving predicate
solve_sudoku(Grid) :-
    prepare_grid(Grid, PreparedGrid),
    solve(PreparedGrid).

solve(Grid) :-
    find_empty(Grid, Row, Col),  % Find first empty cell
    between(1,9,Num),            % Try numbers 1-9
    valid(Grid, Row, Col, Num),  % Check if valid placement
    set_cell(Grid, Row, Col, Num),  % Place number
    solve(Grid).                 % Recursively solve
solve(Grid) :-                   % Base case: no empty cells
    \+ find_empty(Grid, _, _).

% Find first empty cell (row-major order)
find_empty(Grid, RowIdx, ColIdx) :-
    nth1(RowIdx, Grid, Row),
    nth1(ColIdx, Row, Cell),
    var(Cell).

% Check if number is valid in position
valid(Grid, RowIdx, ColIdx, Num) :-
    % Row check
    nth1(RowIdx, Grid, Row),
    \+ member(Num, Row),
    
    % Column check
    column(Grid, ColIdx, Column),
    \+ member(Num, Column),
    
    % Box check
    box_start(RowIdx, Rs),
    box_start(ColIdx, Cs),
    findall(Cell,
            (between(Rs,Rs+2,R), between(Cs,Cs+2,C),
             nth1(R, Grid, Row1), nth1(C, Row1, Cell)),
            Box),
    \+ member(Num, Box).

% Get column values
column(Grid, Col, Column) :-
    maplist(nth1(Col), Grid, Column).

% Set cell value
set_cell(Grid, Row, Col, Value) :-
    nth1(Row, Grid, GridRow),
    nth1(Col, GridRow, Value).

% Calculate 3x3 box starting index
box_start(Idx, Start) :-
    Start is ((Idx-1)//3)*3 + 1.

% Pretty print the Sudoku grid
print_sudoku(Grid) :-
    format('+-------+-------+-------+~n'),
    print_rows(Grid, 1).

print_rows([], _).
print_rows([Row|Rest], N) :-
    format('|'),
    print_row(Row, 1),
    (0 =:= N mod 3 -> 
        format('+-------+-------+-------+~n')
    ; true),
    N1 is N + 1,
    print_rows(Rest, N1).

print_row([], _).
print_row([H|T], Count) :-
    (var(H) -> format('  ') ; format(' ~w', H)),
    (0 =:= Count mod 3 -> format(' |') ; format(' ')),
    Count1 is Count + 1,
    print_row(T, Count1).