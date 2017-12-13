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
    explicit Processor(QVector<Core>&& cores, QVector<double>&& loads,
                       bool hasLeft, bool hasRigth, QObject *parent = Q_NULLPTR);

    void    calculate();

private: // Methods
    Matrix  makeMatrix() const;
    Row     makeRow(int index) const;

    void    fixLoads();

    double  value(const Core& core) const;

    // gauss.cpp
    Row     cramer(const Matrix& matrix, const Row& loads) const;

    Matrix  changeColumn(const Matrix& matrix, const Row& column, int index) const;

    double  det(const Matrix& matrix) const;
    Matrix  cut(const Matrix &matrix, int column) const;

private:
    QVector<Core>   cores;
    QVector<double> loads;

    // Supports
    bool    hasLeft;
    bool    hasRigth;
};

#endif // PROCESSOR_H
