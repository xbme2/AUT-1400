#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template <typename T>
class UniquePtr
{

public:
    UniquePtr(T *p = nullptr) { this->_p = p; }
    UniquePtr(UniquePtr &ptr) = delete;
    ~UniquePtr() { 
        delete _p;
        _p=nullptr; 
    }
    bool operator= (UniquePtr &other) =delete;
    T& operator* () {  return *(this->_p);}//it must to be reference , otherwise it will happen copy constructor which is forbidden
    T* operator-> () {  return this->_p ; }
    operator bool() { return this->_p!=nullptr; }
    T * get() const{
        return this->_p;
    }
    T * release() {
        T * old=_p;
        _p=nullptr;
        return old;
    }
    void reset(T *newpointer = nullptr){
        delete _p;
        _p=nullptr; 
        if(newpointer){
            _p=newpointer;
        }
    }
private:
    T *_p;
};




template <typename T>
UniquePtr<T> make_unique(T var){
    return UniquePtr<T>{new T(var)};
}
#endif // UNIQUE_PTR