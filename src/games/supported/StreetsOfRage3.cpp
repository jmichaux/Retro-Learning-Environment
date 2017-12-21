/* *****************************************************************************
 * A.L.E (Arcade Learning Environment)
 * Copyright (c) 2009-2013 by Yavar Naddaf, Joel Veness, Marc G. Bellemare and
 *   the Reinforcement Learning and Artificial Intelligence Laboratory
 * Released under the GNU General Public License; see License.txt for details.
 *
 * Based on: Stella  --  "An Atari 2600 VCS Emulator"
 * Copyright (c) 1995-2007 by Bradford W. Mott and the Stella team
 *
 * *****************************************************************************
 */
#include "../RomUtils.hpp"
#include <iomanip>

#include "../RomUtils.hpp"

#include "RleSystem.hxx"
#include "RleException.h"

#include "StreetsOfRage3.hpp"
#include <iostream>


using namespace rle;


StreetsOfRage3Settings::StreetsOfRage3Settings() {
    reset();


 
      minimalActions = {JOYPAD_NOOP,
    			// Moving
//          JOYPAD_DOWN,      //Walk down
//          JOYPAD_UP,        // Walk up
//					JOYPAD_LEFT,      // Walk left
//					JOYPAD_RIGHT,     // Walk right
//         	JOYPAD_GENESIS_C, // Jump
//          JOYPAD_LEFT | JOYPAD_GENESIS_C,
//          JOYPAD_RIGHT | JOYPAD_GENESIS_C,

		      // Special attacks
//					JOYPAD_GENESIS_A, 
//          JOYPAD_LEFT | JOYPAD_GENESIS_A,
//          JOYPAD_RIGHT |JOYPAD_GENESIS_A,

          // Regular Attacks
	    		JOYPAD_GENESIS_B, 
//					JOYPAD_GENESIS_B | JOYPAD_GENESIS_C,   // Rear attack or Super slam 
				  
          // Attacks when holding enemy
//          JOYPAD_GENESIS_B | JOYPAD_LEFT,  
//				  JOYPAD_GENESIS_B | JOYPAD_RIGHT,
    };
}


/* create a new instance of the rom */
RomSettings* StreetsOfRage3Settings::clone() const {
    RomSettings* rval = new StreetsOfRage3Settings();
    *rval = *this;
    return rval;
}


