// mortals_v_gorilla.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "v5dice.h"
#include "v5char.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>  //for std::setprecision(...)
using std::cout;
using std::cin;
#include <conio.h>

            //name, max_HP, max_WP, attack_pool, defend_pool, damage_type
V5CHAR weakMortal("Jeremy", 4, 2, 1, 1, superficial);
V5CHAR averageMortal("Mr. Jeremy", 5, 3, 3, 2, superficial);
V5CHAR weakMortalCombat("Jacked Jeremy", 5, 2, 4, 3, superficial);
V5CHAR deadlyMortal("Jeremy the Ripper", 8, 7, 11, 8, superficial);

V5CHAR gorilla("Son of Harambe", 8, 2, 7, 7, aggravated);


void PressSpaceToContinue();

bool WpCheck(V5DICE pool, int threshold, vector<bool>& array);
int HalveForSuperficial(int dam);
void PrintTracker(v5char_tracker_t tracker);

enum margin_enum {
    draw,
    mortal_win,
    gorilla_win,
    invalid_margin
};

//#define TRIAL_1   //stats for rolling a pool of a specific size
//#define TRIAL_2   //stats for repeat conflict rolls, margin & damage
//#define TRIAL_3   //turn-by-turn bout of combat
//#define TRIAL_4   //throw 100 men at that stinkin ape to test its defenses
//#define TRIAL_5   //let the blood bath begin...
#define TRIAL_6   //big data time

