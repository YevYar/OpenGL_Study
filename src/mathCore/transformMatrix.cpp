#include "transformMatrix.h"

#include <algorithm>

namespace ogls::mathCore
{
#ifdef TREAT_VECTORS_AS_COLUMNS
#    define OGLS_BEGIN_IT  begin
#    define OGLS_CBEGIN_IT cbegin
#    define OGLS_CEND_IT   cend
#    define OGLS_END_IT    end
#elif TREATE_VECTORS_AS_ROWS
#    define OGLS_BEGIN_IT  beginColumn
#    define OGLS_CBEGIN_IT cbeginColumn
#    define OGLS_CEND_IT   cendColumn
#    define OGLS_END_IT    endColumn
#else
#    error "Please select a graphics API (USE_OPENGL or USE_VULKAN)"
#endif

// clang-format off
TransformMatrix::TransformMatrix(float defaultDiagonalValue, float w) noexcept :
    m_matrix{{defaultDiagonalValue, 0.0f, 0.0f, 0.0f,
              0.0f, defaultDiagonalValue, 0.0f, 0.0f,
              0.0f, 0.0f, defaultDiagonalValue, 0.0f,
              0.0f, 0.0f, 0.0f, w}},
    m_lastResultMatrix{{defaultDiagonalValue, 0.0f, 0.0f, 0.0f,
                        0.0f, defaultDiagonalValue, 0.0f, 0.0f,
                        0.0f, 0.0f, defaultDiagonalValue, 0.0f,
                        0.0f, 0.0f, 0.0f, w}}
// clang-format on
{
}

// clang-format off
TransformMatrix::TransformMatrix(float xCoef, float yCoef, float zCoef, float w) noexcept :
    m_matrix{{xCoef, 0.0f, 0.0f, 0.0f,
              0.0f, yCoef, 0.0f, 0.0f,
              0.0f, 0.0f, zCoef, 0.0f,
              0.0f, 0.0f, 0.0f, w}},
    m_lastResultMatrix{{xCoef, 0.0f, 0.0f, 0.0f,
                        0.0f, yCoef, 0.0f, 0.0f,
                        0.0f, 0.0f, zCoef, 0.0f,
                        0.0f, 0.0f, 0.0f, w}}
// clang-format on
{
}

TransformMatrix::TransformMatrix(const Vec3& xBasis, const Vec3& yBasis, const Vec3& zBasis, float w) noexcept
{
    auto i = m_matrix.OGLS_BEGIN_IT();

    const auto fillMatrixFromVec = [&](const Vec3& v)
    {
        for (const auto& el : v)
        {
            (*i++).setValue(el.getValue());
        }
        (*i++).setValue(0.0f);
    };

    fillMatrixFromVec(xBasis);
    fillMatrixFromVec(yBasis);
    fillMatrixFromVec(zBasis);

    for (auto j = 0; j < 3; ++j)
    {
        (*i++).setValue(0.0f);
    }
    (*i++).setValue(w);

    m_lastResultMatrix = m_matrix;
}

TransformMatrix::TransformMatrix(const Mat4& m) noexcept : m_matrix{m}, m_lastResultMatrix{m}
{
}

TransformMatrix::TransformMatrix(const TransformMatrix& other) :
    m_lastOperationQueueSize{other.m_lastOperationQueueSize}, m_lastResultMatrix{other.m_lastResultMatrix},
    m_matrix{other.m_matrix}, m_operationQueue{other.m_operationQueue.size()}
{
    for (const auto& operation : other.m_operationQueue)
    {
        m_operationQueue.push_back(operation->clone());
    }
}

Vec4 TransformMatrix::applyTransformationToVector(const Vec4& v, const TransformMatrix& transformation) noexcept
{
    if constexpr (OGLS_VECTOR_IS_COLUMN)
    {
        return (transformation.getResultMatrix() * v).toVector();
    }
    else
    {
        return (v * transformation.getResultMatrix()).toVector();
    }
}

TransformMatrix TransformMatrix::createCombinedTransformMatrix(const TransformMatrix& model,
                                                               const TransformMatrix& view,
                                                               const TransformMatrix& projection) noexcept
{
    if constexpr (OGLS_VECTOR_IS_COLUMN)
    {
        return projection * view * model;
    }
    else
    {
        return model * view * projection;
    }
}

TransformMatrix TransformMatrix::createOrthographicProjection() noexcept
{
    return TransformMatrix{};
}

TransformMatrix TransformMatrix::createPerspectiveProjection(float fovy, float aspect, float zNear, float zFar) noexcept
{
    // Implemented only for OpenGL
    const auto tanHalfFovy = std::tan(fovy / 2.0f);
    auto       result      = Mat4{};
    result[0][0]           = 1.0f / (aspect * tanHalfFovy);
    result[1][1]           = 1.0f / tanHalfFovy;
    result[2][2]           = -(zFar + zNear) / (zFar - zNear);
    result[2][3]           = -1.0f;
    result[3][2]           = -(2.0f * zFar * zNear) / (zFar - zNear);
    return TransformMatrix{result};
}

TransformMatrix& TransformMatrix::operator=(const TransformMatrix& other)
{
    if (this != &other)
    {
        m_lastOperationQueueSize = other.m_lastOperationQueueSize;
        m_lastResultMatrix       = other.m_lastResultMatrix;
        m_matrix                 = other.m_matrix;

        m_operationQueue.clear();
        m_operationQueue.reserve(other.m_operationQueue.size());

        for (const auto& operation : other.m_operationQueue)
        {
            m_operationQueue.push_back(operation->clone());
        }
    }

    return *this;
}

TransformMatrix::operator Mat4() const noexcept
{
    return getResultMatrix();
}

void TransformMatrix::addRotation(float angle, const Vec3& axis)
{
    m_operationQueue.push_back(std::make_unique<Rotation>(angle, axis));
}

void TransformMatrix::addScale(const Vec3& scaling)
{
    m_operationQueue.push_back(std::make_unique<Scale>(scaling));
}

void TransformMatrix::addTranslation(const Vec3& direction)
{
    m_operationQueue.push_back(std::make_unique<Translation>(direction));
}

Mat4 TransformMatrix::getInitialMatrix() const noexcept
{
    return m_matrix;
}

Mat4 TransformMatrix::getResultMatrix() const noexcept
{
    if (m_operationQueue.size() == 0 || m_lastOperationQueueSize == m_operationQueue.size())
    {
        return m_lastResultMatrix;
    }

    m_lastResultMatrix = m_matrix;

    if constexpr (OGLS_VECTOR_IS_COLUMN)
    {
        std::for_each(m_operationQueue.crbegin(), m_operationQueue.crend(),
                      [&](const auto& operation)
                      {
                          operation->execute(m_lastResultMatrix);
                      });
    }
    else
    {
        for (const auto& operation : m_operationQueue)
        {
            operation->execute(m_lastResultMatrix);
        }
    }

    m_lastOperationQueueSize = m_operationQueue.size();
    return m_lastResultMatrix;
}

std::string TransformMatrix::toString(int columnWidth) const
{
    auto ss = std::stringstream{};

    ss << "TransformMatrix:\n";

    getResultMatrix();

    auto el = m_lastResultMatrix.begin();
    ss << "  | " << std::left << std::setw(columnWidth) << (*el).getValue() << ", ";

    for (++el; el != m_lastResultMatrix.end(); ++el)
    {
        if ((*el).j == 0)
        {
            ss << " |\n  | ";
        }

        ss << std::left << std::setw(columnWidth) << (*el).getValue();

        if ((*el).j != 3)
        {
            ss << ", ";
        }
    }

    ss << " |";

    return ss.str();
}

std::ostream& operator<<(std::ostream& out, const TransformMatrix& tm)
{
    out << tm.toString();
    return out;
}

bool operator==(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept
{
    return tm1.getResultMatrix() == tm2.getResultMatrix();
}

bool operator!=(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept
{
    return !(tm1 == tm2);
}

TransformMatrix operator*(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept
{
    return TransformMatrix{tm1.getResultMatrix() * tm2.getResultMatrix()};
}

#undef OGLS_BEGIN_IT
#undef OGLS_CBEGIN_IT
#undef OGLS_CEND_IT
#undef OGLS_END_IT

}  // namespace ogls::mathCore
