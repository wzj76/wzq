//
//  Model.h
//  GouJi
//
//  Created by ZengQi on 14-11-24.
//
//

#ifndef __GouJi__Model__
#define __GouJi__Model__

#include "Event/WZQEventWrapper.h"
#include <string>

#define CREATE_MODEL(__TYPE__) \
static __TYPE__* create() \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet) \
{ \
pRet->init(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

class WZQModel : public WZQEventWrapper
{
public:
    static WZQModel* create();
    WZQModel(const std::string &sName);
    virtual ~WZQModel();
    
public:
    virtual void init() {}
    virtual void destroy() {}
    virtual void reset() {}
    
    std::string getName() { return m_sName; }
    
protected:
    std::string m_sName;
    
};

#endif /* defined(__GouJi__Model__) */
