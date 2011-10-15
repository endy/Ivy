

#include <iostream>

void IvyPrint (const char* message, const char* file, unsigned int line)
{
    std::cout << message << " Line(" << line << ") " << file << std::endl;
}

class IvyMemoryMgr
{
public:
    static void* Alloc(size_t size)
    {
        std::cout << "Alloc size=" << size << std::endl;
        return calloc(1, size);
    };

    static void Free(void* pMem)
    {
        std::cout << "Free Pointer=" << pMem << std::endl;
        free(pMem);
    }
};

void* IvyTrackedAlloc (size_t n, const char* file, unsigned int line)
{
    IvyPrint("IvyAlloc", file, line);
    return IvyMemoryMgr::Alloc(n);
}

void IvyTrackedFree (void* in, const char* file, unsigned int line)
{
    IvyPrint("IvyFree", file, line);
    IvyMemoryMgr::Free(in);
}

#define TrackMemory 1

#if TrackMemory
#define IvyAlloc(size) IvyTrackedAlloc((size), (__FILE__), (__LINE__))
#define IvyFree(ptr) IvyTrackedFree((ptr), (__FILE__), (__LINE__))

#define IvyNew(datatype) new datatype; IvyPrint("IvyNew", __FILE__, __LINE__)
#define IvyArrayNew(datatype,count) (new datatype[count]); IvyPrint("IvyArrayNew", __FILE__, __LINE__)

#define IvyDelete(ptr) IvyPrint("IvyDelete", __FILE__, __LINE__); delete (ptr)
#define IvyArrayDelete(ptr) IvyPrint("IvyArrayDelete", __FILE__, __LINE__); delete [](ptr)

#else
#define IvyAlloc(x) malloc(x)
#define IvyFree(x) free(x)
#define IvyNew(x) new (x)
#define IvyDelete(x) delete (x)
#endif


class IvyObject
{
public:
    void* operator new(size_t size) 
    {
        return IvyMemoryMgr::Alloc(size);
    }
    void* operator new[](size_t size)
    {
        return IvyMemoryMgr::Alloc(size);
    }
    void operator delete(void* pObj)
    {
        IvyMemoryMgr::Free(pObj);
    }
    void operator delete[](void* pObj)
    {
        IvyMemoryMgr::Free(pObj);
    }

protected:
    IvyObject() { }
    virtual ~IvyObject() { }
};

class SimpleClass : public IvyObject
{
public:
    SimpleClass() : m_id(0) {}
    SimpleClass(int id) : m_id(id) {}
    ~SimpleClass(){}

    void PrintName(){ std::cout << "Simple Class Id = " << m_id << std::endl; }

private:
    int m_id;
};


int main()
{
    int* pInt = static_cast<int*>(IvyAlloc(int(4)));
    IvyFree(pInt);


    SimpleClass* pS1 = new SimpleClass();
    pS1->PrintName();
    delete pS1;

    SimpleClass* pS3 = IvyNew(SimpleClass(3));
    pS3->PrintName();
    IvyDelete(pS3);

    int* pIntArray = IvyArrayNew(int, 40);
    IvyArrayDelete(pIntArray);

    SimpleClass* pSCArray = IvyArrayNew(SimpleClass, 32);
    IvyArrayDelete(pSCArray);

    return 0;
}