start:- initial(S),path(S,[],Sol),
        reverse(Sol,Res),prettyprint(Res).
        initial(s(farmer(e),goat(e),cabbage(e),wolf(e))).
	final(s(farmer(w),goat(w),cabbage(w),wolf(w))).

path(Node,Path,[Node|Path]):- final(Node).
path(Node,Path,Sol):- move(Node,N1),
		      not(bad(N1)),
                      not(member(N1,Path)),
                      path(N1,[Node|Path],Sol).

	opp(e,w).	opp(w,e).

bad(s(farmer(T),goat(G),cabbage(C),wolf(W))):-
				(G=W,opp(T,G)); (G=C,opp(T,G)).

%% Farmer takes goat
	move(s(farmer(T),goat(T),cabbage(C),wolf(W)),
		s(farmer(T1),goat(T1),cabbage(C),wolf(W))):-opp(T,T1).

%% Farmer takes cabbage
	move(s(farmer(T),goat(G),cabbage(T),wolf(W)),
		s(farmer(T1),goat(G),cabbage(T1),wolf(W))):-opp(T,T1).

%% Farmer takes wolf
	move(s(farmer(T),goat(G),cabbage(C),wolf(T)),
		s(farmer(T1),goat(G),cabbage(C),wolf(T1))):-opp(T,T1).

%%  Farmer goes alone
	move(s(farmer(T),goat(G),cabbage(C),wolf(W)),
		s(farmer(T1),goat(G),cabbage(C),wolf(W))):-opp(T,T1).

prettyprint(L):- forall(member(X,L),(write(X),nl)).
