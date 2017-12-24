#include "processor.h"

#include "math.h"

#include <QDebug>

Processor::Processor(QObject *parent)
    : QObject(parent)
{}

void Processor::setSupports(bool left, bool rigth)
{
    hasLeft = left;
    hasRigth = rigth;
}

void Processor::prepare()
{
    qDebug() << endl << "--------- Prepare";

    uBegin.clear();
    uEnd.clear();

    Matrix matrix = makeMatrix();
    fixLoads();

    if (hasLeft)
    {
        loads[0] = 0;
        matrix[0] = Row(loads.size(), 0.0);
        matrix[0][0] = 1;
    }
    if (hasRigth)
    {
        int size = loads.size();
        loads[size - 1] = 0;
        matrix[size - 1] = Row(size, 0.0);
        matrix[size - 1][size - 1] = 1;
    }

    Row deltas = cramer(matrix, loads);
    qDebug() << "deltas" << deltas;

    for (int i = 0; i < deltas.size() - 1; i++)
    {
        uBegin.append(deltas[i]);
        uEnd.append(deltas[i + 1]);
    }

    qDebug() << "Ubegin" << uBegin << endl << "Uend" << uEnd;

//    qDebug() << "cramer: " << deltas;
//    Row nx(deltas.size() - 1);

//    for (int i = 0; i < nx.size(); i++)
//    {
//        const Core& core = cores[i];
//        double val = value(core);
//        nx[i] = -val*deltas[i] + val*deltas[i + 1] - core.load*core.length/2;
//    }

//    Row sigma(nx.size());
//    for (int i = 0; i < sigma.size(); i++)
//    {
//        sigma[i] = nx[i] / cores[i].area;
//    }

    //    qDebug() << deltas << nx << sigma;
}

QVector<QVector<Stress>> Processor::calculate(int partCount)
{
    QVector<QVector<Stress>> res(cores.size(), QVector<Stress>(partCount + 1));

    for (int i = 0; i < cores.size(); i++)
    {
        Core core = cores[i];

        double offset = core.length / partCount;
        double pos = 0;
        for (int j = 0; j <= partCount; j++)
        {
            res[i][j] = calcInPoint(i, pos);
            res[i][j].pos = pos;

            pos += offset;
        }
    }

    return res;
}

Stress Processor::calcInPoint(int coreIndex, double pos) const
{
    double n = nx(coreIndex, pos);
    double u = ux(coreIndex, pos);
    double s = sx(coreIndex, n);

    return {n, u, s};
}

double Processor::nx(int i, double pos) const
{
    Core core = cores[i];

    return value(core) * (uEnd[i] - uBegin[i]) + core.load*core.length/2 * (1 - 2*pos/core.length);
}

double Processor::sx(int i, double nx) const
{
    return nx / cores[i].area;
}

double Processor::ux(int i, double pos) const
{
    Core core = cores[i];

    double k = core.load*pow(core.length, 2) / 2*core.elastic*core.area;
    double m = pos/core.length;

    return uBegin[i] +  m*(uEnd[i] - uBegin[i]) + k*m * (1 - m);
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
        double val = value(cores[index]);

        row[0] = val;
        row[1] = -val;
    }
    else if (index == loads.size() - 1)
    {
        double val = value(cores[index - 1]);

        row[size - 1] = val;
        row[size - 2] = -val;
    }
    else
    {
        double valLeft = value(cores[index - 1]);
        double valRight = value(cores[index]);

        row[index - 1] = -valLeft;
        row[index]     = valLeft + valRight;
        row[index + 1] = -valRight;
    }

    return row;
}

void Processor::fixLoads()
{
    for (int i = 0; i < loads.size(); ++i)
    {
        if (i == 0)
        {
            Core core = cores[i];
            loads[i] = core.load*core.length/2  + loads[i];
        }
        else if (i == loads.size() - 1)
        {
            Core core = cores[i - 1];
            loads[i] = core.load*core.length/2 + loads[i];
        }
        else
        {
            Core left = cores[i - 1];
            Core right = cores[i];

            loads[i] = left.load*left.length/2 + right.load*right.length/2 + loads[i];
        }
    }

    qDebug() << "loads: " << loads;
}

double Processor::value(const Core &core) const
{
    return core.area * core.elastic / core.length;
}
