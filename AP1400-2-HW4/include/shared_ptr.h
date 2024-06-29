#ifndef SHARED_PTR
#define SHARED_PTR
template <typename T>

class SharedPtr
{

public:
    int *count;
    SharedPtr(T *ptr = nullptr) : _p(ptr)
    {
        count = new int(1);
    }
    SharedPtr(SharedPtr &other) : _p(other.get()), count(other.count)
    {
        (*count)++;
    }
    ~SharedPtr()
    {
        assert(*count >= 0);
        (*count)--;
        if (*count == 0)
        {
            delete _p;
        }
        _p = nullptr;
    }
    SharedPtr &operator=(SharedPtr &other)
    {
        if (this != &other)
        {
            this->_p = other.get();
            this->count = other.count;
            (*count)++;
        }
        return *this;
    }

    operator bool()
    {
        return _p != nullptr;
    }
    T *operator->() { return this->_p; }
    T &operator*() { return *(this->_p); }
    T *get() const { return this->_p; }
    int use_count() { return *count; }
    void reset(T *newpo = nullptr)
    {
        delete _p;
        _p = nullptr;
        *count = 0;
        if (newpo)
        {
            _p = newpo;
            count = new int(1);
        }
    }

private:
    T *_p;
};

template <typename T>
SharedPtr<T> make_shared(T var)
{
    return SharedPtr(new T(var));
}

#endif // SHARED_PTR