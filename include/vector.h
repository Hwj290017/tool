
#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
namespace hwj
{
template <typename T> class Vector
{
  private:
    T* start;
    T* end;
    T* limit;

  public:
    Vector(size_t const size = 0) : start(new T[size]), end(start + size), limit(end)
    {
    }
    Vector(Vector const& other)
        : start(new T[other.end - other.start]), end(start + (other.end - other.start)), limit(end)
    {
        size_t size = other.end - other.start;
        for (size_t i = 0; i < size; i++)
            start[i] = other.start[i];
    }
    Vector(Vector&& other) : start(other.start), end(other.end), limit(other.limit)
    {
        other.start = nullptr;
    }
    ~Vector()
    {
        delete[] start;
    }

    // 拷贝赋值
    Vector& operator=(Vector const& other)
    {
        size_t size = other.start - other.end;
        start = new T[size];
        end = start + size;
        limit = end;
        for (size_t i = 0; i < size; i++)
            start[i] = other.start[i];
        return *this;
    }
    // 移动赋值
    Vector& operator=(Vector&& other)
    {
        start = other.start;
        end = other.end;
        limit = other.limit;
        other.start = nullptr;
        return *this;
    }
    // 判断相等
    bool operator==(Vector const& other) const
    {
        return compare(*this, other) == 0 ? true : false;
    }
    // []
    T& operator[](size_t index)
    {
        if (start + index >= end)
            throw "index out of range";
        return *(start + index);
    }
    //[]const
    T const& operator[](size_t index) const
    {
        if (start + index >= end)
            throw "index out of range";
        return *(start + index);
    }
    // 获取数据大小
    size_t size() const
    {
        return end - start;
    }
    // 确保总空间容量
    void reserve(size_t num)
    {
        while (num > limit)
            expandSpace();
    }
    // 改变Vector大小
    void resize(size_t newSize)
    {
        size_t size = limit - start;
        if (newSize > size)
        {
            T* newStart = new T[newSize];

            for (int i = 0; i < size; i++)
                newStart[i] = start[i];
            delete[] start;
            start = newStart;
            limit = newStart + newSize;
        }
        end = start + newSize;
    }
    // 尾部插入元素
    void append(T const& item)
    {
        if (end >= limit)
            expandSpace();
        *end = item;
        ++end;
    }
    // 连接另一个vector
    void extend(Vector const& other)
    {
        size_t otherSize = other.end - other.start;
        size_t size = end - start;
        if (limit - end < otherSize)
        {
            T* newStart = new T[size + otherSize];
            for (size_t i = 0; i < size; ++i)
                newStart[i] = start[i];
            delete[] start;
            start = newStart;
            end = newStart + size;
            limit = newStart + size + otherSize;
        }
        for (size_t i = 0; i < otherSize; i++)
            end[i] = other.start[i];
    }
    // 比较字节序
    static int compare(Vector const& first, Vector const& second)
    {
        T* index1 = first.start;
        T* index2 = second.start;
        while (index1 < first.end && index2 < second.end)
        {
            if (*index1 < *index2)
                return -1;
            if (*index1 > *index2)
                return 1;
            ++index1;
            ++index2;
        }
        if (index1 >= first.end)
        {
            if (index2 >= second.end)
                return 0;
            return -1;
        }
        return 1;
    }

    // 空间翻倍
  private:
    void expandSpace()
    {
        size_t limitSize = limit - start;
        size_t size = end - start;
        size_t newSize = limitSize <= 0 ? 2 : 2 * limitSize;
        T* newStart = new T[newSize];
        for (size_t i = 0; i < size; i++)
            newStart[i] = start[i];
        delete[] start;
        start = newStart;
        end = newStart + size;
        limit = newStart + newSize;
    }
};
} // namespace hwj
#endif