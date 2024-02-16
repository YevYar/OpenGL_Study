#ifndef OGLS_MATHCORE_TRANSFORM_MATRIX_H
#define OGLS_MATHCORE_TRANSFORM_MATRIX_H

#include "mathCore/matrix.h"
#include "mathCore/vector.h"

namespace ogls::mathCore
{
#ifdef TREAT_VECTORS_AS_COLUMNS
constexpr inline auto OGLS_VECTOR_IS_COLUMN = true;
#elif TREATE_VECTORS_AS_ROWS
constexpr inline auto OGLS_VECTOR_IS_COLUMN = false;
#else
#    error "Please select a graphics API (USE_OPENGL or USE_VULKAN)"
#endif

/**
 * \brief TransformMatrix represents a non-linear transformation Matrix in 3D-space.
 *
 * It is strongly suggested to prefer TransformMatrix over Matrix to perform different transformations,
 * because TransformMatrix performs calculations and presents results in the required format of the selected
 * graphical API. For example, OpenGL handles vectors as column-matrices, Vulkan API handles vectors as row-matrices.
 * TransformMatrix takes into account this difference.
 *
 * TransformMatrix is a wrapper over Matrix. That is why TransformMatrix is convertible from/to Matrix.
 * You can access underlying Matrix<4, 4> by getResultMatrix(). After that you can call
 * [getPointerToData()](\ref Matrix::getPointerToData()) to get the data,
 * which can be passed to the graphical API.
 *
 * \warning Creating TransformMatrix from the Matrix<4, 4> should be avoided,
 * because it can lead to wrong interpretation of the Matrix data during calculations for different graphical APIs.
 *
 * TransformMatrix is lazy-evaluated. It means that the result Matrix<4, 4> is calculated every time,
 * when getResultMatrix() is called, except cases when no new Operation hasn't been added since the last call of
 * getResultMatrix(). The add... methods (for example, addTranslation()) don't lead to recalculation
 * of the result Matrix<4, 4>.
 *
 * All methods of TransformMatrix and functions on it use getResultMatrix() to get calculated
 * Matrix<4, 4>, which is a result of the performing of operations on the initial Matrix<4, 4>.
 * You can access the initial Matrix<4, 4> by getInitialMatrix(). The returned Matrix<4, 4> is unchanged
 * since the construction of this TransformMatrix.
 *
 * How to use:
 * - create TransformMatrix object for the model Matrix, perform transformations on it;
 * - create TransformMatrix object for the view Matrix, perform transformations on it;
 * - use createPerspectiveProjection() or createOrthographicProjection() to create projection Matrix;
 * - combine them by createCombinedTransformMatrix();
 * - use getResultMatrix().[getPointerToData()](\ref Matrix::getPointerToData()) to get raw data to pass to the
 * graphical API\n OR\n apply result TransformMatrix to the Vector by applyTransformationToVector().
 *
 * \code{.cpp}
 *
 * \endcode
 * \see getResultMatrix().
 */
class TransformMatrix final
{
    private:
        /**
         * \brief Operation is a base abstract class for all operations, which can be performed on TransformMatrix.
         */
        class Operation
        {
            public:
                constexpr virtual ~Operation() noexcept = default;

                /**
                 * \brief Returns a copy of the operation.
                 */
                virtual std::unique_ptr<Operation> clone() const                   = 0;
                /**
                 * \brief Performs an operation on the passed Matrix<4, 4>.
                 *
                 * \param m - a Matrix<4, 4> to perform the operation on.
                 */
                virtual void                       execute(Mat4& m) const noexcept = 0;

        };  // class Operation

        /**
         * \brief Translation performs a translation operation in the direction of the passed to constructor Vector<3>.
         */
        class Translation final : public Operation
        {
            public:
                /**
                 * \brief Constructs a Translation to perform a translation operation in the direction of the Vector<3>.
                 *
                 * \param direction - the direction of the translation operation.
                 */
                constexpr explicit Translation(const Vec3& direction) noexcept : m_direction{direction}
                {
                }

