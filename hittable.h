#ifndef HITTABLE_H
#define HITTABLE_H

#include <Box2D/Box2D.h>
#include "weapon.h"
#include "memory"

class Hittable
{
public:
    template <class T>
    explicit Hittable(T *t) :
        hittable{std::make_unique<Hittable_derived<T>>(t)}
    {
    }
    void take_hit(Weapon &weapon);
private:
    struct Hittable_base{
        virtual ~Hittable_base() = default;
        virtual void take_hit(Weapon &weapon) = 0;
    };
    template <class T>
    struct Hittable_derived : Hittable_base{
        Hittable_derived(T *t) : t(t){}
        void take_hit(Weapon &weapon) override{
            t->take_hit(weapon);
        }
        T *t;
    };
    std::unique_ptr<Hittable_base> hittable;
};

#endif // HITTABLE_H
