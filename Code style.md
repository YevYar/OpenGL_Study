# Code style

## Indention
**4 spaces** are used for code indentation, not tabs.

## Naming
- **Camel case** is used.
- **Class name** begins with a capital letter. Example of **class name**: class **M**y**C**lass.
- **Function name** begins with a lower case letter, but all following words begin with a capital letter. Example of **function name**: void **m**y**F**unc().
- **Variable name** begins with a lower case letter, but all following words begin with a capital letter. Example of **variable name**: int **m**y**V**ariable.
- **Private class field name** begins with a prefix **m_**, but after the rule for variables naming is applied. **Public fields of classes or structs** should be named without prefix. Example of **class field name**: int **m_**class**F**ield.
- **Enum** are named as classes (**Note**: *enum class* is preferred). **Enum members** are named in camel case with the first capital letter. Example of enum: `enum class BufferBindingTarget : GLenum { ArrayBufferBinding = 0x8894, AtomicCounterBufferBinding = 0x92C1 }`.
- **The names of all types** — classes, structs, type aliases, enums, and type template parameters — have the same naming convention. Type names should **start with a capital letter** and have a **capital letter for each new word**. No underscores.
- **Interface name** starts with **I**: class **IC**loneable { }.
- **Global constants** in header or source files are named in uppercase with underscores as separator between words: `constexpr auto SOME_INT_CONSTANT = int{1};`.
- **Namespace name** begins with a lower case letter, but all following words begin with a capital letter. Example of **namespace name**: namespace **m**y**N**amespace.
- **File name** and **folder name** starts with a lower case letter, but all following words begin with a capital letter. Example of **file name**: **m**y**H**eader**F**ile.h.

## Braces
- Every curly brace is placed on a new line.
- Every **if**, **for** and **while** statement must have curly brace even if it contains only one statement.

## Reference/pointer
& and * in declaration of such variable is a part of a type, not a part of the name: `int* p1 = &a;` NOT `int *p1 = &a`.

