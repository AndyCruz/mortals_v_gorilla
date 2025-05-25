#pragma once    //pragma balls

#include "v5dice.h"
#include <string>
using std::string;
using std::max;

enum v5char_damage_types_enum {
    none,
    superficial,
    aggravated
};

struct v5char_tracker_t {
    int max;
    int damSp;
    int damAgg;
};

struct v5char_status_flags {
    bool is_impaired;   //-2 dice to rolls (Physical for HP impairment, Social + Mental for WP impairment)
    bool is_dazed;      //-1 die to Physical rolls + -2 dice to Mental rolls
    bool is_broken;     //-3 dice to combat rolls
    bool is_masswound;  //-2 dice to all rolls + add +1 damage to all damage suffered
    bool is_stunned;    //deal 1 sp wp damage or miss a turn
    bool is_crippled;   //cannot make impacted rolls
    bool is_dead;       //cannot
};

class V5CHAR {
public:
    V5CHAR();   //constructor
    V5CHAR(string name, int max_hp, int max_wp, int pool_off, int pool_def, v5char_damage_types_enum dam_type);   //constructor
    ~V5CHAR();  //destructor

    void DealDamage(int dam, v5char_damage_types_enum dam_type, v5char_tracker_t& tracker);
    void ClearDamage(v5char_tracker_t& tracker);

    v5char_tracker_t GetHp();
    v5char_tracker_t GetWp();

    string GetName();
    int GetAttackPool();
    int GetDefendPool();

    v5char_damage_types_enum GetDamageType();
    v5char_status_flags GetStatusFlags();

    int RollAttackPool(int mod);
    int RollDefendPool(int mod);

    bool IsDead();

    V5DICE GetDiceRaw();

    v5char_tracker_t trackerHp; //I'm lazy
    v5char_tracker_t trackerWp;

private:
    string charName;

    //v5char_tracker_t trackerHp;
    //v5char_tracker_t trackerWp;

    int attackPool;
    int defendPool;

    v5char_damage_types_enum attackType;
    
    bool deadFlag;

    V5DICE charDice;

    v5char_status_flags statusFlags;
};
