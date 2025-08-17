#include <iostream>

class Singleton {
private:
    Singleton() {
        std::cout << "Singleton created\n";
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void doSomething() {
        std::cout << "Doing something with the singleton\n";
    }
};
