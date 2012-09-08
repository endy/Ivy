
#ifndef _CDAPP_H_
#define _CDAPP_H_

#include "DxApp.h"

class CDApp : public DxApp
{
public:
    static CDApp* Create();

    virtual void Destroy();

    virtual void Run();

protected:
	CDApp();
	virtual ~CDApp();

	bool Init();

private:
	CDApp(const CDApp& copy);	// disallow copy constructor
};

#endif // _CDAPP_H_