int main()
{

    V5CHAR trialMortal = weakMortal;
    V5CHAR trialGorilla = gorilla;

    int numberOfTrials = 100'000;
    int numberOfMortalsAttacking = 3;

    /*
    //3 is the mythical breakpoint for ganging up on the gorilla:
    //  * 2 or lower and the gorilla can take out 100+
    //  * at 3 it cannot!
    //  * any higher than 3 and it's not even close
    */

#ifdef TRIAL_1
    V5DICE myPool;
    int num_dice = 7;   //8->9 crosses the 50% rounding -> 60% rounding threshold
    while (1) {
        int num_trials = numberOfTrials;
        int moving_total = 0;
        double average = -1;
        int min = num_trials;
        int max = -1;

        for (int i = 0; i < num_trials; i++) {
            myPool.RollDice(num_dice);

            //cout << "rolled " << num_dice << " dice: ";
            //for (int i : myPool.get_scores()) {
            //    cout << i << ",";
            //}
            //cout << "\n# of successes: " << myPool.get_num_successes() << "\n";
            if (myPool.get_num_successes() < min) {
                min = myPool.get_num_successes();
            }
            if (myPool.get_num_successes() > max) {
                max = myPool.get_num_successes();
            }
            moving_total += myPool.get_num_successes();
        }
        average = (double)moving_total / (double)num_trials;
        cout << "TRIALS = " << num_trials << ", DICE = " << num_dice << ", MIN = " << min << ", MAX = " << max << " (theory max = "
            << 2 * num_dice - (num_dice % 2) << "), MEAN = " << average << "\n";


        PressSpaceToContinue();
    }
#endif // TRIAL_1

#ifdef TRIAL_2

    V5DICE mortalDice;
    V5DICE gorillaDice;

    //V5CHAR trialMortal = weakMortal;
    //V5CHAR trialGorilla = gorilla;
    int trialMortalResult = 0, trialGorillaResult = 0;

    cout << std::fixed << std::setprecision(3);

    while (1) {
        int num_trials = numberOfTrials;
        int moving_total_margin = 0;
        int moving_total_margin_on_gorilla = 0, moving_total_margin_on_mortal = 0;
        double average_margin = -1;
        double average_margin_on_gorilla = -1, average_margin_on_mortal = -1;
        int moving_total_damage = 0;
        int moving_total_damage_to_gorilla = 0, moving_total_damage_to_mortal = 0;
        double average_damage = -1;
        double average_damage_to_gorilla = -1, average_damage_to_mortal = -1;

        //int moving_total_gorilla_want_reroll = 0;
        //double average_gorilla_want_reroll = -1;
        //vector<bool> tempArray;

        int margin = 0;
        int damage = 0;
        int damageToGorilla = 0;
        int damageToMortal = 0;

        for (int i = 0; i < num_trials; i++) {

            trialMortalResult = trialMortal.RollAttackPool(0);
            trialGorillaResult = trialGorilla.RollAttackPool(0);

            //if (WpCheck(trialGorilla.GetDiceRaw(), 3, tempArray)) {
            //    moving_total_gorilla_want_reroll++;
            //}

            margin = abs(trialGorillaResult - trialMortalResult);
            damage = 0;
            damageToGorilla = 0, damageToMortal = 0;

            if (trialGorillaResult > trialMortalResult && trialGorilla.GetDamageType() == aggravated) {   //don't halve aggravated damage
                damage = margin;
            }
            else if (trialMortalResult > trialGorillaResult && trialMortal.GetDamageType() == aggravated) {
                damage = margin;
            }
            else {                                          //only halve superficial damage
                damage = HalveForSuperficial(margin);
                //cout << ", damage = " << damage << ")\n";
            }

            if (margin == 0) {
                damageToGorilla = damageToMortal = 1;
            //    cout << "They tie! Both suffer " << damage << " superficial damage...";
            }
            else if (trialMortalResult > trialGorillaResult) {
                damageToGorilla = damage;
                //    cout << trialMortal.GetName() << " wins with a margin of " << margin << ". " << trialGorilla.GetName() << "takes " << damage << " superficial damage...";
                moving_total_margin_on_gorilla += margin;
            }
            else if (trialMortalResult < trialGorillaResult) {
                damageToMortal = damage;
            //    cout << trialGorilla.GetName() << " wins with a margin of " << margin << ". " << trialMortal.GetName() << " takes " << damage << " superficial damage...";
                moving_total_margin_on_mortal += margin;
            }
            //cout << "\n\n";
            moving_total_margin += margin;

            moving_total_damage += damage;
            moving_total_damage_to_gorilla += damageToGorilla;
            moving_total_damage_to_mortal += damageToMortal;
        }
        cout << "" << trialMortal.GetName() << " vs. " << trialGorilla.GetName() << "\n";
        average_margin = (double)moving_total_margin / (double)num_trials;
        average_margin_on_gorilla = (double)moving_total_margin_on_gorilla / (double)num_trials;
        average_margin_on_mortal = (double)moving_total_margin_on_mortal / (double)num_trials;
        cout << "\t\tNum trials = " << num_trials << ", Total mean margin = " << average_margin << "\n";
        cout << "\tMean " << trialGorilla.GetName() << " margin over " << trialMortal.GetName() << " = " << average_margin_on_mortal
            << ", \tMean " << trialMortal.GetName() << " margin over " << trialGorilla.GetName() << " = " << average_margin_on_gorilla << "\n";

        average_damage = (double)moving_total_damage / (double)num_trials;
        average_damage_to_gorilla = (double)moving_total_damage_to_gorilla / (double)num_trials;
        average_damage_to_mortal = (double)moving_total_damage_to_mortal / (double)num_trials;
        cout << "\t" << "Mean " << trialGorilla.GetName() << " damage to " << trialMortal.GetName() << " = " << average_damage_to_mortal
            << ", \tMean " << trialMortal.GetName() << " damage to " << trialGorilla.GetName() << " = " << average_damage_to_gorilla << "\n\t";

        //average_gorilla_want_reroll = (double)moving_total_gorilla_want_reroll / (double)num_trials * 100.0;
        //cout << "\t\t% of the time " << trialGorilla.GetName() << " would have rerolled = " << average_gorilla_want_reroll << "\n";

        PressSpaceToContinue();
    }
#endif // TRIAL_2

#ifdef TRIAL_3
    //V5CHAR trialMortal = weakMortal;
    //V5CHAR trialGorilla = gorilla;
    int trialMortalResult = 0, trialGorillaResult = 0;

    int turnCount = 0;

    int trialMortalSuccesses = 0, trialGorillaSuccesses = 0;
    int trialMarginNum = 0;
    int trialDamageNum = 0;
    margin_enum marginResult = invalid_margin;
    string winner, loser;

    cout << std::fixed << std::setprecision(3);

    cout << "Combat begins!\t" << trialMortal.GetName() << " vs " << trialGorilla.GetName() << "!\n";

    while (1) {
        cout << "Turn " << ++turnCount << ":\n";

        trialMortalSuccesses = trialMortal.RollAttackPool(0);
        trialGorillaSuccesses = trialGorilla.RollAttackPool(0);

        cout << "\t" << trialMortal.GetName() << " rolls " << trialMortalSuccesses << " success(es), "\
            << trialGorilla.GetName() << " rolls " << trialGorillaSuccesses << " success(es)\n";

        trialMarginNum = abs(trialMortalSuccesses - trialGorillaSuccesses);
        trialDamageNum = trialMarginNum;

        if (trialMarginNum == 0) {
            marginResult = draw;
            trialDamageNum = 1;
            winner = "draw";
            loser = "both";
        }
        else if (trialMortalSuccesses > trialGorillaSuccesses) {
            marginResult = mortal_win;
            winner = trialMortal.GetName();
            loser = trialGorilla.GetName();
            cout << "(FRAUD ALERT)";
        }
        else if (trialMortalSuccesses < trialGorillaSuccesses) {
            marginResult = gorilla_win;
            winner = trialGorilla.GetName();
            loser = trialMortal.GetName();
        }
        else {
            cout << "uhhh, some kind of error";
            marginResult = invalid_margin;
            winner = loser = "invalid";
            trialDamageNum = 0;
        }

        cout << "\tWinner of this turn: " << winner << "! Deal " << trialDamageNum << " damage to " << loser << "\t";
        
        PressSpaceToContinue();
    }
#endif // TRIAL_3

#ifdef TRIAL_4
    int numJeremy = 100;

    bool trialGorillaImpaired = false;
    int penaltyImpaired = -2;

    int tempGorillaSuccesses = 0;
    int tempMortalSuccesses = 0;
    int tempDamage = 0;

    while (1) {
        cout << "Combat begins! " << trialGorilla.GetName() << " defends themself from a horde of angry " << trialMortal.GetName() << "s!!!\n";
        for (int i = 0; i < numJeremy; i++) {
            if (trialGorillaImpaired) {
                tempGorillaSuccesses = trialGorilla.RollDefendPool(-i + penaltyImpaired);
            }
            else {
                tempGorillaSuccesses = trialGorilla.RollDefendPool(-i); //reduce pool by each successive defense roll (min 1 die)
            }
            tempMortalSuccesses = trialMortal.RollAttackPool(0);

            cout << "\t" << trialMortal.GetName() << " #" << i + 1 << "\n\t\t" << trialMortal.GetName() << " rolls " << tempMortalSuccesses
                << " (" << trialMortal.GetAttackPool() << " dice), " << trialGorilla.GetName() << " rolls " << tempGorillaSuccesses
                << " (" << max(1, trialGorilla.GetDefendPool() - i) << " dice)";

            if (tempMortalSuccesses > tempGorillaSuccesses) {
                tempDamage = tempMortalSuccesses - tempGorillaSuccesses;
            }
            else if (tempMortalSuccesses == tempGorillaSuccesses) {
                tempDamage = 1;
            }
            else {
                tempDamage = 0;
            }

            cout << "\n\t\t\tAttack deals " << tempDamage << " damage!";
            if (trialMortal.GetDamageType() == superficial) {
                tempDamage = HalveForSuperficial(tempDamage);
            }
            trialGorilla.DealDamage(tempDamage, trialMortal.GetDamageType(), trialGorilla.trackerHp);
            cout << " (" << trialGorilla.GetHp().damSp << " sp + " << trialGorilla.GetHp().damAgg << " agg / " << trialGorilla.GetHp().max
                << " health) ";
            PrintTracker(trialGorilla.GetHp());
            cout << "\n";

            if (!trialGorillaImpaired) {    //include the impairment penalty once impaired
                if (trialGorilla.GetStatusFlags().is_impaired) {
                    trialGorillaImpaired = true;
                    cout << "\t\t\t\t" << trialGorilla.GetName() << " is now impaired!\n";
                }
            }

            if (trialGorilla.IsDead()) {
                cout << "\n\t" << trialMortal.GetName() << " #" << i + 1 << " lands the killing blow!\t";
                break;
            }
        }
        if (!trialGorilla.IsDead()) {
            cout << "\t" << trialGorilla.GetName() << " survives! Suffered " << trialGorilla.GetHp().damSp << " Sp + "
                << trialGorilla.GetHp().damAgg << " Agg damage, out of " << trialGorilla.GetHp().max << " health...\t";
        }
        PressSpaceToContinue();
        trialGorilla.ClearDamage(trialGorilla.trackerHp);
    }
#endif // TRIAL_4

#ifdef TRIAL_5
    int trialMortalGangupSize = numberOfMortalsAttacking;

    int deadTrialMortalsCount = 0;
    int trialMortalMod = 0;
    int trialGorillaMod = 0;

    int tempMargin = 0;
    int tempDamage = 0;
    int tempDamToMortal = 0, tempDamToGorilla = 0;

    int turnCount = 0;

    bool trialMortalImpaired = false, trialGorillaImpaired = false;
    const int penalty_impaired = -2;

    int tempGorillaSuccesses = 0;
    int tempMortalSuccesses = 0;

    while (1) {

        cout << "\nLadies and gentlemen, welcome to the arena!!!\nIn the defending champion's corner we have... " << trialGorilla.GetName()
            << "!\nAaaand in the challenger's corner is gathered a mass of the same opponent... " << trialMortal.GetName() << "!";
        cout << "\n\t(Fight lasts until " << trialGorilla.GetName() << "'s death, ganged up on by " << trialMortalGangupSize
            << " " << trialMortal.GetName() << "s at a time)\n\t";
        PressSpaceToContinue();

        while (1) {
            cout << "\n\tTurn # " << ++turnCount << "\t(" << trialGorilla.GetName() << " current health tracker: ";
            PrintTracker(trialGorilla.trackerHp);
            cout << ", Dead " << trialMortal.GetName() << "s: " << deadTrialMortalsCount << ")";

            //roll mutual attacks
            tempMortalSuccesses = trialMortal.RollAttackPool(trialMortalMod);
            tempGorillaSuccesses = trialGorilla.RollAttackPool(trialGorillaMod);

            tempMargin = abs(tempMortalSuccesses - tempGorillaSuccesses);   //find the margin
            tempDamage = tempMargin;

            if (tempMargin == 0) {                                  //draw, both deal 1 damage
                tempDamage = 1;
                tempDamToMortal = tempDamToGorilla = tempDamage;
            }
            else if (tempMortalSuccesses > tempGorillaSuccesses) {  //mortal win, deal damage to gorilla
                tempDamToGorilla = tempDamage;
                tempDamToMortal = 0;
            }
            else if (tempMortalSuccesses < tempGorillaSuccesses) {  //gorilla win, deal damage to mortal
                tempDamToMortal = tempDamage;
                tempDamToGorilla = 0;
            }
            else {
                cout << "uhhh, some kind of error";
                return 1;
            }

            if (trialMortal.GetDamageType() == superficial) {   //halve any superficial damage before dealing it
                tempDamToGorilla = HalveForSuperficial(tempDamToGorilla);
            }
            if (trialGorilla.GetDamageType() == superficial) {
                tempDamToMortal = HalveForSuperficial(tempDamToMortal);
            }

            trialMortal.DealDamage(tempDamToMortal, trialGorilla.GetDamageType(), trialMortal.trackerHp);   //deal the damage
            trialGorilla.DealDamage(tempDamToGorilla, trialMortal.GetDamageType(), trialGorilla.trackerHp);

            if (trialMortal.IsDead()) {                                             //remove impairement penalty when a new challenger steps up
                deadTrialMortalsCount++;
                trialMortal.ClearDamage(trialMortal.trackerHp);
                trialMortalImpaired = false;
                trialMortalMod = 0;
            }
            if (!trialMortalImpaired && trialMortal.GetStatusFlags().is_impaired) { //include impairmenet penalty once the challenger is hurt enough
                trialMortalImpaired = true;
                trialMortalMod += penalty_impaired;
            }

            if (trialGorilla.IsDead()) {    //end trial once gorilla dies
                goto GORILLA_HAS_DIED;
            }
            if (!trialGorillaImpaired && trialGorilla.GetStatusFlags().is_impaired) {   //include impairmenet penalty once the gorilla is hurt enough
                trialGorillaImpaired = true;
                trialGorillaMod += penalty_impaired;
            }

            //roll one-sided attacks
            for (int i = 0; i < trialMortalGangupSize - 1; i++) {
                tempMortalSuccesses = trialMortal.RollAttackPool(0);
                tempGorillaSuccesses = trialGorilla.RollDefendPool(trialGorillaMod - (i + 1));  //an additioanl -1 since 1st action was attacking

                if (tempMortalSuccesses > tempGorillaSuccesses) {
                    tempDamToGorilla = tempMortalSuccesses - tempGorillaSuccesses;
                }
                else if (tempMortalSuccesses == tempGorillaSuccesses) {
                    tempDamToGorilla = 1;
                }
                else {
                    tempDamToGorilla = 0;
                }

                if (trialMortal.GetDamageType() == superficial) {
                    tempDamToGorilla = HalveForSuperficial(tempDamToGorilla);
                }
                trialGorilla.DealDamage(tempDamToGorilla, trialMortal.GetDamageType(), trialGorilla.trackerHp);

                if (trialGorilla.IsDead()) {    //end trial once gorilla dies
                    goto GORILLA_HAS_DIED;
                }
                if (!trialGorillaImpaired && trialGorilla.GetStatusFlags().is_impaired) {//include impairmenet penalty once the gorilla is hurt enough
                    trialGorillaImpaired = true;
                    trialGorillaMod += penalty_impaired;
                }
            }
        }//loop through While loop until gorilla dies
    GORILLA_HAS_DIED:
        cout << "\n\t" << trialGorilla.GetName() << " has died! They took " << deadTrialMortalsCount << " " << trialMortal.GetName()
            << "s down with 'em!";
        PressSpaceToContinue();

        trialGorilla.ClearDamage(trialGorilla.trackerHp);
        trialMortal.ClearDamage(trialMortal.trackerHp);

        deadTrialMortalsCount = 0, trialMortalMod = 0, trialGorillaMod = 0;

        tempMargin = 0, tempDamage = 0, tempDamToMortal = 0, tempDamToGorilla = 0;

        turnCount = 0;

        trialMortalImpaired = false, trialGorillaImpaired = false;
        
        tempGorillaSuccesses = 0, tempMortalSuccesses = 0;
    }
    return 1;

#endif // TRIAL_5

#ifdef TRIAL_6
    int trialMortalGangupSize = numberOfMortalsAttacking;

    int numTrials = numberOfTrials;
    int movingTotalDeadMortals = 0;
    double meanDeadMortals;
    int minDeadMortals = 1'000'000;
    int maxDeadMortals = 0;

    int movingTotalTurnCount = 0;
    double meanTurnCount;
    int minTurnCount = 1'000'000;
    int maxTurnCount = 0;

    int deadTrialMortalsCount = 0;
    int trialMortalMod = 0;
    int trialGorillaMod = 0;

    int tempMargin = 0;
    int tempDamage = 0;
    int tempDamToMortal = 0, tempDamToGorilla = 0;

    int turnCount = 0;

    bool trialMortalImpaired = false, trialGorillaImpaired = false;
    const int penalty_impaired = -2;

    int tempGorillaSuccesses = 0;
    int tempMortalSuccesses = 0;

    while (1) {

        cout << "Running " << numTrials << " simulations, please hold...";
        cout << "\n\t(Fight lasts until " << trialGorilla.GetName() << "'s death, ganged up on by " << trialMortalGangupSize
            << " " << trialMortal.GetName() << "s at a time)";

        for (int x = 0; x < numTrials; x++) {
            while (1) {
                //cout << "\n\tTurn # " << ++turnCount << "\t(" << trialGorilla.GetName() << " current health tracker: ";
                //PrintTracker(trialGorilla.trackerHp);
                //cout << ", Dead " << trialMortal.GetName() << "s: " << deadTrialMortalsCount;
                turnCount++;

                //roll mutual attacks
                tempMortalSuccesses = trialMortal.RollAttackPool(trialMortalMod);
                tempGorillaSuccesses = trialGorilla.RollAttackPool(trialGorillaMod);

                tempMargin = abs(tempMortalSuccesses - tempGorillaSuccesses);   //find the margin
                tempDamage = tempMargin;

                if (tempMargin == 0) {                                  //draw, both deal 1 damage
                    tempDamage = 1;
                    tempDamToMortal = tempDamToGorilla = tempDamage;
                }
                else if (tempMortalSuccesses > tempGorillaSuccesses) {  //mortal win, deal damage to gorilla
                    tempDamToGorilla = tempDamage;
                    tempDamToMortal = 0;
                }
                else if (tempMortalSuccesses < tempGorillaSuccesses) {  //gorilla win, deal damage to mortal
                    tempDamToMortal = tempDamage;
                    tempDamToGorilla = 0;
                }
                else {
                    cout << "uhhh, some kind of error";
                    return 1;
                }

                if (trialMortal.GetDamageType() == superficial) {   //halve any superficial damage before dealing it
                    tempDamToGorilla = HalveForSuperficial(tempDamToGorilla);
                }
                if (trialGorilla.GetDamageType() == superficial) {
                    tempDamToMortal = HalveForSuperficial(tempDamToMortal);
                }

                trialMortal.DealDamage(tempDamToMortal, trialGorilla.GetDamageType(), trialMortal.trackerHp);   //deal the damage
                trialGorilla.DealDamage(tempDamToGorilla, trialMortal.GetDamageType(), trialGorilla.trackerHp);

                if (trialMortal.IsDead()) {                                             //remove impairement penalty when a new challenger steps up
                    deadTrialMortalsCount++;
                    trialMortal.ClearDamage(trialMortal.trackerHp);
                    trialMortalImpaired = false;
                    trialMortalMod = 0;
                }
                if (!trialMortalImpaired && trialMortal.GetStatusFlags().is_impaired) { //include impairmenet penalty once the challenger is hurt enough
                    trialMortalImpaired = true;
                    trialMortalMod += penalty_impaired;
                }

                if (trialGorilla.IsDead()) {    //end trial once gorilla dies
                    goto GORILLA_HAS_DIED2;
                }
                if (!trialGorillaImpaired && trialGorilla.GetStatusFlags().is_impaired) {   //include impairmenet penalty once the gorilla is hurt enough
                    trialGorillaImpaired = true;
                    trialGorillaMod += penalty_impaired;
                }

                //roll one-sided attacks
                for (int i = 0; i < trialMortalGangupSize - 1; i++) {
                    tempMortalSuccesses = trialMortal.RollAttackPool(0);
                    tempGorillaSuccesses = trialGorilla.RollDefendPool(trialGorillaMod - (i + 1));//additional -1 because 1st action is attacking

                    if (tempMortalSuccesses > tempGorillaSuccesses) {
                        tempDamToGorilla = tempMortalSuccesses - tempGorillaSuccesses;
                    }
                    else if (tempMortalSuccesses == tempGorillaSuccesses) {
                        tempDamToGorilla = 1;
                    }
                    else {
                        tempDamToGorilla = 0;
                    }

                    if (trialMortal.GetDamageType() == superficial) {
                        tempDamToGorilla = HalveForSuperficial(tempDamToGorilla);
                    }
                    trialGorilla.DealDamage(tempDamToGorilla, trialMortal.GetDamageType(), trialGorilla.trackerHp);

                    if (trialGorilla.IsDead()) {    //end trial once gorilla dies
                        goto GORILLA_HAS_DIED2;
                    }
                    if (!trialGorillaImpaired && trialGorilla.GetStatusFlags().is_impaired) {//include impairmenet penalty once the gorilla is hurt enough
                        trialGorillaImpaired = true;
                        trialGorillaMod += penalty_impaired;
                    }
                }
            }//loop through While loop until gorilla dies
        GORILLA_HAS_DIED2:
            movingTotalDeadMortals += deadTrialMortalsCount;
            movingTotalTurnCount += turnCount;

            if (deadTrialMortalsCount < minDeadMortals) {
                minDeadMortals = deadTrialMortalsCount;
            }
            else if (deadTrialMortalsCount > maxDeadMortals) {
                maxDeadMortals = deadTrialMortalsCount;
            }

            if (turnCount < minTurnCount) {
                minTurnCount = turnCount;
            }
            else if (turnCount > maxTurnCount) {
                maxTurnCount = turnCount;
            }

            trialMortalMod = 0, trialGorillaMod = 0;

            trialMortalImpaired = false, trialGorillaImpaired = false;
            trialGorilla.ClearDamage(trialGorilla.trackerHp);
            trialMortal.ClearDamage(trialMortal.trackerHp);

            deadTrialMortalsCount = turnCount = 0;
        }

        meanDeadMortals = (double)movingTotalDeadMortals / (double)numTrials;
        meanTurnCount = (double)movingTotalTurnCount / (double)numTrials;

        cout << "\n\n\tMean turn count to " << trialGorilla.GetName() << "'s death = " << meanTurnCount << ", mean dead "
            << trialMortal.GetName() << "s = " << meanDeadMortals << "\n\t\tMin turn count = " << minTurnCount << ", max turn count = "
            << maxTurnCount << "\n\t\tMin dead " << trialMortal.GetName() << "s = " << minDeadMortals << ", max dead "
            << trialMortal.GetName() << "s = " << maxDeadMortals << "\n";
        PressSpaceToContinue();
        cout << "\n";

        trialGorilla.ClearDamage(trialGorilla.trackerHp);
        trialMortal.ClearDamage(trialMortal.trackerHp);

        movingTotalDeadMortals = 0;
        minDeadMortals = 1'000'000, maxDeadMortals = 0;

        deadTrialMortalsCount = turnCount = 0;

        movingTotalTurnCount = 0;
        minTurnCount = 1'000'000, maxTurnCount = 0;

        trialMortalMod = 0, trialGorillaMod = 0;

        tempMargin = 0, tempDamage = 0;
        tempDamToMortal = 0, tempDamToGorilla = 0;

        turnCount = 0;

        trialMortalImpaired = false, trialGorillaImpaired = false;
    }
    return 1;
#endif // TRIAL_6
}