                std::unique_ptr<Operation> clone() const override
                {
                    return std::make_unique<Translation>(*this);
                }

                constexpr void execute(Mat4& m) const noexcept override
                {
                    auto i = size_t{0};

                    for (const auto& el : m_direction)
                    {
                        if constexpr (OGLS_VECTOR_IS_COLUMN)
                        {
                            m[i++][3] = el;
                        }
                        else
                        {
                            m[3][i++] = el;
                        }
                    }
                }

            private:
                /**
                 * \brief The direction of the translation operation.
                 */
                Vec3 m_direction;

        };  // class Translation

    public:
        /**
         * \brief Applies transformation matrix to the vector.
         *
         * Multiplies Vector<4> v and TransformMatrix transformation in the required order of the selected graphical API.
         *
         * \return the Vector<4> with the applied transformation.
         */
        static Vec4 applyTransformationToVector(const Vec4& v, const TransformMatrix& transformation) noexcept;
        /**
         * \brief Creates combined TransfromMatrix from the TransformMatrix of the model, view and projection.
         *
         * Multiplies TransformMatrix -es in the required order of the selected graphical API.
         *
         * \param model      - the TransformMatrix of the object.
         * \param view       - the TransformMatrix of the Camera.
         * \param projection - the TransformMatrix of the projection (see createPerspectiveProjection(),
         * createOrthographicProjection()).
         * \return the combined TransformMatrix of the model, view and projection.
         */
        static TransformMatrix createCombinedTransformMatrix(const TransformMatrix& model, const TransformMatrix& view,
                                                             const TransformMatrix& projection) noexcept;
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
         * \return the orthographic projection TransformMatrix.
         */
        static TransformMatrix createOrthographicProjection() noexcept;
        /**
         * \brief Creates a perspective projection.
         *
         * The projection matrix maps a given frustum range to clip space, but also manipulates the w value
         * of each vertex coordinate in such a way that the further away a vertex coordinate is from the viewer,
         * the higher this w component becomes. Once the coordinates are transformed to clip space
         * they are in the range -w to w (anything outside this range is clipped).
         *
         * \see [section Perspective projection](https://learnopengl.com/Getting-started/Coordinate-Systems).
         * \return the perspective projection TransformMatrix.
         */
        static TransformMatrix createPerspectiveProjection(float fovy, float aspect, float zNear, float zFar) noexcept;

        OGLS_DEFAULT_MOVABLE(TransformMatrix)

        /**
         * \brief Constructs a TransformMatrix with provided value of elements of the main diagonal and 0 elsewhere.
         *
         * \param defaultDiagonalValue - the value of axis coefficients on the main diagonal.
         * \param w                    - the value of W-component
         * (see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates)).
         */
        explicit TransformMatrix(float defaultDiagonalValue = {1.0f}, float w = {1.0f}) noexcept;
        /**
         * \brief Constructs a TransformMatrix with provided values of elements of the main diagonal and 0 elsewhere.
         *
         * \param xCoef - the value of X-axis coefficient on the main diagonal.
         * \param yCoef - the value of Y-axis coefficient on the main diagonal.
         * \param zCoef - the value of Z-axis coefficient on the main diagonal.
         * \param w     - the value of W-component
         * (see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates)).
         */
        TransformMatrix(float xCoef, float yCoef, float zCoef, float w = {1.0f}) noexcept;
        /**
         * \brief Constructs a TransformMatrix from the provided basis Vector.
         *
         * \param xBasis - the basis Vector of X-axis.
         * \param yBasis - the basis Vector of Y-axis.
         * \param zBasis - the basis Vector of Z-axis.
         * \param w      - the value of W-component
         * (see [Homogeneous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates)).
         */
        TransformMatrix(const Vec3& xBasis, const Vec3& yBasis, const Vec3& zBasis, float w = {1.0f}) noexcept;
        /**
         * \brief Constructs a TransformMatrix from the Matrix<4, 4>.
         *
         * \warning Creating TransformMatrix from the Matrix<4, 4> should be avoided,
         * because it can lead to wrong interpretation of the Matrix data during calculations for different graphical APIs.
         */
        explicit TransformMatrix(const Mat4& m) noexcept;
        /**
         * \brief Constructs new TransformMatrix as copy of the other TransformMatrix.
         */
        TransformMatrix(const TransformMatrix& other);

