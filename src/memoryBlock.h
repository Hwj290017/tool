
namespace hwj
{

// 预声明内存池类
class MemoryAlloc;
// 内存块类
class MemoryBlock
{
  public:
    // 内存块编号
    int id;
    // 引用情况
    int _ref;
    // 所属内存池
    MemoryAlloc* mAlloc;
    // 下一块位置
    MemoryBlock* Next;
    // 是否在内存池内
    bool inPool;

  private:
};

} // namespace hwj