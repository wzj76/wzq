//
//  Model.cpp
//  GouJi
//
//  Created by ZengQi on 14-11-24.
//
//

#include "WZQModel.h"

using namespace std;

WZQModel* WZQModel::create()
{
    WZQModel* pRet = new WZQModel("WZQModel");
    if (pRet)
    {
        pRet->init();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    pRet = nullptr;
    return nullptr;
}

WZQModel::WZQModel(const string &sName)
:
m_sName(sName)
{
}

WZQModel::~WZQModel()
{
}
