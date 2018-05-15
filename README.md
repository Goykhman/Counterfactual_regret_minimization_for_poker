# Counterfactual_regret_minimization_for_poker
CFR algorithm optimizing strategies in simplified heads-up poker. The rules of the game are as follows.

Two players are in a heads-up game. Each player places the same ante into the pot, and is dealt two private cards.
One player (Dealer) is on the button. The other player (Player) bets/checks. If the Player checks, or if the Player bets and
the Dealer calls, then three community cards are dealt, and the showdown occurs. During the showdown the player who
makes the best five-card poker hand wins. If the Player bets and the Dealer folds, the Player wins the pot.
The bet size is fixed.

The goal of the CFR algorithm is to calculate the (near-)equilibrium strategies for Player/Dealer, prescribing the
probabilities to bet/call when holding each of 169 possible two-card private hands.

* Deck.h contains the Deck class definition. Cards are dealt from the Deck object, where each card is represented by
an integer (inspired by http://suffe.cool/poker/evaluator.html)

c=r+s

where

r=256, 512, 1024, 2048,

s=2,3,5,7,11,13,17,19,23,29,31,37,41.

Deck contains the shuffling and dealing functionality.

* CheckRank.h contains the CheckRank module, which in turn contains functionality to get the rank of any five-card poker
hand. The rank is a number from 0 (Royal Flush) to 7461 (High Card 75432). These 7462 ranks can be grouped into nine
categories (Straight Flush (including Royal Flush), Four of a Kind, Full House, Flush, Straight, Three of a Kind, Two Pair,
Pair, High Card), CheckRank contains functionality to also determine which of these categories the hand belongs to.
During construction, the CheckRank reads the ranks.csv file and prepares the flushes and non-flushes rank lookup tables. The
tables are maps between unique products of five prime numbers and their ranks. It is needed to separate flushes and non
flushes into different tables (this is the only suit difference which affects the strength of the hand), because rank-wise the
content of the tables is the same for (non-straight Flushes and High Cards), and for (Straight Flushes and non-flush
Straights).

Evaluation of the rank is done as follows. Suppose the five-card hand h={c1,...,c5} is given. First for the bitwise AND is
calculated for (c1>>8)&...&(c5>>8) to determine whether the hand is a flush (the output is non-zero iff the hand is a flush).
Then the (non-)flushes are searched for in the (non-)flushes table, by looking up the number (c1* ...* c5) & 255.

* Game.h contains the Game class, which can be used either as Player or as Dealer. Keeps track of the current strategy,
cumulative regret, and strategy sum of the players.

* Regret.cpp runs the CFR self-training simulation, and prints the resulting strategies into console. The strategies are also saved in .csv files. Also outputs the time series fit scores for the Player and the Dealer, and saves them into .csv files.

* Plot_strategies.py consumes the strategies .csv files and produces color-coded plots of the strategies.

* Plot_time_series.py consumes the .csv files with Player's and Dealer's fit scores and produces plots of these time series.
