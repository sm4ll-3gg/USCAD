#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include <QStandardItemModel>

struct Core
{
    Core() {}

    Core(double length, double area, double elastic, double strength, double load)
        :length(length), area(area), elastic(elastic), strength(strength), load(load)
    {}

    double  length{0};
    double  area{0};
    double  elastic{0};
    double  strength{0};
    double  load{0};
};

#endif // DECLARATIONS_H
