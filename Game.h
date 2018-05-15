/********************************************************************
 
 Game module which encodes a Player/Dealer, its stratetgy, and its
 bankroll. It also keeps the sum of strategies, the average strategy,
 and the sum of regrets.
 
 Each Game is played according to a strategy which is encoded in a
 'strategy', which is a 1D vector, which can be interpreted as the
 vector of probabilities to bet, or as a vector of probabilities to
 call, depending on whether the game is used as a Player or a Dealer.
 
 This class uses prime-number representation of each poker rank.
 
 Player's and Dealer's regrets sums and strategy sums are vectors
 of size 2, of the structure (act, do not act). For the Player it
 is interpreted as (bet, do not bet), and for the Dealer it is
 interpreted as (call, do not call).
 
 ********************************************************************/

using namespace std;

class Game{
    
public:
    
    Game(){
        for(int i=0;i<169;++i){
            strategy.push_back(0.5);
            average_strategy.push_back(0);
            vector<double> r={0,0};
            regret_sum.push_back(r);
            strategy_sum.push_back(r);
        }
        bankroll=0;
        subconstructor();
    }
    
    void subconstructor(){
        prime_to_index={{2,0},{3,1},{5,2},{7,3},{11,4},{13,5},
            {17,6},{19,7},{23,8},{29,9},{31,10},{37,11},{41,12}};
        index_to_rank={"A","K","Q","J","10","9","8","7","6","5","4","3","2"};
    }
    
    int strategy_index(const vector<int> & hole_cards){
    // returns index 0...168 corresponding to pair of hole cards.
        int card1=hole_cards[0];
        int card2=hole_cards[1];
        int s1=card1>>8;
        int s2=card2>>8;
        bool same_suit=false;
        if(s1==s2)
            same_suit=true;
        int r1=prime_to_index[card1&255];
        int r2=prime_to_index[card2&255];
        int i,j;
        if(r1==r2){
            i=r1;
            j=r1;
        }
        else if(same_suit){
            if(r1<r2){
                i=r1;
                j=r2;
            }
            else{
                i=r2;
                j=r1;
            }
        }
        else{
            if(r1<r2){
                i=r2;
                j=r1;
            }
            else{
                i=r1;
                j=r2;
            }
        }
        return 13*i+j;
    }
    
    bool act(const vector<int> & hole_cards){
        double p=strategy[strategy_index(hole_cards)];
        double r=((double) rand() /RAND_MAX);
        if(r<p)
            return true; // act (bet or call)
        else
            return false; // do not act (check or fold)
    }
    
    void set_strategy(const int & k, const double & p){
        strategy[k]=p;
    }
    
    void set_average_strategy(const int & k, const double & p){
        average_strategy[k]=p;
    }
    
    void set_strategy_sum(const int & k, const vector<double> & p){
        strategy_sum[k]=p;
    }
    
    void set_regret_sum(const int & k, const vector<double> & p){
        regret_sum[k]=p;
    }
    
    double get_strategy(const int & k){
        return strategy[k];
    }
    
    double get_average_strategy(const int & k){
        return average_strategy[k];
    }
    
    vector<double> get_strategy_sum(const int & k){
        return strategy_sum[k];
    }
    
    vector<double> get_regret_sum(const int & k){
        return regret_sum[k];
    }
    
    vector<double> get_whole_strategy(){
        return strategy;
    }
    
    vector<double> get_whole_average_strategy(){
        return average_strategy;
    }
    
    vector<vector<double>> get_whole_strategy_sum(){
        return strategy_sum;
    }
    
    vector<vector<double>> get_whole_regret_sum(){
        return regret_sum;
    }
    
    void change_bankroll(const double & b){
        bankroll+=b;
    }
    
    void set_bankroll(const double & b){
        bankroll=b;
    }
    
    double get_bankroll(){
        return bankroll;
    }
    
    void print_strategy(const vector<double> & v){
        if(v.size()!=169){
            cout << "Wrong sized-strategy sent to print" << endl;
            throw "Wrong sized-strategy sent to print";
        }
        cout << "Strategy has length " << v.size() << ", and is de-serialzied as" << endl;
        cout << "        ";
        for(string s : index_to_rank){
            cout << s << "          ";
        }
        cout << endl;
        for(int i=0;i<13;++i){
            if(i!=4)
                cout << index_to_rank[i] << "       ";
            else // leave a smaller margin for rank 10
                cout << index_to_rank[i] << "      ";
            for(int j=0;j<13;++j){
                ostringstream strs;
                double e=v[13*i+j];
                if(e<0.001)
                    e=0;
                if(e>0.999)
                    e=1;
                strs << e;
                string str=strs.str();
                string buffer="";
                if(j!=4)
                    for(int k=0;k<11-str.size();++k)
                        buffer+=" ";
                else
                    for(int k=0;k<12-str.size();++k)
                        buffer+=" ";
                cout << str << buffer;
            }
            cout << endl;
        }
    }
    
private:
    
    double bankroll;
    vector<double> strategy;
    vector<double> average_strategy;
    vector<vector<double>> strategy_sum;
    vector<vector<double>> regret_sum;
    unordered_map<int,int> prime_to_index;
    vector<string> index_to_rank;
    
};
