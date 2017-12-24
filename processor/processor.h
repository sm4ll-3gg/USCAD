#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "declarations.h"

#include <QObject>
#include <QStandardItemModel>

class Processor : public QObject
{
    Q_OBJECT

    using Row    = QVector<double>;
    using Matrix = QVector<Row   >;

public:
    explicit Processor(QObject *parent = Q_NULLPTR);

    void    setCores(QVector<Core>&& c)  { cores = c; }
    void    setLoads(QVector<double>&& l) { loads = l; }
    void    setSupports(bool left, bool rigth);
    void    setPointCount(int count) { pointCount = count; }

    void            prepare();
    QVector<QVector<Stress>>    calculate(int partCount);

    Stress  calcInPoint(int coreIndex, double pos) const;

private: // Methods
    Matrix  makeMatrix() const;
    Row     makeRow(int index) const;

    void    fixLoads();

    double  value(const Core& core) const;

    double  nx(int coreIndex, double pos) const;
    double  sx(int coreIndex, double nx)  const;
    double  ux(int coreIndex, double pos) const;

    // gauss.cpp
    Row     cramer(const Matrix& matrix, const Row& loads) const;

    Matrix  changeColumn(const Matrix& matrix, const Row& column, int index) const;

    double  det(const Matrix& matrix) const;
    Matrix  cut(const Matrix &matrix, int column) const;

private:
    QVector<Core>   cores;
    QVector<double> loads;

    QVector<double> uBegin;
    QVector<double> uEnd;

    // Supports
    bool    hasLeft;
    bool    hasRigth;

    int     pointCount{0};
};

#endif // PROCESSOR_H
