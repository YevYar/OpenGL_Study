#ifndef OGLS_MATHCORE_TRANSFORM_MATRIX_H
#define OGLS_MATHCORE_TRANSFORM_MATRIX_H

#include "mathCore/matrix.h"
#include "mathCore/vector.h"

#ifdef TREAT_VECTORS_AS_COLUMNS
#    define OGLS_BEGIN_IT  begin
#    define OGLS_CBEGIN_IT cbegin
#    define OGLS_CEND_IT   cend
#    define OGLS_END_IT    end
constexpr inline auto OGLS_VECTOR_IS_COLUMN = true;
#elif TREATE_VECTORS_AS_ROWS
#    define OGLS_BEGIN_IT  beginColumn
#    define OGLS_CBEGIN_IT cbeginColumn
#    define OGLS_CEND_IT   cendColumn
#    define OGLS_END_IT    endColumn
constexpr inline auto OGLS_VECTOR_IS_COLUMN = false;
#else
#    error "Please select a graphics API (USE_OPENGL or USE_VULKAN)"
#endif

namespace ogls::mathCore
{
class TransformMatrix;

constexpr bool            operator==(const TransformMatrix&, const TransformMatrix&) noexcept;
constexpr bool            operator!=(const TransformMatrix&, const TransformMatrix&) noexcept;
constexpr TransformMatrix operator*(const TransformMatrix&, const TransformMatrix&) noexcept;

/**
 * \brief TransformMatrix represents a non-linear transformation Matrix in 3D-space.
 *
 * It is strongly suggested to prefer TransformMatrix over Matrix to perform different transformations,
 * because TransformMatrix performs calculations and presents results in the required format of the selected
 * graphical API. For example, OpenGL handles vectors as column-matrices, Vulkan API handles vectors as row-matrices.
 * TransformMatrix takes into account this difference.
 *
 * TransformMatrix is a wrapper over Matrix. That is why TransformMatrix is convertible from/to Matrix.
 * You can access underlying Matrix<4, 4> by getMatrix(). After that you can call
 * [getPointerToData()](\ref Matrix::getPointerToData()) to get the data,
 * which can be passed to the graphical API.
 *
 * \warning Creating TransformMatrix from the Matrix<4, 4> should be avoided,
 * because it can lead to wrong interpretation of the Matrix data during calculations for different graphical APIs.
 *
 * How to use:
 * - create TransformMatrix object for the model Matrix, perform transformations on it;
 * - create TransformMatrix object for the view Matrix, perform transformations on it;
 * - use createPerspectiveProjection() or createOrthographicProjection() to create projection Matrix;
 * - combine them by createCombinedTransformMatrix();
 * - use getMatrix().[getPointerToData()](\ref Matrix::getPointerToData()) to get raw data to pass to the graphical
 * API\n OR\n apply result TransformMatrix to the Vector by applyTransformationToVector().
 *
 * \code{.cpp}
 *
 * \endcode
 */
class TransformMatrix final
{
    public:
        /**
         * \brief Applies transformation matrix to the vector.
         *
         * Multiplies Vector<4> v and TransformMatrix transformation in the required order of the selected graphical API.
         */
        static constexpr Vec4 applyTransformationToVector(const Vec4& v, const TransformMatrix& transformation) noexcept
        {
            if constexpr (OGLS_VECTOR_IS_COLUMN)
            {
                return (transformation.getMatrix() * v).toVector();
            }
            else
            {
                return (v * transformation.getMatrix()).toVector();
            }
        }

