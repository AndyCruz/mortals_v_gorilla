#pragma once    //pragma balls
#include "v5char.h"

V5CHAR::V5CHAR() {
	charName.clear();
	
	trackerHp.max = trackerWp.max = trackerHp.damSp = trackerWp.damSp = trackerHp.damAgg = trackerWp.damAgg = 0;

	attackPool = defendPool = 0;

	attackType = none;

	charDice.clear_dice();

	deadFlag = false;

	statusFlags = { false };
}

V5CHAR::V5CHAR(string name, int max_hp, int max_wp, int pool_off, int pool_def, v5char_damage_types_enum dam_type){   //constructor
	charName = name;
	
	trackerHp.max = max_hp;
	trackerWp.max = max_wp;
	trackerHp.damSp = trackerHp.damAgg = trackerWp.damSp = trackerWp.damAgg = 0;

	attackPool = pool_off;
	defendPool = pool_def;

	attackType = dam_type;

	charDice.clear_dice();

	deadFlag = false;
	statusFlags = { false };

}

V5CHAR::~V5CHAR() {
}

void V5CHAR::DealDamage(int dam, v5char_damage_types_enum dam_type, v5char_tracker_t& tracker) {
	//superficial damage is added starting from the left, aggravated damage is added starting from the right
	
	if (dam <= 0 || dam_type == none) {	//don't waste my time
		return;
	}

	int emptySlots = tracker.max - (tracker.damSp + tracker.damAgg);	//# of empty slots in the tracker

	//handle superficial damage
	if(dam_type == superficial) {
		if (dam <= emptySlots) {
			tracker.damSp += dam;
			if (dam == emptySlots) {
				statusFlags.is_impaired = true;
			}
			return;
		}
		tracker.damSp += emptySlots;//fill the empty slots w/ superficial damage
		//dam_type = aggravated;		//convert damage type to aggravated
		dam -= emptySlots;			//lower damage total by amount already dealt
		emptySlots = 0;				//let the aggravated logic for overwriting superficial below handle the overkill
		statusFlags.is_impaired = true;
	}

	//handle aggravated damage
	if (dam >= emptySlots) {
		tracker.damSp -= dam - emptySlots;	//aggravated overwrites superficial when they meet
		if (dam == emptySlots) {
			statusFlags.is_impaired = true;
		}
	}
	tracker.damAgg += dam;

	if (tracker.damAgg >= tracker.max) {	//if the whole tracker fills up with aggravated, the character dies
		tracker.damSp = 0;
		deadFlag = true;
		statusFlags.is_dead = true;
	}

	return;
}

void V5CHAR::ClearDamage(v5char_tracker_t& tracker) {
	tracker.damAgg = tracker.damSp = 0;
	deadFlag = false;
	statusFlags.is_dead = statusFlags.is_impaired = false;
	return;
}

v5char_tracker_t V5CHAR::GetHp() {
	return trackerHp;
}

v5char_tracker_t V5CHAR::GetWp() {
	return trackerWp;
}

string V5CHAR::GetName() {
	return charName;
}

int V5CHAR::GetAttackPool() {
	return attackPool;
}

int V5CHAR::GetDefendPool() {
	return defendPool;
}

v5char_damage_types_enum V5CHAR::GetDamageType() {
	return attackType;
}

v5char_status_flags V5CHAR::GetStatusFlags() {
	return statusFlags;
}

int V5CHAR::RollAttackPool(int mod) {
	if (attackPool + mod < 1) {
		mod = 1 - attackPool;
	}
	charDice.RollDice(attackPool + mod);
	return charDice.get_num_successes();
}

int V5CHAR::RollDefendPool(int mod) {
	if (defendPool + mod < 1) {
		mod = 1 - defendPool;
	}
	charDice.RollDice(defendPool + mod);
	return charDice.get_num_successes();
}

bool V5CHAR::IsDead() {
	return deadFlag;
}

V5DICE V5CHAR::GetDiceRaw() {
	return charDice;
}