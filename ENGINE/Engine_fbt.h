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

class FORTE_Engine final : public CSimpleFB {
  DECLARE_FIRMWARE_FB(FORTE_Engine)

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
    FORTE_Engine(CStringDictionary::TStringId paInstanceNameId, forte::core::CFBContainer &paContainer);

    CIEC_BOOL var_QI;
    CIEC_INT var_Duration;
    CIEC_STRING var_Movement;

    CIEC_BOOL var_QO;

    CIEC_BOOL var_conn_QO;

    CEventConnection conn_CNF;

    CDataConnection *conn_QI;
    CDataConnection *conn_Duration;
    CDataConnection *conn_Movement;

    CDataConnection conn_QO;

    CIEC_ANY *getDI(size_t) override;
    CIEC_ANY *getDO(size_t) override;
    CEventConnection *getEOConUnchecked(TPortId) override;
    CDataConnection **getDIConUnchecked(TPortId) override;
    CDataConnection *getDOConUnchecked(TPortId) override;

    void evt_REQ(const CIEC_BOOL &paQI, const CIEC_INT &paDuration, const CIEC_STRING &paMovement, CIEC_BOOL &paQO) {
      var_QI = paQI;
      var_Duration = paDuration;
      var_Movement = paMovement;
      executeEvent(scmEventREQID, nullptr);
      paQO = var_QO;
    }

    void operator()(const CIEC_BOOL &paQI, const CIEC_INT &paDuration, const CIEC_STRING &paMovement, CIEC_BOOL &paQO) {
      evt_REQ(paQI, paDuration, paMovement, paQO);
    }
};

