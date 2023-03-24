# Code style

## Indention
**4 spaces** are used for code indentation, not tabs.

## Naming
- **Camel case** is used.
- **Class name** begins with a capital letter. Example of **class name**: class **M**y**C**lass.
- **Function name** begins with a lower case letter, but all following words begin with a capital letter. Example of **function name**: void **m**y**F**unc().
- **variable name** begins with a lower case letter, but all following words begin with a capital letter. Example of **variable name**: int **m**y**V**ariable.
- **Class field name** begins with a prefix **m_**, but after the rule for variables naming is applied. Example of **class field name**: int **m_**class**F**ield.
- **Enum** are named as classes (**Note**: *enum class* is preferred). **Enum members** are in upper case. Example of enum: `enum class BufferBindingTarget : GLenum { ARRAY_BUFFER_BINDING = 0x8894, ATOMIC_COUNTER_BUFFER_BINDING = 0x92C1 }`.
- **Interface name** starts with **I**: class **IC**loneable { }.
- **File name** and **folder name** starts with a lower case letter, but all following words begin with a capital letter. Example of **file name**: **m**y**H**eader**F**ile.h.

## Braces
- Every curly brace is placed on a new line.
- Every **if**, **for** and **while** statement must have curly brace even if it contains only one statement.

## Reference/pointer
& and * in declaration of such variable is a part of a type, not a part of the name: `int* p1 = &a;` NOT `int *p1 = &a`.

## Class/struct declaration
The following template of class declaration is used:
```
class MyClass
{
    static_assert


    // TYPES
    public:
        ...

    protected:
        ...

    private:
        ...

    // METHODS
    public:
        static
        ...

    protected:
        static
        ...

    private:
        static
        ...

    // DATA
    public:
        static
        ...

    protected:
        static
        ...

    private:
        static
        ...

}
```
As long as you can, stick to the **Rule of Zero**, but if you have to write at least one of the Big Five, default the rest.

## Files and folders
- The source code of the project is separated on two parts - **app** and **core**. **Core** is a library, **app** is a demo application, which uses **core** library. 
- Source code of the **core** is placed in **src** folder, public headers of the **core** - in **root include** folder. **app** folder contains sources and headers of the demo application.
- Header and source files are placed in the same folder, except cases, when header file is a part of public **core** interface. In such a case header file is placed in **root include** folder and source file in **src** folder.
- In case of PIMPL, **struct Impl** must be declared in the file with the same name as the name of the class, which owns this Impl, suffixed by **Impl** in the name of the header file. For example if a class **MyClass** is declared in the file **myClass.h** in *include* folder, the header file with Impl must be declared in **src/.../myClassImpl.h**.
- Every file ends with one empty line.

### Header file template
```
#ifndef OPENGLCORE_SHADER_UNIFORMS_IMPL_H // contains namespace and file name
#define OPENGLCORE_SHADER_UNIFORMS_IMPL_H

#include "uniforms.h"

namespace openglCore::shader // all declarations in header files must be in some namespace,
    // NO DECLARATIONS IN GLOBAL SCOPE
{
    struct BaseUniform::BaseImpl
    {
        // METHODS
        /**
         * \brief Constructs new object.
         *
         * \param location - a location of the uniform in a shader program.
         * \param name - a name of the uniform variable.
         * \throw exceptions::GLRecAcquisitionException(), if location is 0
         */
        BaseImpl(GLuint shaderProgram, GLint location, std::string name);

        NOT_COPYABLE_MOVABLE(BaseImpl)

        // DATA
        /**
         * \brief Location (id) of the referenced OpenGL uniform variable in a shader program.
         */
        const GLint m_location = -1;

    };
}

#endif

```

