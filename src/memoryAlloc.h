// 导入内存块头文件
#include "MemoryBlock.h"
#include <mutex>
namespace hwj
{

class MemoryAlloc
{
  public:
    MemoryAlloc();
    virtual ~MemoryAlloc();
    // 设置初始化
    void setInit(size_t nSize, size_t nBlockSize); // 传入的为内存块可用内存大小和内存块数量
    // 初始化
    void initMemory();
    // 申请内存
    void* allocMem(size_t nSize); // 传入的为申请可用内存的大小
    // 释放内存
    void freeMem(void* p);

  protected:
    // 内存池地址
    char* _pBuf;
    // 头部内存单元
    MemoryBlock* _pHeader;
    // 内存块大小
    size_t _nSize;
    // 内存块数量
    size_t _nBlockSize;
    // 多线程锁
    std::mutex _mutex; // 锁上申请内存方法和释放内存方法即可实现多线程操作
};
} // namespace hwj