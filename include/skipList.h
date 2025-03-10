
#ifndef _SKIPLIST_H_
#define _SKIPLIST_H_
#include "random.h"
#include <mutex>
namespace hwj
{
template <typename Key, typename Value> class SkipList
{
  private:
    class Node;

  public:
    class Iter;
    SkipList(int (*cmp)(const Key&, const Key&));
    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;
    ~SkipList();
    void insert(const Key&, const Value&);
    void erase(const Key&);
    Value get(const Key&);
    Iter begin()
    {
        return head->next[0];
    }
    Iter end()
    {
        return nullptr;
    }
    class Iter
    {
      public:
        Iter(Node* index) : index(index)
        {
        }
        bool operator!=(Iter const& other)
        {
            return index != other.index;
        }
        Iter& operator++()
        {
            index = index->next[0];
            return *this;
        }
        Key& operator*()
        {
            return index->key;
        }

      private:
        Node* index;
    };

  private:
    class Node
    {
      public:
        Key key;
        Value value;
        Node** next;
        Node(const Key& key, const Value& value, const unsigned int level);
        Node(const Node&) = delete;
        ~Node();
        // Node(Node&&);
    };
    void findLess(const Key& key, Node** prev);
    void findLessOrEqual(const Key& key, Node** prev);
    unsigned int getLevel();
    int (*cmp)(const Key& lhs, const Key& rhs);
    Node* head;
    RandomInt random;
    std::mutex mutex;
    static const unsigned int maxLevel = 12;
    static const unsigned int branch = 4;
};
// 初始化跳表
template <typename Key, typename Value>
SkipList<Key, Value>::SkipList(int (*cmp)(const Key&, const Key&))
    : cmp(cmp), head(new Node(0, 0, maxLevel)), random(0, branch - 1)
{
}
// 析构
template <typename Key, typename Value> SkipList<Key, Value>::~SkipList()
{
    Node* p = head;
    Node* q;
    while (p)
    {
        q = p->next[0];
        delete p;
        p = q;
    }
}
// 插入元素
template <typename Key, typename Value> void SkipList<Key, Value>::insert(const Key& key, const Value& value)
{
    std::lock_guard<std::mutex> guard(mutex);
    Node* prev[maxLevel];
    findLessOrEqual(key, prev);
    if (cmp(prev[0]->key, key) >= 0)
    {
        prev[0]->value = value;
        return;
    }
    unsigned int level = getLevel();
    Node* newNode = new Node(key, value, level);
    for (int i = 0; i < level; i++)
    {
        Node* temp = prev[i]->next[i];
        prev[i]->next[i] = newNode;
        newNode->next[i] = temp;
    }
}
// 删除元素
template <typename Key, typename Value> void SkipList<Key, Value>::erase(const Key& key)
{
    std::lock_guard<std::mutex> guard(mutex);
    Node* prev[maxLevel];
    findLess(key, prev);
    Node* temp = prev[0]->next[0];
    if (temp && cmp(temp->key, key) == 0)
    {
        for (int i = 0; i < maxLevel; i++)
        {
            if (prev[i]->next[i] == temp)
            {
                prev[i]->next[i] = temp->next[i];
            }
        }
        delete temp;
    }
}
// 寻找元素
template <typename Key, typename Value> Value SkipList<Key, Value>::get(const Key& key)
{
    std::lock_guard<std::mutex> guard(mutex);
    Node* prev[maxLevel];
    findLessOrEqual(key, prev);
    if (cmp(prev[0]->key, key) >= 0)
        return prev[0]->value;
    return 0;
}
// 寻找刚好小于的元素
template <typename Key, typename Value> void SkipList<Key, Value>::findLess(const Key& key, Node** prev)
{
    Node* now = head;
    unsigned int level = maxLevel - 1;
    while (true)
    {
        Node* next = now->next[level];
        // 找到刚好更小的节点
        if (!next || cmp(next->key, key) >= 0)
        {
            prev[level] = now;
            if (level == 0)
                return;
            level--;
        }
        else
        {
            now = next;
        }
    }
}

// 寻找刚好小于等于的元素
template <typename Key, typename Value> void SkipList<Key, Value>::findLessOrEqual(const Key& key, Node** prev)
{
    findLess(key, prev);
    for (int i = 0; i < maxLevel; i++)
    {
        Node* temp = prev[i]->next[i];
        if (temp && cmp(temp->key, key) == 0)
            prev[i] = temp;
    }
}
// 得到加入节点对应的高度
template <typename Key, typename Value> unsigned int SkipList<Key, Value>::getLevel()
{
    unsigned int level = 1;
    while (level < maxLevel)
    {
        if (random.get() == 0)
            level++;
        else
            break;
    }
    return level;
}

template <typename Key, typename Value>
SkipList<Key, Value>::Node::Node(const Key& key, const Value& value, const unsigned int level)
    : key(key), value(value), next(new Node*[level])
{
    for (int i = 0; i < level; i++)
        next[i] = nullptr;
}

template <typename Key, typename Value> SkipList<Key, Value>::Node::~Node()
{
    delete[] next;
}
} // namespace hwj
#endif
