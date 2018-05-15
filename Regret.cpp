/********************************************************************************
 
 Two players are in a heads-up game. Each player is dealt two private cards.
 One player (Dealer) is on the button. The other (Player) bets/checks. Dealer
 calls/folds, then three community cards are dealt and the showdown occurs.
 To enter the round both Player and Dealer put the same ante. The bet size is
 fixed.
 
 Optimize the strategies using the counterfactual regret minimization algorithm.
 Optimization is done in batches so that one can keep track of the performance
 of the players as the optimization progresses.
 
 Attributes:
 
 starting_bankroll          -- bankroll reset at the beginning of each batch of
                               self-training
 Rounds                     -- number of play rounds in a batch used to evaluate
                               performance
 ante                       -- each player places the same ante before the game
 bet                        -- bet size
 Optimization_rounds        -- number of batches of CFR optimization
 Player                     -- Player
 Dealer                     -- Dealer
 mean_bankroll              -- mean bankroll time series.
 
 ********************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdlib>
#include <random>
#include <time.h>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <stdexcept>

#include "Deck.h"
#include "CheckRank.h"
#include "Game.h"

using namespace std;

class Regret{
    
public:
    
    Regret(double bank, int R, double Bet, double Ante, int E){
        start_bankroll=bank;
        Rounds=R;
        bet=Bet;
        ante=Ante;
        Optimization_rounds=E;
        Player.set_bankroll(start_bankroll);
        Dealer.set_bankroll(start_bankroll);
    }
    
    // This function is called after each round of poker in order to update
    // regrets and strategies counts. If at the given round of poker (played
    // by the poker() method) the player was dealt hand "ip" and the dealer
    // was dealt hand "id", then we need to update strategies of player
    // and dealer when holding hands "ip" and "id", repsectively.
    void prepare_strategies(int ip, int id){
        vector<double> player_regrets=Player.get_regret_sum(ip);
        vector<double> dealer_regrets=Dealer.get_regret_sum(id);
        double pR0=player_regrets[0];
        double pR1=player_regrets[1];
        double dR0=dealer_regrets[0];
        double dR1=dealer_regrets[1];
        if(pR0<0)
            pR0=0;
        if(pR1<0)
            pR1=0;
        if(dR0<0)
            dR0=0;
        if(dR1<0)
            dR1=0;
        double player_regrets_tot=pR0+pR1;
        double dealer_regrets_tot=dR0+dR1;
        // Set Player's strategy
        if(player_regrets_tot<=0)
            Player.set_strategy(ip,0.5);
        else{
            double R=pR0/player_regrets_tot;
            Player.set_strategy(ip,R);
        }
        // Set Dealer's strategy
        if(dealer_regrets_tot<=0)
            Dealer.set_strategy(id,0.5);
        else{
            double R=dR0/dealer_regrets_tot;
            Dealer.set_strategy(id,R);
        }
        // Add strategies to strategies sums
        // Player.
        vector<double> player_strat_sum=Player.get_strategy_sum(ip);
        double p=Player.get_strategy(ip);
        double p_tilde=1-p;
        player_strat_sum[0]+=p;
        player_strat_sum[1]+=p_tilde;
        Player.set_strategy_sum(ip,player_strat_sum);
        // Dealer.
        vector<double> dealer_strat_sum=Dealer.get_strategy_sum(id);
        double q=Dealer.get_strategy(id);
        double q_tilde=1-q;
        dealer_strat_sum[0]+=q;
        dealer_strat_sum[1]+=q_tilde;
        Dealer.set_strategy_sum(id,dealer_strat_sum);
    }
    
    void calculate_average_strategy(){
        for(int j=0;j<169;++j){
            vector<double> player_strat_sum=Player.get_strategy_sum(j);
            Player.set_average_strategy(j,player_strat_sum[0]/(player_strat_sum[0]+player_strat_sum[1]));
            vector<double> dealer_strat_sum=Dealer.get_strategy_sum(j);
            Dealer.set_average_strategy(j,dealer_strat_sum[0]/(dealer_strat_sum[0]+dealer_strat_sum[1]));
        }
    }
    
    vector<int> poker(){
        // Both player and dealer put the same ante
        // Deal the hole cards to player and dealer
        Deck deck;
        int c1=deck.deal_card();
        int c2=deck.deal_card();
        int c3=deck.deal_card();
        int c4=deck.deal_card();
        vector<int> player_hand={c1,c2};
        vector<int> dealer_hand={c3,c4};
        // strategy indexes of Player and Dealer
        int player_strategy_index=Player.strategy_index(player_hand);
        int dealer_strategy_index=Dealer.strategy_index(dealer_hand);
        // current regrets of Player and Dealer
        vector<double> player_regret=Player.get_regret_sum(player_strategy_index);
        vector<double> dealer_regret=Dealer.get_regret_sum(dealer_strategy_index);
        // Deal the community cards
        for(int i=0;i<3;++i){
            int c=deck.deal_card();
            player_hand.push_back(c);
            dealer_hand.push_back(c);
        }
        // Compare the ranks of the best hands player and dealer can claim
        int player_rank=checkrank.findRank(player_hand);
        int dealer_rank=checkrank.findRank(dealer_hand);
        // Determine who wins
        bool player_wins=player_rank<dealer_rank ? true : false;
        double p=Player.get_strategy(player_strategy_index); // probability for Player to bet
        double q=Dealer.get_strategy(dealer_strategy_index); // probability for Dealer to call
        // Consider various game states, defined by who wins or whether it's a draw.
        if(player_rank==dealer_rank){
            // Expected value of Player's strategy given the current game state.
            double Vp=p*(1-q)*ante;
            vector<double> pr={player_regret[0]+(1-q)*ante-Vp,player_regret[1]-Vp};
            Player.set_regret_sum(player_strategy_index,pr);
            // Expected value of Dealer's strategy given the current game state.
            double Vd=(1-q)*(-ante);
            vector<double> dr={dealer_regret[0]+p*(-Vd),dealer_regret[1]+p*(-ante-Vd)};
            Dealer.set_regret_sum(dealer_strategy_index,dr);
        }
        else if(player_wins){
            // Expected value of Player's strategy given the current game state.
            double Vp=(1-p)*ante+p*((1-q)*ante+q*(ante+bet));
            vector<double> pr={player_regret[0]+(1-q)*ante+q*(ante+bet)-Vp,player_regret[1]+ante-Vp};
            Player.set_regret_sum(player_strategy_index,pr);
            // Expected value of Dealer's strategy given the current game state.
            double Vd=(1-q)*(-ante)+q*(-ante-bet);
            vector<double> dr={dealer_regret[0]+p*(-ante-bet-Vd),dealer_regret[1]+p*(-ante-Vd)};
            Dealer.set_regret_sum(dealer_strategy_index,dr);
        }
        else if(!player_wins){
            // Expected value of Player's strategy given the current game state.
            double Vp=(1-p)*(-ante)+p*((1-q)*ante+q*(-ante-bet));
            vector<double> pr={player_regret[0]+(1-q)*ante+q*(-ante-bet)-Vp,player_regret[1]-ante-Vp};
            Player.set_regret_sum(player_strategy_index,pr);
            // Expected value of Dealer's strategy given the current game state.
            double Vd=(1-q)*(-ante)+q*(ante+bet);
            vector<double> dr={dealer_regret[0]+p*(ante+bet-Vd),dealer_regret[1]+p*(-ante-Vd)};
            Dealer.set_regret_sum(dealer_strategy_index,dr);
        }
        vector<int> ret={player_strategy_index,dealer_strategy_index};
        // Before returning, play the actual game.
        double is_bet=Player.act(player_hand); // If Player bets
        double is_call=Dealer.act(dealer_hand); // If Dealer calls
        if(is_bet){
            if(is_call){
                if(player_rank==dealer_rank)
                    return ret;
                else if(player_wins){
                    Player.change_bankroll(bet+ante);
                    Dealer.change_bankroll(-bet-ante);
                    return ret;
                }
                else if(!player_wins){
                    Dealer.change_bankroll(bet+ante);
                    Player.change_bankroll(-bet-ante);
                    return ret;
                }
            }
            else{
                Player.change_bankroll(ante);
                Dealer.change_bankroll(-ante);
                return ret;
            }
        }
        else{
            if(player_rank==dealer_rank)
                return ret;
            else if(player_wins){
                Player.change_bankroll(ante);
                Dealer.change_bankroll(-ante);
                return ret;
            }
            else if(!player_wins){
                Dealer.change_bankroll(ante);
                Player.change_bankroll(-ante);
                return ret;
            }
        }
        return ret;
    }

    void play(){
        Player.set_bankroll(start_bankroll);
        Dealer.set_bankroll(start_bankroll);
        vector<int> players;
        for(int i=0;i<Rounds;++i){
            players=poker();
            prepare_strategies(players[0],players[1]);
        }
        double p=Player.get_bankroll()/start_bankroll;
        double d=Dealer.get_bankroll()/start_bankroll;
        cout << "Player's return is " << p << ", Dealer's return is " << d << endl;
        player_bankroll.push_back(p);
        dealer_bankroll.push_back(d);
    }

    void optimize(){
        for(int i=0;i<Optimization_rounds;++i){
            cout << "i=" << i << endl;
            play();
            if(i%10==0){
                calculate_average_strategy();
                print_average_strategy();
                save_average_strategy();
                save_time_series();
            }
        }
        calculate_average_strategy();
        print_average_strategy();
        save_average_strategy();
        save_time_series();
    }

    void print_average_strategy(){
        cout << "Average betting strategy of Player is" << endl;
        Player.print_strategy(Player.get_whole_average_strategy());
        cout << "Average calling strategy of Dealer is" << endl;
        Dealer.print_strategy(Dealer.get_whole_average_strategy());
    }
    
    void save_average_strategy(){
        // Save Player's strategy
        vector<double> player_strategy=Player.get_whole_average_strategy();
        ofstream file_player;
        file_player.open("strategy_player.csv");
        for(int n=0; n<168; n++){
            file_player << player_strategy[n];
            file_player << "," ;
        }
        file_player << player_strategy[168];
        file_player.close();
        // Save Dealer's strategy
        vector<double> dealer_strategy=Dealer.get_whole_average_strategy();
        ofstream file_dealer;
        file_dealer.open("strategy_dealer.csv");
        for(int n=0; n<168; n++){
            file_dealer << dealer_strategy[n];
            file_dealer << "," ;
        }
        file_dealer << dealer_strategy[168];
        file_dealer.close();
    }
    
    void save_time_series(){
        // Save Player's fit time series
        ofstream file_player;
        file_player.open("player_fit_time_series.csv");
        int vsize = player_bankroll.size()-1;
        for(int n=0; n<vsize; n++){
            file_player << player_bankroll[n];
            file_player << "," ;
        }
        file_player << player_bankroll[vsize];
        file_player.close();
        // Save Dealer's fit time series
        ofstream file_dealer;
        file_dealer.open("dealer_fit_time_series.csv");
        vsize = dealer_bankroll.size()-1;
        for(int n=0; n<vsize; n++){
            file_dealer << dealer_bankroll[n];
            file_dealer << "," ;
        }
        file_dealer << dealer_bankroll[vsize];
        file_dealer.close();
    }

private:
    
    double start_bankroll;
    double Rounds;
    double bet;
    double ante;
    int Optimization_rounds;

    Game Player;
    Game Dealer;

    CheckRank checkrank;

    vector<double> player_bankroll;
    vector<double> dealer_bankroll;
    
};

int main(){
    
    srand(10000*time(0));
    
    double start_bankroll=1000000.0;
    
    int game_rounds=1000000;
    
    double bet=2;
    double ante=1;
    
    int optimization_rounds=2000;
    
    Regret regret(start_bankroll,game_rounds,bet,ante,optimization_rounds);
    
    regret.optimize();
    
    return 0;
}
