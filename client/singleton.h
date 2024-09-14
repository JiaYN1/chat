#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>
#include <iostream>
#include <mutex>

/*
 * 1. 不允许外部生成、释放
 * 2. 静态成员变量和静态返回单例的函数
 * 3. 禁用拷贝构造和赋值
 */

template <typename T>
class Singleton {
protected:
    Singleton() = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            _instance = std::shared_ptr<T>(new T);  // 不用make_shared继承模版单例类的子类的构造会设置为private，make_shared无法访问私有的构造函数
        });
        return _instance;
    }

    // static T& GetInstance() {
    //     static T instance;
    //     return instance;
    // }

    void PrintAddress() {
        std::cout << _instance.get() << std::endl;
    }

    ~Singleton() {
        std::cout << "This is singleton destruct." << std::endl;
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;

#endif // SINGLETON_H
