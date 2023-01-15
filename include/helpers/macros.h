#ifndef MACROS_H
#define MACROS_H

#define DEFAULT_COPYABLE(ClassX) \
	ClassX(const ClassX&) = default;\
	ClassX& operator=(const ClassX&) = default;

#define DEFAULT_MOVABLE(ClassX) \
	ClassX(ClassX&&) = default;\
	ClassX& operator=(ClassX&&) = default;

#define DEFAULT_COPYABLE_MOVABLE(ClassX) \
	ClassX(const ClassX&) = default;\
	ClassX(ClassX&&) = default;\
	ClassX& operator=(const ClassX&) = default;\
	ClassX& operator=(ClassX&&) = default;

#define NOT_COPYABLE(ClassX) \
	ClassX(const ClassX&) = delete;\
	ClassX& operator=(const ClassX&) = delete;

#define NOT_MOVABLE(ClassX) \
	ClassX(ClassX&&) = delete;\
	ClassX& operator=(ClassX&&) = delete;

#define NOT_COPYABLE_MOVABLE(ClassX) \
	ClassX(const ClassX&) = delete;\
	ClassX(ClassX&&) = delete;\
	ClassX& operator=(const ClassX&) = delete;\
	ClassX& operator=(ClassX&&) = delete;

#endif
