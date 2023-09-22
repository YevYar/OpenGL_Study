#include "mathCore/vector.h"

#include "mathCore/matrix.h"

namespace ogls::mathCore
{
/*constexpr*/ Vector crossProduct(const Vector& v1, const Vector& v2)
{
    auto m = Matrix{3, 3, 1.0f};

    insertVectorIntoMatrix(m, v1, VectorIntoMatrixInsertionOrder::RowMajor, 1);
    insertVectorIntoMatrix(m, v2, VectorIntoMatrixInsertionOrder::RowMajor, 2);

    return Vector{m.calculateAlgebraicComplement(Matrix::Index{.rows = 0, .columns = 0}),
                  m.calculateAlgebraicComplement(Matrix::Index{.rows = 0, .columns = 1}),
                  m.calculateAlgebraicComplement(Matrix::Index{.rows = 0, .columns = 2})};
}

void insertVectorIntoMatrix(Matrix& m, const Vector& v, VectorIntoMatrixInsertionOrder order, size_t index,
                            float placeholder)
{
    if (m.isNullMatrix())
    {
        return;
    }

    const auto inserter = [&](size_t k, float value)
    {
        if (order == VectorIntoMatrixInsertionOrder::RowMajor)
        {
            m.setValue(index, k, value);
        }
        else
        {
            m.setValue(k, index, value);
        };
    };

    const auto maxIndex = order == VectorIntoMatrixInsertionOrder::RowMajor ? m.getDimensionality().columns
                                                                            : m.getDimensionality().rows;
    for (auto i = size_t{0}; i < maxIndex; ++i)
    {
        switch (i)
        {
            case 0:
                inserter(0, v.x());
                break;
            case 1:
                inserter(1, v.y());
                break;
            case 2:
                inserter(2, v.z());
                break;
            default:
                inserter(i, placeholder);
        }
    }
}

}  // namespace ogls::mathCore
