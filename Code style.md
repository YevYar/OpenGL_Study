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
        ...

    protected:
        ...

    private:
        ...

    // DATA
    public:
        ...

    protected:
        ...

    private:
        ...

}
```

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

namespace openglCore::shader
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
Example of uniform.cpp file: 
```
// Include header files, which this source file implements
#include "uniforms.h"
#include "uniformsImpl.h"

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

// Declare methods and functions from uniforms.h
...


// IMPLEMENTATION
// Declare methods and functions from uniformsImpl.h. // IMPLEMENTATION comment is necessary
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
## Patterns
- In **core** PIMPL should be used.

