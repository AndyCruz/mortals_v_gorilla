#pragma once    //pragma balls
#include "v5dice.h"

V5DIE::V5DIE() {
    srand((unsigned int)time(NULL));//seeding rand functions w/ current time in seconds since Jan 1 1970 (at time of compiling? I don't think it is! Yay!) to truly randomize it
    int temp = rand();              //calling rand after seeding srand is suggested to prevent unintentional repetition errors
    clear_die();
}

V5DIE::~V5DIE() {

}

//determined the max value for even distribution w/ modulo by 10 is 32,759 (or 0x7FF7)
#define RAND_CHECK_MAX 32759
#define RAND_NUM_LOOPS 3

void V5DIE::roll_die() {
    int temp = rand();
    for (int i = 0; i < RAND_NUM_LOOPS; i++) {   //cap looping amount in case rand() calls frick up to avoid infinite looping hell
        if (temp <= RAND_CHECK_MAX) {
            break;
        }
        temp = rand();
    }

    die_score = temp % 10;  //evenly distributes output between 0 and 9
    switch (die_score) {
        case 1:
            die_result = botch;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            die_result = fail;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            die_result = success;
            break;
        case 0:
            die_result = crit;
            break;
        default:
            die_result = invalid_result;
    }
    return; //end result is ~30 seconds per TRIAL_6 w/ 100,000 weak mortals vs gorilla
            //original roll_die function below took ~33 seconds per TRIAL_6, so a 9% improvement!
}

void V5DIE::roll_die_OLD() {
    double temp = (double)rand() / (double)(RAND_MAX + 1) * 10.0 + 1; //temp=[1,11)
    die_score = (int)temp;  //die_score=[1,10]

    switch (die_score) {
        case 1:
            die_result = botch;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            die_result = fail;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
            die_result = success;
            break;
        case 10:
            die_result = crit;
            break;
        default:
            die_result = invalid_result;
    }
    return;
}

v5die_results_enum V5DIE::get_result() {
    return die_result;
}

int V5DIE::get_score() {
    return die_score;
}

void V5DIE::clear_die() {
    die_score = -1;             //-1 for both
    die_result = invalid_result;//
    return;
}

//************************************************************************************************************************************************//

V5DICE::V5DICE() {
    clear_dice();
}

V5DICE::~V5DICE() {

}

void V5DICE::RollDice(int count) {
    clear_dice();
    if (count <= 0) {
        return;
    }

    for (int i = 0; i < count; i++) {
        roll_die();
        dice_scores.push_back(get_score());
        dice_results.push_back(get_result());
    }

    bool crit_half = false;
    for (int j = 0; j < dice_results.size(); j++) {
        if (dice_results[j] & success || dice_results[j] & crit) {
            num_successes++;
            if (dice_results[j] & crit) {
                if (crit_half) {
                    num_successes += 2; //already added 1st crit's success before, just added 2nd crit's & now add the bonus 2 (total of 4 successes from a crit pair)
                    crit_status = true;
                    crit_half = false;
                }
                else {
                    crit_half = true;
                }
            }
        }
    }

    return;
}

void V5DICE::RerollDice(vector<bool> array) {
    if (array.size() <= 0) {    //can quit out ASAP if nothing in the vector
        return;
    }
    else if (array.size() > dice_scores.size()) {   //cap the size to the last dice pool size
        array.resize(dice_scores.size());
    }
    for (int i = 0; i < array.size(); i++) {
        if (array.at(i)) {  //for each flag set to TRUE, reroll that die and overwrite the index with the new results
            V5DIE tempDie;
            tempDie.roll_die();
            dice_scores.at(i) = tempDie.get_score();
            dice_results.at(i) = tempDie.get_result();
        }
    }

    //now recalculate this pool's total number of successes
    num_successes = 0;
    crit_status = false;
    bool crit_half = false;
    for (int i = 0; i < dice_results.size(); i++) {
        switch (dice_results.at(i)) {
        case botch:
        case fail:
            break;
        case success:
            num_successes++;
            break;
        case crit:
            num_successes++;
            if (!crit_half) {
                crit_half = true;
            }
            else {
                num_successes += 2;
                crit_half = false;
                crit_status = true;
            }
        }
    }
}

vector<v5die_results_enum> V5DICE::get_results() {
    return dice_results;
}

vector<int> V5DICE::get_scores() {
    return dice_scores;
}

int V5DICE::get_num_successes() {
    return num_successes;
}

bool V5DICE::get_crit_status() {
    return crit_status;
}

void V5DICE::clear_dice() {
    dice_scores.clear();
    dice_results.clear();
    num_successes = 0;
    crit_status = false;
}