        /**
         * \brief Creates combined TransfromMatrix from the TransformMatrix of the model, view and projection.
         *
         * Multiplies TransformMatrix -es in the required order of the selected graphical API.
         *
         * \param model      - the TransformMatrix of the object.
         * \param view       - the TransformMatrix of the Camera.
         * \param projection - the TransformMatrix of the projection (see createPerspectiveProjection(),
         * createOrthographicProjection()).
         */
        static constexpr TransformMatrix createCombinedTransformMatrix(const TransformMatrix& model,
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

        /**
         * \brief Creates an orthographic projection.
         *
         * An orthographic projection matrix defines a cube-like frustum box that defines the clipping space,
         * where each vertex outside this box is clipped. When creating an orthographic projection matrix
         * the width, height and length of the visible frustum are specified.
         * All the coordinates inside this frustum will end up within the NDC range after transformed
         * by its matrix and thus won't be clipped.
         *
         * \see [section Orthographic projection](https://learnopengl.com/Getting-started/Coordinate-Systems).
         */
        static constexpr TransformMatrix createOrthographicProjection() noexcept
        {
            return TransformMatrix{};
        }

        /**
         * \brief Creates a perspective projection.
         *
         * The projection matrix maps a given frustum range to clip space, but also manipulates the w value
         * of each vertex coordinate in such a way that the further away a vertex coordinate is from the viewer,
         * the higher this w component becomes. Once the coordinates are transformed to clip space
         * they are in the range -w to w (anything outside this range is clipped).
         *
         * \see [section Perspective projection](https://learnopengl.com/Getting-started/Coordinate-Systems).
         */
        static constexpr TransformMatrix createPerspectiveProjection() noexcept
        {
            return TransformMatrix{};
        }

        OGLS_DEFAULT_CONSTEXPR_NOEXCEPT_COPYABLE_MOVABLE(TransformMatrix)

        /**
         * Constructs a TransformMatrix with provided value of elements of the main diagonal and 0 elsewhere.
         *
         * \param defaultDiagonalValue - the value of axis coefficients on the main diagonal.
         * \param w                    - the value of W-component
         * (see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates)).
         */
        // clang-format off
        constexpr explicit TransformMatrix(float defaultDiagonalValue = {1.0f}, float w = {1.0f}) noexcept :
            m_matrix{{defaultDiagonalValue, 0.0f, 0.0f, 0.0f,
                      0.0f, defaultDiagonalValue, 0.0f, 0.0f,
                      0.0f, 0.0f, defaultDiagonalValue, 0.0f,
                      0.0f, 0.0f, 0.0f, w}}
        // clang-format on
        {
        }

        /**
         * Constructs a TransformMatrix with provided values of elements of the main diagonal and 0 elsewhere.
         *
         * \param xCoef - the value of X-axis coefficient on the main diagonal.
         * \param yCoef - the value of Y-axis coefficient on the main diagonal.
         * \param zCoef - the value of Z-axis coefficient on the main diagonal.
         * \param w     - the value of W-component
         * (see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates)).
         */
        // clang-format off
        constexpr TransformMatrix(float xCoef, float yCoef, float zCoef, float w = {1.0f}) noexcept :
            m_matrix{{xCoef, 0.0f, 0.0f, 0.0f,
                      0.0f, yCoef, 0.0f, 0.0f,
                      0.0f, 0.0f, zCoef, 0.0f,
                      0.0f, 0.0f, 0.0f, w}}
        // clang-format on
        {
        }

        /**
         * Constructs a TransformMatrix from the provided basis Vector.
         *
         * \param xBasis - the basis Vector of X-axis.
         * \param yBasis - the basis Vector of Y-axis.
         * \param zBasis - the basis Vector of Z-axis.
         * \param w      - the value of W-component
         * (see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates)).
         */
        constexpr TransformMatrix(const Vec3& xBasis, const Vec3& yBasis, const Vec3& zBasis, float w = {1.0f}) noexcept
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
        }

        /**
         * Constructs a TransformMatrix from the Matrix<4, 4>.
         *
         * \warning Creating TransformMatrix from the Matrix<4, 4> should be avoided,
         * because it can lead to wrong interpretation of the Matrix data during calculations for different graphical APIs.
         */
        constexpr explicit TransformMatrix(const Mat4& m) noexcept : m_matrix{m}
        {
        }

        /**
         * Returns a wrapped Matrix<4, 4>.
         */
        constexpr explicit operator Mat4() const noexcept
        {
            return m_matrix;
        }

        /**
         * \brief Returns std::string representation of the TransformMatrix object.
         *
         * \see toString().
         */
        operator std::string() const
        {
            return toString();
        }

        /**
         * Returns a wrapped Matrix<4, 4>.
         */
        constexpr Mat4 getMatrix() const noexcept
        {
            return m_matrix;
        }

        /**
         * \brief Returns a std::string representation of the TransformMatrix object with all elements.
         *
         * \param columnWidth - is a minimum number of characters (digits + dot) of TransformMatrix element to be displayed.
         */
        std::string toString(int columnWidth = 6) const
        {
            auto ss = std::stringstream{};

            ss << "TransformMatrix:\n";

            auto el = m_matrix.begin();
            ss << "  | " << std::left << std::setw(columnWidth) << (*el).getValue() << ", ";

            for (++el; el != m_matrix.end(); ++el)
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

    private:
        Mat4 m_matrix;

};  // class TransformMatrix

/**
 * \brief Checks equality of two TransformMatrix.
 *
 * \return true if two TransformMatrix have equal elements, false otherwise.
 */
constexpr bool operator==(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept
{
    return tm1.getMatrix() == tm2.getMatrix();
}

/**
 * \brief Checks if two TransformMatrix are not equal.
 *
 * \return true if two TransformMatrix have different elements, false otherwise.
 */
constexpr bool operator!=(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept
{
    return !(tm1 == tm2);
}

/**
 * \brief Multiplies two TransformMatrix.
 */
constexpr TransformMatrix operator*(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept
{
    return TransformMatrix{tm1.getMatrix() * tm2.getMatrix()};
}

}  // namespace ogls::mathCore

#undef OGLS_BEGIN_IT
#undef OGLS_CBEGIN_IT
#undef OGLS_CEND_IT
#undef OGLS_END_IT

#endif
