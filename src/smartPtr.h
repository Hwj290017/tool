
#include <cstddef>
#include <mutex>
namespace hwj
{
template <typename T> class SmartPtr
{
  public:
    SmartPtr(T* ptr) : ptr(ptr), count(new size_t(1)), mtx(new std::mutex())
    {
    }
    SmartPtr(const SmartPtr& other) : ptr(other.ptr), count(other.count), mtx(other.mtx)
    {
        std::lock_guard<std::mutex> lock(*mtx);
        ++*count;
    }
    ~SmartPtr()
    {
        mtx->lock();
        --*count;
        mtx->unlock();
        if (!*count)
        {
            delete ptr;
            delete count;
            delete mtx;
        }
    }
    void operator=(SmartPtr& other)
    {
        mtx->lock();
        --*count;
        mtx->unlock();
        if (!*count)
        {
            delete ptr;
            delete count;
            delete mtx;
        }
        ptr = other.ptr;
        count = other.count;
        mtx = other.mtx;
        mtx->lock();
        ++*count;
        mtx->unlock();
    }
    T& operator*()
    {
        return *ptr;
    }
    T* operator->()
    {
        return ptr;
    }

  private:
    T* ptr;
    size_t* count;
    std::mutex* mtx;
};
} // namespace hwj