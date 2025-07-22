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

#pragma once

#include "core/simplefb.h"
#include "core/datatypes/forte_bool.h"
#include "core/datatypes/forte_int.h"
#include "core/datatypes/forte_string.h"
#include "core/iec61131_functions.h"
#include "core/datatypes/forte_array_common.h"
#include "core/datatypes/forte_array.h"
#include "core/datatypes/forte_array_fixed.h"
#include "core/datatypes/forte_array_variable.h"

class FORTE_OPC_Client final : public CSimpleFB {
  DECLARE_FIRMWARE_FB(FORTE_OPC_Client)

  private:
    static const CStringDictionary::TStringId scmDataInputNames[];
    static const CStringDictionary::TStringId scmDataInputTypeIds[];
    static const CStringDictionary::TStringId scmDataOutputNames[];
    static const CStringDictionary::TStringId scmDataOutputTypeIds[];
    static const TEventID scmEventREQID = 0;
    static const TDataIOID scmEIWith[];
    static const TForteInt16 scmEIWithIndexes[];
    static const CStringDictionary::TStringId scmEventInputNames[];
    static const TEventID scmEventCNFID = 0;
    static const TDataIOID scmEOWith[];
    static const TForteInt16 scmEOWithIndexes[];
    static const CStringDictionary::TStringId scmEventOutputNames[];

    static const SFBInterfaceSpec scmFBInterfaceSpec;

    CIEC_ANY *getVarInternal(size_t) override;

    void alg_REQ(void);

    void enterStateREQ(CEventChainExecutionThread *const paECET);

    void executeEvent(TEventID paEIID, CEventChainExecutionThread *const paECET) override;

    void readInputData(TEventID paEIID) override;
    void writeOutputData(TEventID paEIID) override;
    void setInitialValues() override;

  public:
    FORTE_OPC_Client(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_INT var_ns_1;
    CIEC_INT var_i_1;
    CIEC_INT var_ns_2;
    CIEC_INT var_i_2;
    CIEC_STRING var_URL;

    CIEC_BOOL var_QO;
    CIEC_STRING var_Movement;
    CIEC_INT var_Duration;
    CIEC_BOOL var_Enable;

    CIEC_BOOL var_conn_QO;
    CIEC_STRING var_conn_Movement;
    CIEC_INT var_conn_Duration;
    CIEC_BOOL var_conn_Enable;

    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_ns_1;
    CDataConnection *conn_i_1;
    CDataConnection *conn_ns_2;
    CDataConnection *conn_i_2;
    CDataConnection *conn_URL;

    CDataConnection conn_QO;
    CDataConnection conn_Movement;
    CDataConnection conn_Duration;
    CDataConnection conn_Enable;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_INT &pans_1, const CIEC_INT &pai_1, const CIEC_INT &pans_2, const CIEC_INT &pai_2, const CIEC_STRING &paURL, CIEC_BOOL &paQO, CIEC_STRING &paMovement, CIEC_INT &paDuration, CIEC_BOOL &paEnable) {
      var_QI = paQI;
      var_ns_1 = pans_1;
      var_i_1 = pai_1;
      var_ns_2 = pans_2;
      var_i_2 = pai_2;
      var_URL = paURL;
      executeEvent(scmEventREQID, nullptr);
      paQO = var_QO;
      paMovement = var_Movement;
      paDuration = var_Duration;
      paEnable = var_Enable;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_INT &pans_1, const CIEC_INT &pai_1, const CIEC_INT &pans_2, const CIEC_INT &pai_2, const CIEC_STRING &paURL, CIEC_BOOL &paQO, CIEC_STRING &paMovement, CIEC_INT &paDuration, CIEC_BOOL &paEnable) {
      evt_REQ(paQI, pans_1, pai_1, pans_2, pai_2, paURL, paQO, paMovement, paDuration, paEnable);
    }
};

