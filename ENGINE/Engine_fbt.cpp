/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: Engine
 *** Description: Simple FB with one algorithm
 *** Version:
 ***     1.0: 2025-07-18/vadim -  -
 *************************************************************************/

#include "Engine_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "Engine_fbt_gen.cpp"
#endif

#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

#include "MainEngine.h"
#include <iostream>

DEFINE_FIRMWARE_FB(FORTE_Engine, g_nStringIdEngine)

const CStringDictionary::TStringId FORTE_Engine::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdDuration, g_nStringIdMovement};
const CStringDictionary::TStringId FORTE_Engine::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdINT, g_nStringIdSTRING};
const CStringDictionary::TStringId FORTE_Engine::scmDataOutputNames[] = {g_nStringIdQO};
const CStringDictionary::TStringId FORTE_Engine::scmDataOutputTypeIds[] = {g_nStringIdBOOL};
const TDataIOID FORTE_Engine::scmEIWith[] = {1, 2, 0, scmWithListDelimiter};
const TForteInt16 FORTE_Engine::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_Engine::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_Engine::scmEOWith[] = {0, scmWithListDelimiter};
const TForteInt16 FORTE_Engine::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_Engine::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_Engine::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  3, scmDataInputNames, scmDataInputTypeIds,
  1, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_Engine::FORTE_Engine(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_QI(0_BOOL),
    var_Duration(0_INT),
    var_Movement(""_STRING),
    var_QO(0_BOOL),
    var_conn_QO(var_QO),
    conn_CNF(this, 0),
    conn_QI(nullptr),
    conn_Duration(nullptr),
    conn_Movement(nullptr),
    conn_QO(this, 0, &var_conn_QO) {
}

void FORTE_Engine::setInitialValues() {
  var_QI = 0_BOOL;
  var_Duration = 0_INT;
  var_Movement = ""_STRING;
  var_QO = 0_BOOL;
}

void FORTE_Engine::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_Engine::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_Engine::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(1, var_Duration, conn_Duration);
      readData(2, var_Movement, conn_Movement);
      readData(0, var_QI, conn_QI);
      break;
    }
    default:
      break;
  }
}

void FORTE_Engine::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(0, var_QO, conn_QO);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_Engine::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_Duration;
    case 2: return &var_Movement;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Engine::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
  }
  return nullptr;
}

CEventConnection *FORTE_Engine::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_Engine::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_Duration;
    case 2: return &conn_Movement;
  }
  return nullptr;
}

CDataConnection *FORTE_Engine::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
  }
  return nullptr;
}

CIEC_ANY *FORTE_Engine::getVarInternal(size_t) {
  return nullptr;
}

using namespace std;

void FORTE_Engine::alg_REQ(void) {
  if(!var_QI){
    var_QO = CIEC_BOOL(false);
    return;
  }
  MainEngine Engine;
  auto exec = Engine.cmd_list[var_Movement.getStorage()];
  exec(var_Duration.getSignedValue());
  var_QO = CIEC_BOOL(true);
  return;
}
