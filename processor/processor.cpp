#include "processor.h"

#include <QDebug>

Processor::Processor(QVector<Core> &&construction, QVector<double> &&loads,
                     bool hasLeft, bool hasRigth, QObject *parent)
    : QObject(parent),
      construction(construction), loads(loads), hasLeft(hasLeft), hasRigth(hasRigth)
{}

void Processor::calculate() const
{
    Matrix matrix = makeMatrix();

    if (hasLeft)
    {
        matrix[0] = Row(loads.size(), 0.0);
        matrix[0][0] = 1;
    }
    if (hasRigth)
    {
        int size = loads.size();
        matrix[size - 1] = Row(size, 0.0);
        matrix[size - 1][size - 1] = 1;
    }

    Row deltas = cramer(matrix, loads);
    Row nx(deltas.size() - 1);

    for (int i = 0; i < nx.size(); i++)
    {
        const Core& core = construction[i];
        double val = value(core);
        nx[i] = -val*deltas[i] + val*deltas[i + 1] - core.load*core.length/2;
    }

    Row sigma(nx.size());
    for (int i = 0; i < sigma.size(); i++)
    {
        sigma[i] = nx[i] / construction[i].area;
    }

    qDebug() << deltas << nx << sigma;
}

Processor::Matrix Processor::makeMatrix() const
{
    int size = loads.size();
    Matrix matrix(size);

    for (int i = 0; i < size; ++i)
        matrix[i] = makeRow(i);

    return matrix;
}

Processor::Row Processor::makeRow(int index) const
{
    int size = loads.size();

    Row row(size);

    if (index == 0)
    {
        double val = value(construction[index]);

        row[0] = val;
        row[1] = -val;
    }
    else if (index == loads.size() - 1)
    {
        double val = value(construction[index - 1]);

        row[size - 1] = val;
        row[size - 2] = -val;
    }
    else
    {
        double valLeft = value(construction[index - 1]);
        double valRight = value(construction[index]);

        row[index - 1] = -valLeft;
        row[index]     = valLeft + valRight;
        row[index + 1] = -valRight;
    }

    return row;
}

double Processor::value(const Core &core) const
{
    return core.area * core.elastic / core.length;
}
