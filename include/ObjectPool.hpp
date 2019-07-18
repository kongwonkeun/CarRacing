#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include <memory>
#include "Objects/Car.hpp"
#include "Game.hpp"
#include "Objects/Bullet.hpp"

namespace cp
{
    template<class T> 
    class ObjectPool
    {
    public:
        ObjectPool(size_t size) : max_pool_size(size) {}
        ~ObjectPool() {
            obj_list.clear();
        }

        T* getObject(GameDataRef _data, int car_num) {
            if (!obj_list.empty()) {
                T* obj = obj_list.front();
                obj_list.pop_front();
                return obj;
            }
            else if (obj_made < max_pool_size) {
                obj_made++;
                return new T(_data, car_num);
            } 
            else
                return NULL;
        }

        void returnObject(T* obj) {
            obj->reset();
            obj_list.push_back(obj);
        }

        const size_t max_pool_size;
        size_t obj_made = 0;

    private:
        std::list<T*> obj_list;
    };
}

#endif // OBJECTPOOL_HPP
