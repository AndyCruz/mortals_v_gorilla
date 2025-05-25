#pragma once    //pragma balls

//#include <string>   //for strings
#include <stdlib.h> //for rand function
#include <vector>   //for vectors
using std::vector;
#include <stdio.h>  //for switch case
#include <time.h>   //for time(...) function to seed srand

/*
    Even Pools # of successes   = [0,2n-1),(2n-1,2n]
        crits come in pairs so max successes is just double pool size when even
        even pools CANNOT score 1 under their max successes
    Odd Pools # of successes    = [0,2n-1]
        odd pools are effectively just an even pool + 1 more die
    Max Pools # of successes    = 2n-(n%2)
        each odd #ed increment is a smaller step than evens because of crits
*/

#define V5DIE_RESULTS_BOTCH     1
#define V5DIE_RESULTS_FAIL      5
#define V5DIE_RESULTS_SUCCESS   6
#define V5DIE_RESULTS_CRIT      10
#define V5DIE_RESULTS_INVALID   -1

#define V5DIE_VALUE_SUCCESS     1
#define V5DIE_VALUE_NOT_SUCCESS 0
#define V5DIE_VALUE_CRITICAL    2   //each 10 in a pair of 10s is worth double successes

enum v5die_results_enum {
    invalid_result,     //0000'0000
    botch,              //0000'0001
    fail,               //0000'0010
    success = 4,        //0000'0100
    crit = 8            //0000'1000
};

enum v5pool_result_enum {
    invalid_pool_result,//0000'0000
    bestial_failed,     //0000'0001
    failed,             //0000'0010
    succeeded = 4,      //0000'0100
    critical = 8,       //0000'1000
    messy_critical = 16 //0001'0000
};

class V5DIE {
public:
    V5DIE();   //constructor
    ~V5DIE();  //destructor
    void roll_die();
    void roll_die_OLD();
    v5die_results_enum get_result();
    int get_score();
    void clear_die();
private:
    int die_score;
    v5die_results_enum die_result;
};

class V5DICE : public V5DIE {
public:
    V5DICE();   //constructor
    ~V5DICE();  //destructor
    void RollDice(int count);
    void RerollDice(vector<bool> array);
    vector<v5die_results_enum> get_results();
    vector<int> get_scores();
    int get_num_successes();
    bool get_crit_status();
    void clear_dice();
private:
    vector<int> dice_scores;
    vector<v5die_results_enum> dice_results;
    int num_successes;
    bool crit_status;
};