        /**
         * \brief Copies the state of the other TransformMatrix.
         */
        TransformMatrix& operator=(const TransformMatrix& other);
        /**
         * \brief Returns Matrix<4, 4>, which is the result of applying of transformations to the wrapped Matrix<4, 4>.
         *
         * \see getResultMatrix().
         */
        explicit operator Mat4() const noexcept;

        /**
         * \brief Adds a Translation operation to the queue of operations on the TransformMatrix.
         *
         * The Translation operation is not executed at the moment. The Translation is performed during the execution
         * of getResultMatrix().
         *
         * \param direction - the direction Vector<3> of the Translation operation.
         * \see getResultMatrix().
         */
        void        addTranslation(const Vec3& direction);
        /**
         * \brief Returns an initial wrapped Matrix<4, 4>.
         */
        Mat4        getInitialMatrix() const noexcept;
        /**
         * \brief Returns Matrix<4, 4>, which is the result of applying of transformations to the wrapped Matrix<4, 4>.
         *
         * Performs Operation -es in the required order of the selected graphical API.
         *
         * For example, we want to rotate an object and translate it after that. In case of Vulkan API these operations
         * must be applied in the same order: Object * RotationTransformation * TranslationTransformation.
         * However, in case of OpenGL it must be written as TranslationTransformation * RotationTransformation * Object.
         *
         * The initial Matrix<4, 4> isn't modified (getInitialMatrix() returns the same Matrix<4, 4> as before).
         *
         * The method is lazy-evaluated. The result Matrix<4, 4> is calculated only during first call or after adding
         * new Operation after last call of this method.
         *
         * \return Matrix<4, 4> with all added operations applied.
         */
        Mat4        getResultMatrix() const noexcept;
        /**
         * \brief Returns a std::string representation of the TransformMatrix object with all elements.
         *
         * The elements of the current result Matrix<4, 4> are displayed.
         *
         * \param columnWidth - is a minimum number of characters (digits + dot) of TransformMatrix element to be
         * displayed.
         * \see getResultMatrix().
         */
        std::string toString(int columnWidth = 6) const;

    private:
        /**
         * \brief The number of added Operation -es at the moment of the last call of getResultMatrix().
         */
        mutable size_t                          m_lastOperationQueueSize = {0};
        /**
         * \brief Last calculated in getResultMatrix() result Matrix<4, 4>.
         */
        mutable Mat4                            m_lastResultMatrix;
        /**
         * \brief The initial wrapped Matrix<4, 4>.
         */
        Mat4                                    m_matrix;
        /**
         * \brief Added Operation -es.
         */
        std::vector<std::unique_ptr<Operation>> m_operationQueue;

};  // class TransformMatrix

/**
 * \brief Prints into the stream a std::string representation of the TransformMatrix object.
 */
std::ostream&   operator<<(std::ostream& out, const TransformMatrix& tm);
/**
 * \brief Checks equality of two TransformMatrix.
 *
 * \return true if two TransformMatrix have equal elements, false otherwise.
 */
bool            operator==(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept;
/**
 * \brief Checks if two TransformMatrix are not equal.
 *
 * \return true if two TransformMatrix have different elements, false otherwise.
 */
bool            operator!=(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept;
/**
 * \brief Multiplies two TransformMatrix.
 */
TransformMatrix operator*(const TransformMatrix& tm1, const TransformMatrix& tm2) noexcept;

}  // namespace ogls::mathCore

#endif
