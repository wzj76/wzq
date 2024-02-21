//
//  Controller.cpp
//  GouJi
//
//  Created by 曾琪 on 14-9-28.
//
//

#include "WZQController.h"

USING_NS_CC;
using namespace std;

WZQController::WZQController(const string &sName)
:
m_sName(sName)
{
}

WZQController::~WZQController()
{
}

void WZQController::execute(const std::string &sName, void *pUserData)
{
    log("%s::execute >>> %s", m_sName.c_str(), sName.c_str());
    onEventData(sName, pUserData);
}

string WZQController::getValue(const ValueMap& obOption, const string& sKey)
{
    string sValue = "";
    if (obOption.find(sKey) != obOption.end())
    {
        sValue = obOption.at(sKey).asString();
    }
    return sValue;
}
