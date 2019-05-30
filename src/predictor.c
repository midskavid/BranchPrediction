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
const char *studentName = "MridulKavidayal;PiyushTayal";
const char *studentID   = "A53283690;A53285621";
const char *email       = "mkaviday;ptayal";

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

uint8_t *globalPredictionTable;
uint8_t *localPredictionTable;
uint8_t *selector;
uint32_t *localHistoryTable;
uint32_t globalHistory;

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
      globalPredictionTable = (uint8_t*)malloc((int)pow(2.0,(double)ghistoryBits)*sizeof(uint8_t));
      
      // Initialize all to one..
      int len = (int)pow(2.0,(double)ghistoryBits);
      for (int ii = 0; ii < len; ++ii)
        globalPredictionTable[ii] = 1;
      globalHistory = 0;
      return;
    }
    case TOURNAMENT :
    {
      globalPredictionTable = (uint8_t*)malloc((int)pow(2.0,(double)ghistoryBits)*sizeof(uint8_t));
      localPredictionTable = (uint8_t*)malloc((int)pow(2.0,(double)lhistoryBits)*sizeof(uint8_t));
      localHistoryTable = (uint32_t*)malloc((int)pow(2.0,(double)pcIndexBits)*sizeof(uint32_t));
      selector = (uint8_t*)malloc((int)pow(2.0,(double)ghistoryBits)*sizeof(uint8_t));
      // initialize all to 1..
      int len = (int)pow(2.0,(double)ghistoryBits);
      for (int ii = 0; ii < len; ++ii){
        globalPredictionTable[ii] = 1;
        selector[ii] = 2; // Check what happens with other values!!!
      }

      len = (int)pow(2.0,(double)lhistoryBits);
      for (int ii = 0; ii < len; ++ii)
        localPredictionTable[ii] = 1;

      len = (int)pow(2.0,(double)pcIndexBits);
      for (int ii = 0; ii < len; ++ii)
        localHistoryTable[ii] = 0;
      globalHistory = 0;
      return;
    }
    case CUSTOM:
    {
      globalPredictionTable = (uint8_t*)malloc((int)pow(2.0,(double)ghistoryBits)*sizeof(uint8_t));
      localPredictionTable = (uint8_t*)malloc((int)pow(2.0,(double)lhistoryBits)*sizeof(uint8_t));
      localHistoryTable = (uint32_t*)malloc((int)pow(2.0,(double)pcIndexBits)*sizeof(uint32_t));
      selector = (uint8_t*)malloc((int)pow(2.0,(double)ghistoryBits)*sizeof(uint8_t));
      // initialize all to 1..
      int len = (int)pow(2.0,(double)ghistoryBits);
      for (int ii = 0; ii < len; ++ii){
        globalPredictionTable[ii] = 1;
        selector[ii] = 2; // Check what happens with other values!!!
      }

      len = (int)pow(2.0,(double)lhistoryBits);
      for (int ii = 0; ii < len; ++ii)
        localPredictionTable[ii] = 1;

      len = (int)pow(2.0,(double)pcIndexBits);
      for (int ii = 0; ii < len; ++ii)
        localHistoryTable[ii] = 0;
      globalHistory = 0;
      return;      
    }
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
      return ((globalPredictionTable[indexTable] & (1 << 1)) >> 1) ? TAKEN : NOTTAKEN;
      break;
    }
    case TOURNAMENT:
    {
      // printf("%djojojoj\n", (pc & ((1 << pcIndexBits) - 1)));
      // printf("%d\n", localHistoryTable[(pc & ((1 << pcIndexBits) - 1))]);
      // printf("%d\n", ((1 << lhistoryBits) - 1));
      // printf("***********\n");
      int indexTable = localHistoryTable[pc & ((1 << pcIndexBits) - 1)] & ((1 << lhistoryBits) - 1);
      int localPrediction = localPredictionTable[indexTable];
      int globalPrediction = globalPredictionTable[globalHistory & ((1 << ghistoryBits) - 1)];
      if ((selector[globalHistory & ((1 << ghistoryBits) - 1)] & (1 << 1)) >> 1){
        // Global Prediction..
        return ((globalPrediction & (1 << 1)) >> 1) ? TAKEN : NOTTAKEN;
      }
      else{
        // Local Prediction..
        return ((localPrediction & (1 << 1)) >> 1) ? TAKEN : NOTTAKEN;
      }
      break;
    }
    case CUSTOM:
    {
      int indexTable = localHistoryTable[pc & ((1 << pcIndexBits) - 1)] & ((1 << lhistoryBits) - 1);
      int localPrediction = localPredictionTable[indexTable];
      indexTable = (int)((pc & ((1 << ghistoryBits) - 1)) ^ (globalHistory & ((1 << ghistoryBits) - 1)));
      int globalPrediction = globalPredictionTable[indexTable];
      if ((selector[globalHistory & ((1 << ghistoryBits) - 1)] & (1 << 1)) >> 1){
        // Global Prediction..
        return ((globalPrediction & (1 << 1)) >> 1) ? TAKEN : NOTTAKEN;
      }
      else{
        // Local Prediction..
        return ((localPrediction & (1 << 1)) >> 1) ? TAKEN : NOTTAKEN;
      }
      break;
    }
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
      if (globalPredictionTable[indexTable] == SN) {
        if (outcome)
          globalPredictionTable[indexTable] += 1;
      }
      else if (globalPredictionTable[indexTable] == ST) {
        if (!outcome) {
          globalPredictionTable[indexTable] -= 1;
        }
      }
      else
        globalPredictionTable[indexTable] += outcome ? 1 : -1;
      break;
    }
    case TOURNAMENT:
    {
      int indexTable = localHistoryTable[(int)(pc & ((1 << pcIndexBits) - 1))] & ((1 << lhistoryBits) - 1);

      int localPrediction = localPredictionTable[indexTable];
      int globalPrediction = globalPredictionTable[globalHistory & ((1 << ghistoryBits) - 1)];

      if (outcome){
        // Global Prediction..
        if ((globalPrediction & (1 << 1)) != (localPrediction & (1 << 1))){
          // check who predicted..
          if ((globalPrediction > 1) && (selector[globalHistory & ((1 << ghistoryBits) - 1)]) != 3)
            selector[globalHistory & ((1 << ghistoryBits) - 1)]++;
          else if ((localPrediction > 1 ) && (selector[globalHistory & ((1 << ghistoryBits) - 1)] !=0))
            selector[globalHistory & ((1 << ghistoryBits) - 1)]--;
        }
        if (localPrediction != 3)
          ++localPredictionTable[indexTable];
        if (globalPrediction != 3)
          ++globalPredictionTable[globalHistory & ((1 << ghistoryBits) - 1)];
      }
      else{
        // Local Prediction..
        if ((globalPrediction & (1 << 1)) != (localPrediction & (1 << 1))){
          // check who predicted..
          if ((globalPrediction <= 1) && (selector[globalHistory & ((1 << ghistoryBits) - 1)]) != 3)
            selector[globalHistory & ((1 << ghistoryBits) - 1)]++;
          else if ((localPrediction <= 1 ) && (selector[globalHistory & ((1 << ghistoryBits) - 1)] !=0))
            selector[globalHistory & ((1 << ghistoryBits) - 1)]--;
        }
        if (localPrediction != 0)
          --localPredictionTable[indexTable];
        if (globalPrediction != 0)
          --globalPredictionTable[globalHistory & ((1 << ghistoryBits) - 1)];
      }
      globalHistory = (globalHistory << 1) | outcome; 
      // Update localHistoryTable
      localHistoryTable[pc & ((1 << pcIndexBits) - 1)] = (localHistoryTable[pc & ((1 << pcIndexBits) - 1)] << 1) | outcome; 
      break;    
    }
    case CUSTOM:
    {
      int indexLocal = localHistoryTable[(int)(pc & ((1 << pcIndexBits) - 1))] & ((1 << lhistoryBits) - 1);

      int localPrediction = localPredictionTable[indexLocal];
      int indexGlobal = (int)((pc & ((1 << ghistoryBits) - 1)) ^ (globalHistory & ((1 << ghistoryBits) - 1)));
      int globalPrediction = globalPredictionTable[indexGlobal];
      if (outcome){
        // Global Prediction..
        if ((globalPrediction & (1 << 1)) != (localPrediction & (1 << 1))){
          // check who predicted..
          if ((globalPrediction > 1) && (selector[globalHistory & ((1 << ghistoryBits) - 1)]) != 3)
            selector[globalHistory & ((1 << ghistoryBits) - 1)]++;
          else if ((localPrediction > 1 ) && (selector[globalHistory & ((1 << ghistoryBits) - 1)] !=0))
            selector[globalHistory & ((1 << ghistoryBits) - 1)]--;
        }
        if (localPrediction != 3)
          ++localPredictionTable[indexLocal];
        if (globalPrediction != 3)
          ++globalPredictionTable[indexGlobal];
      }
      else{
        // Local Prediction..
        if ((globalPrediction & (1 << 1)) != (localPrediction & (1 << 1))){
          // check who predicted..
          if ((globalPrediction <= 1) && (selector[globalHistory & ((1 << ghistoryBits) - 1)]) != 3)
            selector[globalHistory & ((1 << ghistoryBits) - 1)]++;
          else if ((localPrediction <= 1 ) && (selector[globalHistory & ((1 << ghistoryBits) - 1)] !=0))
            selector[globalHistory & ((1 << ghistoryBits) - 1)]--;
        }
        if (localPrediction != 0)
          --localPredictionTable[indexLocal];
        if (globalPrediction != 0)
          --globalPredictionTable[indexGlobal];
      }
      globalHistory = (globalHistory << 1) | outcome; 
      // Update localHistoryTable
      localHistoryTable[pc & ((1 << pcIndexBits) - 1)] = (localHistoryTable[pc & ((1 << pcIndexBits) - 1)] << 1) | outcome; 
      break;
    }
    default:
      break;
  }  
}

void delete_predictor(){
  switch (bpType) {
    case STATIC :
      return;
    case GSHARE : 
      free(globalPredictionTable);
      return;
    case CUSTOM:
    case TOURNAMENT :
      free(globalPredictionTable);
      free(localPredictionTable);
      free(localHistoryTable);
      free(selector);
      return;
  }
}