void StreetsOfRage3Settings::step(const RleSystem& system) {
// Begin code for testing
  if(system.settings()->getBool("SOR3_test") == true){

      // Fix Agent health
      writeRam(&system, 0xDF6C, 0x34);

      // Make invincible
      writeRam(&system, 0xDF5C, 0xFF);
  
      // Freeze Time
     // writeRam(&system, 0xFC3C, 0x99);
}

// This setting gives all enemies minimal health and lives
  if((system.settings()->getBool("SOR3_test") == true) || system.settings()->getInt("SOR3_difficulty") == 0){
      // Fix enemy health and lives
      writeRam(&system, 0xE16C, 0x0);
      writeRam(&system, 0xE26C, 0x0);
      writeRam(&system, 0xE36C, 0x0);
      writeRam(&system, 0xE46C, 0x0);
      writeRam(&system, 0xE56C, 0x0);
      writeRam(&system, 0xE18A, 0x0);
      writeRam(&system, 0xE28A, 0x0);
      writeRam(&system, 0xE38A, 0x0);
      writeRam(&system, 0xE48A, 0x0);
      writeRam(&system, 0xE58A, 0x0);
}
// End code for testing


//  Read out current score, health, lives, kills  
  // Score set to 0. Will read score from rle/gym wrapper
  reward_t score = 0; //getDecimalScore(0xEF99, 0xEF96, &system);	
  m_lives = readRam(&system, 0xDF8A);
  m_health = readRam(&system, 0xDF6D);
 
//  std::cout << readRam(&system, 0xFB02) << std::endl;

//  update the reward
  m_reward = score - m_score;
  m_score = score;
//	Update terminal status
  if ((readRam(&system, 0xDFA1) == 255) && readRam(&system, 0xFB00) == 14){
      std::cout << "LOST GAME" << std::endl;
      m_terminal = true;
    }
	

// Get level information
  m_current_level = readRam(&system, 0xFB04) + 1;
	//std::cout << m_current_level << std::endl;
  m_end_level = system.settings()->getInt("SOR3_end_level");
  if((m_end_level < system.settings()->getInt("SOR3_start_level")) || (m_end_level > 8)){
     m_end_level = system.settings()->getInt("SOR3_start_level");
  }

  int m_progress_1 = readRam(&system, 0xDF40); // distance 1
  int m_progress_2 = readRam(&system, 0xFED0); // distance 2
  int m_progress_3 = readRam(&system, 0xDF02); // distance 3
  int scene = readRam(&system, 0xFB05); // scene 

  // Level 1
  if (m_end_level == 1){
       if ((m_current_level == 1) && (scene == 2) && (m_progress_1 == 0) && (m_progress_2 == 25) && (m_progress_3 > 400)){
           m_terminal = true;
       }
  }// Level 2
  else if (m_end_level == 2){
       if ((m_current_level == 2) && (scene == 2) && (m_progress_1 == 0) && (m_progress_2 == 25) && (m_progress_3 > 128)){
           m_terminal = true;
       }
  }// Level 3
  else if (m_end_level == 3){ 
      if ((m_current_level == 3) && (scene == 2) && (m_progress_1 == 0) && (m_progress_2 == 25) && (m_progress_3 > 400)){
           m_terminal = true;
      }      
  }// Level 4
  else if (m_end_level == 4){
      if ((m_current_level == 4) && (scene == 2) && (m_progress_1 == 0) && (m_progress_2 == 25) && (m_progress_3 > 300)){
           m_terminal = true;
      }
  }// Level 5
  else if (m_end_level == 5){ 
      if ((m_current_level == 5) && (scene == 7) && (m_progress_1 == 0) && (m_progress_2 == 25) && (m_progress_3 > 30)){
           m_terminal = true;
       }
  }// Level 6
  else if (m_end_level == 6){
      if ((m_current_level == 6) && (scene == 7) && (m_progress_1 == 0) && (m_progress_2 == 25) && (m_progress_3 > 200)){
           m_terminal = true;
       }
  }// Level 7
  else if (m_end_level == 7){
      if ((m_current_level == 7) && (scene == 4) && (m_progress_1 == 0) && (m_progress_2 == 25) && (m_progress_3 > 30)){
           m_terminal = true;
       }
  }// Level 8 (Alternate level 7)
  else if (m_end_level == 8){
      if ((m_current_level == 8) && (scene == 3) && (m_progress_1 == 0) && (m_progress_2 == 116) &&  (m_progress_3 > 30)){
           m_terminal = true;
       }
  } 
}



/* reset the state of the game */
void StreetsOfRage3Settings::reset() {
    m_reward   = 0;
    m_score = 0;
    m_terminal = false;
}


/* saves the state of the rom settings */
void StreetsOfRage3Settings::saveState( Serializer & ser ) {
  ser.putInt(m_reward);
  ser.putInt(m_score);
  ser.putBool(m_terminal);
}


// loads the state of the rom settings
void StreetsOfRage3Settings::loadState( Deserializer & des ) {
  m_reward = des.getInt();
  m_score = des.getInt();
  m_terminal = des.getBool();
}


