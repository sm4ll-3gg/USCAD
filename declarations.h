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

class LoadsModel : public QStandardItemModel
{
public:
    explicit LoadsModel(int row, int columns, QObject* parent = Q_NULLPTR)
        :QStandardItemModel(row, columns, parent)
    {}

    Qt::ItemFlags   flags(const QModelIndex &index) const Q_DECL_OVERRIDE
    {
        Qt::ItemFlags flags = QStandardItemModel::flags(index);

        if(index.column() == 0)
            flags.setFlag(Qt::ItemIsEditable, false);

        return flags;
    }
};

#endif // DECLARATIONS_H