## Namespaces
- Every header file must declare all elements in some namespace.
- The base namespace of *include* folder in the root folder of the project is `ogls` (abbreviation for *OpenGL Study*).
- All headers in subfolders of *include* folder declare elements in namespace with the name of the folder. For example, some header from *helpers* folder must declares its stuff in namespace `ogls::helpers`.
- Don't use using-statements like `using namespace ogls::oglCore::shader` in the top of **.cpp* files. Using of using-statements is allowed only in class declarations or inside the function scope. In such case *using-statements* must be in alphabetical order and followed by **2 empty lines**.
- All code in **.cpp** files must be defined inside `namespace someNamespace { ... }`. Anonymous namespaces are also defined inside this namespace to not pollute global namespace of the translation unit (see [this Stackoverflow answer](https://stackoverflow.com/a/29368872/11658801)).
- The end `}` of namespace must be followed by two whitespaces and a line comment with the namespace name: `}  // namespace ogls::oglCore::shader`
- The order of declaration and empty lines inside the namespace must be as follow:
  ```
  namespace someNamespace
  {
  // Position of nested namespace can be different. In general it should be on the start of parent namespace,
  // but if something from parent namespace is used inside the inner namespace, inner namespace can be declared
  // after it.
  namespace
  {
  }  // namespace

  using 1
  using 2

  type 1  // also can be before using-statement, if using-statement uses this type
  // this empty line can be omitted if type 1 is declaration, not a definition 
  type 2

  function 1
  // this empty line can be omitted if function 1 is declaration, not a definition 
  function 2


  variable 1  // also can be before types and functions if they use it
  variable 2

  /* anonymous namespace definition */ namespace
  {
  }  // namespace

  }  // namespace someNamespace
  ```

## Class/struct declaration
The following template of class declaration is used:
```
class MyClass
{
    static_assert


    // TYPES
    public:
        using
        ...

    protected:
        using
        ...

    private:
        using
        ...

    // METHODS
    public:
        static

        using-declarations

        constructors = default;
        constructors, which have custom implementation
        constructors = delete;
        destructor

        operations

        methods (own) in alphabetical order

        methods (own virtual) in alphabetical order

        methods (override) in alphabetical order

        methods (pure virtual) in alphabetical order
        ...

    protected:
        static

        using-declarations
        
        constructors = default;
        constructors, which have custom implementation
        constructors = delete;
        destructor

        operations

        methods (own) in alphabetical order

        methods (own virtual) in alphabetical order

        methods (override) in alphabetical order

        methods (pure virtual) in alphabetical order
        ...

    private:
        static

        using-declarations
        
        constructors = default;
        constructors, which have custom implementation
        constructors = delete;
        destructor

        operations

        methods (own) in alphabetical order

        methods (own virtual) in alphabetical order

        methods (override) in alphabetical order

        methods (pure virtual) in alphabetical order
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


        friend classes
        friend functions

};  // class MyClass
```
- As long as you can, stick to the **Rule of Zero**, but if you have to write at least one of the Big Five, default the rest.
- Order methods of one group in alphabetical order.
- Use alphabetical order of class fields. If necessary, group class fields and apply alphabetical order inside this groups.
- Alphabetical order may be broken, if functions are inlined and they use other function or variable, which must be defined before the function. 
### Line breaks in class/struct declaration:
- 2 empty lines after static asserts.
- 1 empty line before access modifier.
- 1 empty line after each of methods groups, but no empty lines between methods of one group.
- 1 empty line after static fields.
- 2 empty lines before *friends*. It's needed to emphasize, that *friends* aren't part of private section.
- 1 empty line before closing `}`.

## Files and folders
- The source code of the project is separated on two parts - **app** and **core**. **Core** is a library, **app** is a demo application, which uses **core** library. 
- Source code of the **core** is placed in **src** folder, public headers of the **core** - in **root include** folder. **app** folder contains sources and headers of the demo application.
- Header and source files are placed in the same folder, except cases, when header file is a part of public **core** interface. In such a case header file is placed in **root include** folder and source file in **src** folder.
- **core** base namespace is `ogls`, **app** base namespace is `app`.
- In case of PIMPL, **class Impl** must be declared in the file with the same name as the name of the class, which owns this Impl, suffixed by **Impl** in the name of the header file. For example if a class **MyClass** is declared in the file **myClass.h** in *include* folder, the header file with Impl must be declared in **src/.../myClassImpl.h**.
- Every file ends with one empty line.

### Header file template
```
#ifndef OGLS_OGLCORE_SHADER_UNIFORMS_IMPL_H // template is OGLS_<namespace>_<file name>_H
 // or OGLS_<path to file starting from include folder>_<file name>_H if no namespace in header file (for example, helpers/macros.h).
#define OGLS_OGLCORE_SHADER_UNIFORMS_IMPL_H

#include "uniforms.h"

namespace ogls::oglCore::shader // all declarations in header files must be in some namespace,
    // NO DECLARATIONS IN GLOBAL SCOPE
{
class BaseUniform::BaseImpl
{
    public:
        /**
         * \brief Constructs new object.
         *
         * \param location - a location of the uniform in a shader program.
         * \param name - a name of the uniform variable.
         * \throw exceptions::GLRecAcquisitionException(), if location is 0
         */
        BaseImpl(GLuint shaderProgram, GLint location, std::string name);
        NOT_COPYABLE_MOVABLE(BaseImpl);

    public:
        /**
         * \brief Location (ID) of the referenced OpenGL uniform variable in a shader program.
         */
        const GLint m_location = {-1};

};  // class BaseUniform::BaseImpl

}  // namespace ogls::oglCore::shader

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

namespace ogls::oglCore::shader
{
// Declare local anonymous namespace, which is visible only inside this translation unit
namespace
{
    void someFunc();

}  // namespace

// Declare and define Impl struct, if separate *Impl.h file is not created.

// Define methods and functions from uniforms.h
...

//------ IMPLEMENTATION

// Define methods and functions from uniformsImpl.h. // IMPLEMENTATION comment and two empty lines before it are necessary
...

// Define functions from local namespace
namespace
{
    void someFunc()
    { // for empty definition do like here
    }

}  // namespace

// In the end of file create template instantiations if necessary
template class Uniform<GLfloat, 1>;

}  // namespace ogls::oglCore::shader

```
- First, constructors and destructors are defined: constructor without parameters, constructors with parameters, copy and move constructors, destructor.
- Method definitions must be provided in the same order, in which they were declared in header file.

## AAA (Almost Always Auto) and AAU (Almost Always Unicorn)
Based on ['ES.11: Use auto to avoid redundant repetition of type names'](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es11-use-auto-to-avoid-redundant-repetition-of-type-names), ['Using auto whenever possible'](https://subscription.packtpub.com/book/programming/9781786465184/1/ch01lvl1sec5/using-auto-whenever-possible), ['“auto to stick” and Changing Your Style'](https://www.fluentcpp.com/2018/09/28/auto-stick-changing-style/), ['Auto for Types, but Not for Concepts'](https://www.fluentcpp.com/2020/12/04/auto-for-types-but-not-for-concepts/) and [this Stackoverflow answer with good links](https://stackoverflow.com/a/69467830)
it was decided to use the following conventions:
- `auto a = std::string{"text"}; // from C++17 the compiler is required to elide the temporary object as well as the move operation` (see ['“auto to stick” and Changing Your Style'](https://www.fluentcpp.com/2018/09/28/auto-stick-changing-style/)).

    For initialization of non-primitive types `{}` should be used (see [GotW #1 Solution: Variable Initialization – or Is It?](https://herbsutter.com/2013/05/09/gotw-1-solution/)).
However be aware of overloads, which accepts `std::initializer_list`;
- `auto i = int{42}; // when creating some variable of primitive type`.

    Such declaration allows to be more explicit with the type of the variable without using of types literals, make variable always initialized and 
avoid possible misunderstanding between `auto lst = {1}; // lst is an initializer list` and `auto x{1}; // x is an int (in C++17; initializer_list in C++11)` (see ['ES.11'](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es11-use-auto-to-avoid-redundant-repetition-of-type-names));
- `auto isTrue = true; // only for bool type`
- `auto iFromFun = someType{getSomeIntValue()}; // when it is important to specify, that i is of someType type and to not allow narrowing`
- **BUT** in type declarations type is specified before the field name: `int m_someField = {0};`.

## Patterns
- In **core** PIMPL should be used. When PIMPL is used, the **Impl** class should be declared in separate ***Impl.h** file in **src** folder. It is useful in cases, when class, which owns **Impl**, has friends, which must have access to internal details of the class. In such a case these friends must know declaration of **Impl** class to use it. 
- In **app** using of PIMPL is not obligatory.

## Macros
- Using of macros must be limited.
- Macros names consist of upper case letters, words in names are separated by underscores: `#define INSTANTIATE_UNIFORM(Type)`.
- `#define` macros right before you use them, and `#undef` them right after ([see Preprocessor Macros](https://google.github.io/styleguide/cppguide.html#Preprocessor_Macros)).
- Macros should be prefixed by `OGLS_` ([see Macro Names](https://google.github.io/styleguide/cppguide.html#Macro_Names)).
- After macros call `;` is necessary: `SOME_MACROS(x);`. However, in type declaration `;` must be omitted: `SOME_MACROS(x)`.

## Some necessary empty lines
- Before closing `}` of class/struct declaration.
- Before closing `}` of namespace.
- Before `#endif`.
- Between functions (except methods in type declaration) and between function and type declaration.

## Noexcept
- Destructors must be declared as `noexcept`. If destructor calls potentially-throwing function, `try-catch` block must wrap this call.
- Functions, which use macros `OGLS_CALL()`, cannot be declared as `noexcept`, because `OGLS_CALL()` can be changed to become potentially-throwing.

## Some rules from [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Try to avoid forward declarations of entities defined in another project ([see Forward Declarations](https://google.github.io/styleguide/cppguide.html#Forward_Declarations)).
- Default arguments are banned on virtual functions, where they don't work properly, and in cases where the specified default might not evaluate to the same value depending on when it was evaluated ([see Default Arguments](https://google.github.io/styleguide/cppguide.html#Default_Arguments)).
- Use brace initialization to convert arithmetic types (e.g., int64_t{x}) ([see Casting](https://google.github.io/styleguide/cppguide.html#Casting)).
- Use the prefix form (++i) of the increment and decrement operators unless you need postfix semantics ([see Preincrement and Predecrement](https://google.github.io/styleguide/cppguide.html#Preincrement_and_Predecrement)).
- Do not use an unsigned type merely to assert that a variable is non-negative ([see On Unsigned Integers](https://google.github.io/styleguide/cppguide.html#Integer_Types)).
- Switch statements should always have a default case. If the default case should never execute, treat this as an error ([see Switch Statements](https://google.github.io/styleguide/cppguide.html#Switch_Statements)).
- Fall-through from one case label to another must be annotated using the `[[fallthrough]];` attribute ([see Switch Statements](https://google.github.io/styleguide/cppguide.html#Switch_Statements)).
- Use a struct only for passive objects that carry data; everything else is a class ([see Structs vs. Classes](https://google.github.io/styleguide/cppguide.html#Structs_vs._Classes)). 
