//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include <stdio.h>
#include <math.h>
#include "predictor.h"

//
// TODO:Student Information
//
const char *studentName = "NAME";
const char *studentID   = "PID";
const char *email       = "EMAIL";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = { "Static", "Gshare",
                          "Tournament", "Custom" };

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;
unsigned int *historyTable;
unsigned int globalHistory;
//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
//TODO: Add your own Branch Predictor data structures here
//


//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void
init_predictor()
{
  //
  //TODO: Initialize Branch Predictor Data Structures
  //
  switch (bpType) {
    case STATIC :
      return;
    case GSHARE : 
    {
      historyTable = (unsigned int*)malloc((int)pow(2.0,(double)ghistoryBits)*sizeof(unsigned int));
      int len = (int)pow(2.0,(double)ghistoryBits);
      for (int ii = 0; ii < len; ++ii)
        historyTable[ii] = 0;
      globalHistory = 0;
      return;
    }
    case TOURNAMENT :
      return;
  }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t
make_prediction(uint32_t pc)
{
  //
  //TODO: Implement prediction scheme
  //

  // Make a prediction based on the bpType
  switch (bpType) {
    case STATIC:
      return TAKEN;
    case GSHARE:
    {
      // XOR and then index onto the table....
      // Is is MSB or LSB
      int indexTable = (int)((pc & ((1 << ghistoryBits) - 1)) ^ (globalHistory & ((1 << ghistoryBits) - 1)));
      return ((historyTable[indexTable] & (1 << 1)) >> 1) ? TAKEN : NOTTAKEN;
      break;
    }
    case TOURNAMENT:
    case CUSTOM:
    default:
      break;
  }

  // If there is not a compatable bpType then return NOTTAKEN
  return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void
train_predictor(uint32_t pc, uint8_t outcome)
{
  //
  //TODO: Implement Predictor training
  //
  switch (bpType) {
    case STATIC:
      return;
    case GSHARE:
    {
      int indexTable = (int)((pc & ((1 << ghistoryBits) - 1)) ^ (globalHistory & ((1 << ghistoryBits) - 1)));
      globalHistory = (globalHistory << 1) | outcome;
      if (historyTable[indexTable] == SN) {
        if (outcome)
          historyTable[indexTable] += 1;
      }
      else if (historyTable[indexTable] == ST) {
        if (!outcome) {
          historyTable[indexTable] -= 1;
        }
      }
      else
        historyTable[indexTable] += outcome ? 1 : -1;
      break;
    }
    case TOURNAMENT:
    case CUSTOM:
    default:
      break;
  }  
}

void delete_predictor(){
  switch (bpType) {
    case STATIC :
      return;
    case GSHARE : 
      free(historyTable);
      return;
    case TOURNAMENT :
      return;
  }
}