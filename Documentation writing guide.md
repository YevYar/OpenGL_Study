# Code documentation style

## Struct, class, enum, namespace, using declaration and other type declarations
<pre>
/**
 * \brief <b>Name</b> description.
 *
 * Detailed description.
 */
</pre>

## Field of class/struct
<pre>
/**
 * \brief <b>Noun</b> (what is it) and brief description.
 *
 * Detailed description.
 */
</pre>

## Function, method, macros
<pre>
/**
 * \brief <b>Add<u>S</u></b>, <b>find<u>S</u></b> ...
 *
 * Detailed description.
 *
 * \param <b>param name</b> - description.
 * \return description.
 * \throw list of exceptions.
 */
</pre>

- **Default and deleted** methods mustn't have any comment!
- **Impl structs** in *.cpp files don't have to provide any comments.