void PressSpaceToContinue()//I stole this from somewhere online, google it
{
    int c;
    cout<< "\tPress [SPACE] to continue...";
    fflush(stdout);
    do {
        c = _getch();
    } while (c != ' ');
    cout << "\n";
}


/// <summary>
/// Inspects a given dice pool for whether a willpower reroll should be done or not, 
/// return true = yes reroll, return false = do not reroll
/// </summary>
/// <param name="pool">: the dice pool to inspect for non-success dice</param>
/// <param name="threshold">: the # of failed dice needed in the pool to return true</param>
/// <param name="array">: an array to populate with flags for which dice to reroll</param>
/// <returns></returns>
bool WpCheck(V5DICE pool, int threshold, vector<bool>& array) {
    if (pool.get_scores().size() <= 0) {    //can quit out ASAP if no dice to inspect
        return false;
    }
    int tempCount = 0;
    array.clear();
    for (int i = 0; i < pool.get_scores().size(); i++) {
        if (pool.get_results().at(i) == botch || pool.get_results().at(i) == fail) {
            tempCount++;
            array.push_back(true);
        }
        else {
            array.push_back(false);
        }
        if (tempCount >= threshold) {
            return true;
        }
    }
    return false;
}

int HalveForSuperficial(int dam) {
    if (dam <= 0) {
        return 0;
    }
    if (dam % 2 == 0) { //no roudning needed
        return dam / 2;
    }
    else {              //round up always
        return (dam + 1) / 2;
    }
}

const char dam_char_empty = 'o';//'_'
const char dam_char_sp = '/';
const char dam_char_agg = 'X';

void PrintTracker(v5char_tracker_t tracker) {
    vector<char> printable(tracker.max, dam_char_empty);
    
    for (int i = 0; i < tracker.damSp; i++) {
        printable.at(i) = dam_char_sp;
    }

    if (tracker.damAgg > tracker.max) {
        tracker.damAgg = tracker.max;
    }

    for (int j = 0; j < tracker.damAgg; j++) {
        printable.at(tracker.max - 1 - j) = dam_char_agg;
    }

    for (int k = 0; k < printable.size(); k++) {
        cout << printable.at(k);
    }
    return;
}
