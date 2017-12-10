#include "processor.h"

#include <math.h>
#include <QDebug>

Processor::Row Processor::cramer(const Matrix& matrix, const Row& loads) const
{
    double delta = det(matrix);

    if (delta == 0)
    {
        qWarning() << "Определитель матрицы = 0";
        return Row();
    }

    Row answer(loads.size());
    for (int i = 0; i < answer.size(); i++)
        answer[i] = det(changeColumn(matrix, loads, i)) / delta;

    return answer;
}

Processor::Matrix Processor::changeColumn(const Matrix &matrix, const Row &column, int index) const
{
    Matrix m = matrix;

    for (int i = 0; i < matrix.size(); i++)
        m[i][index] = column[i];

    return m;
}

double Processor::det(const Processor::Matrix &matrix) const
{
    if(matrix.size() == 2)
        return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];

    double result = 0.0;
    for(int i = 0; i < matrix.size(); i++)
    {
        if (matrix[0][i] != 0)
            result += pow(-1, i + 2) * matrix[0][i] * det(cut(matrix, i));
    }

    return result;
}

Processor::Matrix Processor::cut(const Matrix& matrix, int column) const
{
    Matrix cutM = matrix;

    cutM.remove(0);
    for(int i = 0; i < cutM.size(); ++i)
        cutM[i].remove(column);

    return cutM;
}
