/********************************************************************************
 
 Find the rank of each given poker hand. The ranks are listed in the ranks.csv,
 in the decreasing order (starting from the royal flush AKQJT at rank=0 and
 ending at the high card 75432 at rank=7461). Each hand listed in ranks.csv is
 sorted in the decreasing order of rank, such as AATTT. Also notice that tens
 are represented as T, not 10.
 
 ********************************************************************************/

using namespace std;

class CheckRank{
public:
    CheckRank(){
        // the ranks loaded from ranks.csv will be encoded just once
        // when the CheckRank object is initialized.
        encode={{'A',2},{'K',3},{'Q',5},{'J',7},{'T',11},
            {'9',13},{'8',17},{'7',19},{'6',23},{'5',29},
            {'4',31},{'3',37},{'2',41}};
        // ranks.csv is in the same directory as CheckRank.h; but CheckRank.h
        // is called from ../Directory, from which the path to here is:
        string file="ranks.csv";
        ifstream is(file);
        string str;
        getline(is,str);
        int i=0;
        // populate the all_ranks
        while(i<str.size()){
            int j=i;
            string entry;
            while(j<str.size()&&str[j]!=',')
                ++j;
            entry=str.substr(i,j-i);
            all_ranks.push_back(entry);
            i=j+1;
        }
        // Populate flushes map
        for(int i=0;i<10;++i){
            int rank=1;
            for(char c : all_ranks[i]){
                rank*=encode[c];
            }
            if(rank==0)
                throw "Rank is zero!";
            flushes[rank]=i;
        }
        for(int i=322;i<1599;++i){
            int rank=1;
            for(char c : all_ranks[i]){
                rank*=encode[c];
            }
            if(rank==0)
                throw "Rank is zero!";
            flushes[rank]=i;
        }
        // Populate non-flushes map
        for(int i=10;i<322;++i){
            int rank=1;
            for(char c : all_ranks[i]){
                rank*=encode[c];
            }
            if(rank==0)
                throw "Rank is zero!";
            non_flushes[rank]=i;
        }
        for(int i=1599;i<7462;++i){
            int rank=1;
            for(char c : all_ranks[i]){
                rank*=encode[c];
            }
            if(rank==0)
                throw "Rank is zero!";
            non_flushes[rank]=i;
        }
    }
    int findRank(const vector<int> & hand){
        int suit=hand[0];
        int rank=suit;
        suit=suit>>8;
        rank=rank&255; // 255=0b11111111
        for(int i=1;i<hand.size();++i){
            int s=hand[i];
            int r=s;
            s=s>>8;
            r=r&255;
            suit=suit&s;
            rank=rank*r;
        }
        if(suit>0)
            return flushes[rank];
        else
            return non_flushes[rank];
    }
    // return rank from 0 (high card) to 8 (straight/royal flush)
    int bestRank(int r){
        if(r>=6185&&r<7462)
            return 0; // high card
        if(r>=3325)
            return 1; // two pair
        if(r>=2467)
            return 2; // pair
        if(r>=1609)
            return 3; // three of a kind
        if(r>=1599)
            return 4; // straight
        if(r>=322)
            return 5; // flush
        if(r>=166)
            return 6; // full house
        if(r>=10)
            return 7; // four of a kind
        if(r>=0)
            return 8; // straight flush
        cout << "Rank not found!" << endl;
        throw "Rank not found!";
        return 0;
    }
    string get_all_ranks(int n){
        return all_ranks[n];
    }
private:
    vector<string> all_ranks;
    unordered_map<int,int> flushes;
    unordered_map<int,int> non_flushes;
    map<char,int> encode;
};