ActionVect StreetsOfRage3Settings::getStartingActions(const RleSystem& system){
	int num_of_nops(100);
	ActionVect startingActions;

	int scene = readRam(&system, 0xFB00);
	int start_level = system.settings()->getInt("SOR3_start_level");

// Wait for intro to end
	if(scene > 0){
		INSERT_NOPS(7 * num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
		INSERT_NOPS(1.8 * num_of_nops)
	}else{
		INSERT_NOPS(9 * num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
		INSERT_NOPS(1.8 * num_of_nops)
	}	
	// Enter code for difficulty and level select (Enter options)
	INSERT_ACTION_SINGLE_A(JOYPAD_UP)
	INSERT_NOPS(0.5 * num_of_nops)
	INSERT_ACTION_SINGLE_A(JOYPAD_UP | JOYPAD_GENESIS_B)
	INSERT_ACTION_SINGLE_A(JOYPAD_UP | JOYPAD_GENESIS_B | JOYPAD_START)
	INSERT_NOPS(2 * num_of_nops)

	// Select difficulty
	m_difficulty = system.settings()->getInt("SOR3_difficulty");
	if((m_difficulty == 0) || (m_difficulty == 1) || (m_difficulty == 2)){
		INSERT_ACTION_SINGLE_A(JOYPAD_LEFT)
	}else if((m_difficulty == 4) || (m_difficulty == 5)){
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
	}
		
	// Select level
	INSERT_ACTION_SINGLE_A(JOYPAD_UP)
	if((start_level > 1) && (start_level <= 6)){
		INSERT_NOPS(0.5 * num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_UP)
		for(int i = 1; i <= start_level-1; ++i){
			INSERT_NOPS(0.5 * num_of_nops)
			INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		}
		INSERT_NOPS(0.5 * num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_DOWN)
	}

	// Exit to start menu
		INSERT_NOPS(0.5 * num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
		INSERT_NOPS(2 * num_of_nops)
	
	// Select number of agents (TODO add support for 2 players)
		INSERT_ACTION_SINGLE_A(JOYPAD_UP)
		INSERT_NOPS(0.5 * num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_UP)
	
		
	INSERT_ACTION_SINGLE_A(JOYPAD_START)
	INSERT_NOPS(2 * num_of_nops)

// Choose Player 1 character
	string player_1_character = system.settings()->getString("SOR3_player_1_character");
	if("axel" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("zan" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_LEFT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("blaze" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
	}else if("skate" == player_1_character){
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_RIGHT)
		INSERT_ACTION_SINGLE_A(JOYPAD_NOOP)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
 	} 


// Wait for level to start.
// Note that these numbers were tuned to minimize beginning of level scenes 
	if(start_level == 1){
		INSERT_NOPS(3 * num_of_nops)
		INSERT_ACTION_SINGLE_A(JOYPAD_START)
		INSERT_NOPS(4 *  num_of_nops)
	}else if(start_level == 2){
		INSERT_NOPS(3.5 * num_of_nops)
	}else if(start_level == 3){
		INSERT_NOPS(4.6 * num_of_nops)
	}else if(start_level == 4){
		INSERT_NOPS(3.7 * num_of_nops)
	}else if(start_level == 5){
		INSERT_NOPS(3.7 * num_of_nops)
	}else if(start_level == 6){
		INSERT_NOPS(3.7 * num_of_nops)
	}else if(start_level == 7){
		INSERT_NOPS(0.36 * num_of_nops)
	}else if(start_level == 8){
		INSERT_NOPS(0.36 * num_of_nops)
	}
	return startingActions;

}

void StreetsOfRage3Settings::startingOperations(RleSystem& system){
	/*Note that writing to RAM only works for some of the addresses below
	BEFORE the first reset. Writing to all addresses works AFTER the first reset.
	This should speed up the starting actions for every episode after the first reset.*/
	
	//set difficulty
	m_difficulty = system.settings()->getInt("SOR3_difficulty");
    // Also setting enemy health low (see above).
  	if(0 == m_difficulty){
		writeRam(&system, 0xFF08, 0x2);
	}else if(1 == m_difficulty){
		writeRam(&system, 0xFF08, 0x2);
	}else if(2 == m_difficulty){
		writeRam(&system, 0xFF08, 0x4);
	}else if(3 == m_difficulty){
		writeRam(&system, 0xFF08, 0x6);
	}else if(4 == m_difficulty){
		writeRam(&system, 0xFF08, 0x8);
	}else if(5 == m_difficulty){
		writeRam(&system, 0xFF08, 0x0A);
	}

	//set number of continues. By Default continues set to zero.
	writeRam(&system,0xDFA0, 0x1); 

	//set start level	
	m_start_level = system.settings()->getInt("SOR3_start_level");
//	if((m_start_level <= 1) || (m_start_level > 7)){
//		std::cout << "Start level out of bounds. Starting at level 1" << std::endl;
//		writeRam(&system, 0xFB04, 0x0);
//	}else {
//		writeRam(&system, 0xFB04, (m_start_level - 1) * 0x01);
//	}
	if(m_start_level == 7){
		writeRam(&system, 0xFB04, 0x6);
		writeRam(&system, 0xFB02, 0x1);
	}
	if(m_start_level == 8){
		writeRam(&system, 0xFB04, 0x7);
		writeRam(&system, 0xFB02, 0x1);
	}

	//set number of lives 
	//(This sets the number of lives for the game. Would need to do 
	//this differently if you wanted to set the number of lives differently in
	//2 player mode.)
	m_lives = system.settings()->getInt("SOR3_lives");
	if((m_lives < 0) || (m_lives > 9)){
		std::cout << "Number of lives must be between 1 and 9. Initializing agent with 3 lives" << std::endl;
		writeRam(&system, 0xDF8A, 0x2);
	}else {
		writeRam(&system, 0xDF8A, 0x0);
	}
}


