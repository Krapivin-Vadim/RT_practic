/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x NG!
 ***
 *** Name: OPC_Client
 *** Description: Simple FB with one algorithm
 *** Version:
 ***     1.0: 2025-07-18/vadim -  -
 *************************************************************************/

#include "OPC_Client_fbt.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OPC_Client_fbt_gen.cpp"
#endif

#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

#include "open62541.h"
#include <iostream>
#include <string>
#include <sstream>

DEFINE_FIRMWARE_FB(FORTE_OPC_Client, g_nStringIdOPC_Client)

const CStringDictionary::TStringId FORTE_OPC_Client::scmDataInputNames[] = {g_nStringIdQI, g_nStringIdns_1, g_nStringIdi_1, g_nStringIdns_2, g_nStringIdi_2, g_nStringIdURL};
const CStringDictionary::TStringId FORTE_OPC_Client::scmDataInputTypeIds[] = {g_nStringIdBOOL, g_nStringIdINT, g_nStringIdINT, g_nStringIdINT, g_nStringIdINT, g_nStringIdSTRING};
const CStringDictionary::TStringId FORTE_OPC_Client::scmDataOutputNames[] = {g_nStringIdQO, g_nStringIdMovement, g_nStringIdDuration, g_nStringIdEnable};
const CStringDictionary::TStringId FORTE_OPC_Client::scmDataOutputTypeIds[] = {g_nStringIdBOOL, g_nStringIdSTRING, g_nStringIdINT, g_nStringIdBOOL};
const TDataIOID FORTE_OPC_Client::scmEIWith[] = {1, 2, 3, 4, 0, 5, scmWithListDelimiter};
const TForteInt16 FORTE_OPC_Client::scmEIWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_OPC_Client::scmEventInputNames[] = {g_nStringIdREQ};
const TDataIOID FORTE_OPC_Client::scmEOWith[] = {1, 2, 0, 3, scmWithListDelimiter};
const TForteInt16 FORTE_OPC_Client::scmEOWithIndexes[] = {0};
const CStringDictionary::TStringId FORTE_OPC_Client::scmEventOutputNames[] = {g_nStringIdCNF};
const SFBInterfaceSpec FORTE_OPC_Client::scmFBInterfaceSpec = {
  1, scmEventInputNames, nullptr, scmEIWith, scmEIWithIndexes,
  1, scmEventOutputNames, nullptr, scmEOWith, scmEOWithIndexes,
  6, scmDataInputNames, scmDataInputTypeIds,
  4, scmDataOutputNames, scmDataOutputTypeIds,
  0, nullptr,
  0, nullptr
};

FORTE_OPC_Client::FORTE_OPC_Client(const CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer) :
    CSimpleFB(paContainer, scmFBInterfaceSpec, paInstanceNameId, nullptr),
    var_QI(0_BOOL),
    var_ns_1(0_INT),
    var_i_1(0_INT),
    var_ns_2(0_INT),
    var_i_2(0_INT),
    var_URL(""_STRING),
    var_QO(0_BOOL),
    var_Movement(""_STRING),
    var_Duration(0_INT),
    var_Enable(0_BOOL),
    var_conn_QO(var_QO),
    var_conn_Movement(var_Movement),
    var_conn_Duration(var_Duration),
    var_conn_Enable(var_Enable),
    conn_CNF(this, 0),
    conn_QI(nullptr),
    conn_ns_1(nullptr),
    conn_i_1(nullptr),
    conn_ns_2(nullptr),
    conn_i_2(nullptr),
    conn_URL(nullptr),
    conn_QO(this, 0, &var_conn_QO),
    conn_Movement(this, 1, &var_conn_Movement),
    conn_Duration(this, 2, &var_conn_Duration),
    conn_Enable(this, 3, &var_conn_Enable) {
}

void FORTE_OPC_Client::setInitialValues() {
  var_QI = 0_BOOL;
  var_ns_1 = 0_INT;
  var_i_1 = 0_INT;
  var_ns_2 = 0_INT;
  var_i_2 = 0_INT;
  var_URL = ""_STRING;
  var_QO = 0_BOOL;
  var_Movement = ""_STRING;
  var_Duration = 0_INT;
  var_Enable = 0_BOOL;
}

void FORTE_OPC_Client::executeEvent(const TEventID paEIID, CEventChainExecutionThread *const paECET) {
  switch(paEIID) {
    case scmEventREQID:
      enterStateREQ(paECET);
      break;
    default:
      break;
  }
}

void FORTE_OPC_Client::enterStateREQ(CEventChainExecutionThread *const paECET) {
  alg_REQ();
  sendOutputEvent(scmEventCNFID, paECET);
}

void FORTE_OPC_Client::readInputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventREQID: {
      readData(1, var_ns_1, conn_ns_1);
      readData(2, var_i_1, conn_i_1);
      readData(3, var_ns_2, conn_ns_2);
      readData(4, var_i_2, conn_i_2);
      readData(0, var_QI, conn_QI);
      readData(5, var_URL, conn_URL);
      break;
    }
    default:
      break;
  }
}

