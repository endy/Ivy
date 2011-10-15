
#ifndef _CDAPP_H_
#define _CDAPP_H_

#include "DxApp.h"

struct CDAppCreateInfo : public DxAppCreateInfo
{

};

class CDApp : public DxApp
{
public:
    static CDApp* Create(CDAppCreateInfo* pAppInfo);

    virtual void Destroy();

    virtual void Run();

protected:
	CDApp(CDAppCreateInfo* pAppInfo);
	virtual ~CDApp();

	bool Init();

private:
	CDApp(const CDApp& copy);	// disallow copy constructor
};

#endif // _CDAPP_H_