### Source file template
Example of uniform.cpp file (see [Names and Order of Includes](https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes)):
```
// Include header files, which this source file implements
#include "uniforms.h"
#include "uniformsImpl.h"

// Include C-libs
#include <unistd.h>

// Include C++ libs
#include <vector>

// Include 3td-parties 
#inlcude <glad/glad.h>

// Include other headers
#include "exceptions.h"
#include "helpers/debugHelpers.h"
#include "helpers/openglHelpers.h"

// Use namespace, which implements
using namespace openglCore::shader;

// Declare local anonymous namespace, which is visible only inside this translation unit
namespace
{
    void someFunc();
}

// Declare and define Impl struct, if separate *Impl.h file is not created.

// Define methods and functions from uniforms.h
...


// IMPLEMENTATION
// Define methods and functions from uniformsImpl.h. // IMPLEMENTATION comment and two empty lines before it are necessary
...

// Define functions from local namespace
namespace
{
    void someFunc()
    { // for empty definition do like here
    }
}

// In the end of file create template instantiations if necessary
template class Uniform<GLfloat, 1>;

```

## AAA (Almost Always Auto) and AAU (Almost Always Unicorn)
Based on ['ES.11: Use auto to avoid redundant repetition of type names'](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es11-use-auto-to-avoid-redundant-repetition-of-type-names), ['Using auto whenever possible'](https://subscription.packtpub.com/book/programming/9781786465184/1/ch01lvl1sec5/using-auto-whenever-possible), ['“auto to stick” and Changing Your Style'](https://www.fluentcpp.com/2018/09/28/auto-stick-changing-style/), ['Auto for Types, but Not for Concepts'](https://www.fluentcpp.com/2020/12/04/auto-for-types-but-not-for-concepts/) and [this Stackoverflow answer with good links](https://stackoverflow.com/a/69467830)
it was decided to use the following conventions:
- `auto a = std::string{"text"}; // from C++17 the compiler is required to elide the temporary object as well as the move operation` (see ['“auto to stick” and Changing Your Style'](https://www.fluentcpp.com/2018/09/28/auto-stick-changing-style/)).

    For initialization of non-primitive types `{}` should be used (see [GotW #1 Solution: Variable Initialization – or Is It?](https://herbsutter.com/2013/05/09/gotw-1-solution/)).
However be aware of overloads, which accepts `std::initializer_list`;
- `auto i = int{42}; // when creating some variable of primitive type`.

    Such declaration allows to be more explicit with the type of the variable without using of types literals, make variable always initialized and 
avoid possible misunderstanding between `auto lst = {1}; // lst is an initializer list` and `auto x{1}; // x is an int (in C++17; initializer_list in C++11)` (see ['ES.11'](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es11-use-auto-to-avoid-redundant-repetition-of-type-names));
- `auto iFromFun = someType{getSomeIntValue()}; // when it is important to specify, that i is of someType type and to not allow narrowing`

## Patterns
- In **core** PIMPL should be used. When PIMPL is used, the **Impl** struct should be declared in separate ***Impl.h** file in **src** folder. It is useful in cases, when class, which owns **Impl**, has friends, which must have access to internal details of the class. In such a case these friends must know declaration of **Impl** struct to use it. 
- In **app** using of PIMPL is not obligatory.

## Some rules from [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Try to avoid forward declarations of entities defined in another project ([see Forward Declarations](https://google.github.io/styleguide/cppguide.html#Forward_Declarations)).
- Default arguments are banned on virtual functions, where they don't work properly, and in cases where the specified default might not evaluate to the same value depending on when it was evaluated ([see Default Arguments](https://google.github.io/styleguide/cppguide.html#Default_Arguments)).
- Use brace initialization to convert arithmetic types (e.g., int64_t{x}) ([see Casting](https://google.github.io/styleguide/cppguide.html#Casting)).
- Use the prefix form (++i) of the increment and decrement operators unless you need postfix semantics ([see Preincrement and Predecrement](https://google.github.io/styleguide/cppguide.html#Preincrement_and_Predecrement)).
- Do not use an unsigned type merely to assert that a variable is non-negative ([see On Unsigned Integers](https://google.github.io/styleguide/cppguide.html#Integer_Types)).