void FORTE_OPC_Client::writeOutputData(const TEventID paEIID) {
  switch(paEIID) {
    case scmEventCNFID: {
      writeData(1, var_Movement, conn_Movement);
      writeData(2, var_Duration, conn_Duration);
      writeData(0, var_QO, conn_QO);
      writeData(3, var_Enable, conn_Enable);
      break;
    }
    default:
      break;
  }
}

CIEC_ANY *FORTE_OPC_Client::getDI(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QI;
    case 1: return &var_ns_1;
    case 2: return &var_i_1;
    case 3: return &var_ns_2;
    case 4: return &var_i_2;
    case 5: return &var_URL;
  }
  return nullptr;
}

CIEC_ANY *FORTE_OPC_Client::getDO(const size_t paIndex) {
  switch(paIndex) {
    case 0: return &var_QO;
    case 1: return &var_Movement;
    case 2: return &var_Duration;
    case 3: return &var_Enable;
  }
  return nullptr;
}

CEventConnection *FORTE_OPC_Client::getEOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_CNF;
  }
  return nullptr;
}

CDataConnection **FORTE_OPC_Client::getDIConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QI;
    case 1: return &conn_ns_1;
    case 2: return &conn_i_1;
    case 3: return &conn_ns_2;
    case 4: return &conn_i_2;
    case 5: return &conn_URL;
  }
  return nullptr;
}

CDataConnection *FORTE_OPC_Client::getDOConUnchecked(const TPortId paIndex) {
  switch(paIndex) {
    case 0: return &conn_QO;
    case 1: return &conn_Movement;
    case 2: return &conn_Duration;
    case 3: return &conn_Enable;
  }
  return nullptr;
}

CIEC_ANY *FORTE_OPC_Client::getVarInternal(size_t) {
  return nullptr;
}


using namespace std;

void FORTE_OPC_Client::alg_REQ(void) {

  if(!var_QI){
    var_QO = CIEC_BOOL(false);
    return;
  }

  int ns_1 = var_ns_1.getSignedValue();
  int i_1 = var_i_1.getSignedValue();
  int ns_2 = var_ns_2.getSignedValue();
  int i_2 = var_i_2.getSignedValue();
  UA_Client *client = UA_Client_new();
  UA_ClientConfig_setDefault(UA_Client_getConfig(client));
  std::string url = var_URL.getStorage();
  std::cout << "Пробуем подключиться к серверу..." << std::endl;
  UA_Client_connect(client, url.c_str());
  std::cout << "Успешно подключились!\n" << std::endl;
  UA_NodeId boolVarId = UA_NODEID_NUMERIC(ns_1, i_1);
  UA_NodeId stringVarId = UA_NODEID_NUMERIC(ns_2, i_2);
  UA_Variant boolValue;
  UA_Variant stringValue;
    UA_Variant_init(&boolValue);
    UA_Variant_init(&stringValue);
    
  
  UA_Client_readValueAttribute(client, boolVarId, &boolValue);
    
  
  UA_Client_readValueAttribute(client, stringVarId, &stringValue);

  
  
  
    
  
  
  bool boolValueConverted = *(UA_Boolean*)boolValue.data;
    
  
  
  UA_String* stringData = (UA_String*)stringValue.data;
    
  std::string stringValueConverted = 
      std::string((char*)stringData->data, stringData->length);


  std::cout << "=== ТЕКУЩИЕ ЗНАЧЕНИЯ ===" << std::endl;
  std::cout << "Булева переменная (ns=2;i=2): " 
            << (boolValueConverted ? "true" : "false") << std::endl;
  std::cout << "Строковая переменная (ns=2;i=3): '" 
            << stringValueConverted << "'\n" << std::endl;


    

  bool newBoolValue = false;
    

  UA_Variant newValueContainer;
  UA_Variant_init(&newValueContainer);
    

  UA_Variant_setScalar(&newValueContainer, &newBoolValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
    

  UA_Client_writeValueAttribute(client, boolVarId, &newValueContainer);
  std::cout << "Изменили булево значение на: " 
            << (newBoolValue ? "true" : "false") << std::endl;


    

  UA_Variant_clear(&boolValue);
  UA_Variant_clear(&stringValue);
    

  std::cout << "\nОтключаемся от сервера..." << std::endl;
  UA_Client_disconnect(client);
    

  UA_Client_delete(client);
  var_Enable = CIEC_BOOL(boolValueConverted);
  stringstream stream(stringValueConverted);
  string Move;
  int duration;
  stream >> Move >> duration;
  var_Movement = CIEC_STRING(Move.c_str());
  var_Duration = CIEC_INT(duration);
  var_QO = CIEC_BOOL(true);
  return;
}
