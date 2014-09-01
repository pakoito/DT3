def callbacks():

	f = open('Callback.hpp','w')
	f.write('''#ifndef DT3_CALLBACK
#define DT3_CALLBACK
//==============================================================================
///	
///	File: Callback.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/CheckedCast.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class Callback0 {
    public:
        DEFINE_TYPE_BASE(Callback0)
        virtual ~Callback0(void)    {}
        virtual void        operator ()(void) = 0;
		virtual DTboolean	operator ==	(const Callback0& rhs) const = 0;
};

template <typename CLASS>
class Callback0Impl: public Callback0 {
    public:		         
        DEFINE_TYPE(Callback0Impl,Callback0)

        Callback0Impl	(CLASS *c, void (CLASS::*fn)(void))
        {
            _obj = c;
            _fn = fn;
        }

        virtual void    operator ()(void)
        {
            if (_obj && _fn)
                (_obj->*_fn)();
        }
        
		virtual DTboolean	operator ==	(const Callback0& rhs) const
        {
            const Callback0Impl<CLASS> *rhs_type = checked_cast<const Callback0Impl<CLASS> *>(&rhs);
            return rhs_type && (_obj == rhs_type->_obj) && (_fn == rhs_type->_fn);
        }

	public:
        CLASS           *_obj;
        void            (CLASS::*_fn)(void);
};

class CallbackStatic0Impl: public Callback0 {
    public:		         
        DEFINE_TYPE(CallbackStatic0Impl,Callback0)

        CallbackStatic0Impl	(void (*fn)(void))
        {
            _fn = fn;
        }

        virtual void    operator ()(void)
        {
            if (_fn)
                (_fn)();
        }
        
		virtual DTboolean	operator ==	(const Callback0& rhs) const
        {
            const CallbackStatic0Impl *rhs_type = checked_cast<const CallbackStatic0Impl *>(&rhs);
            return rhs_type && (_fn == rhs_type->_fn);
        }

	public:
        void            (*_fn)(void);
};

//==============================================================================
//==============================================================================
\n\n''')

	for x in range(1, 16):
			
		# Class 1
		f.write("template <");
		
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		
		f.write(">\n");
		
		f.write("class Callback%d {\n" % x)
		f.write("\tpublic:\n")
		f.write("\t\tDEFINE_TYPE_BASE(Callback%d)\n" % x)
		f.write("\t\tvirtual ~Callback%d(void)    {}\n" % x)

		f.write("\t\tvirtual void operator ()(")
		for t in range(0, x):
			f.write("T%d t%d" % (t,t))
			if (t < x-1):
				f.write(",")
		f.write(") = 0;\n")
		
		f.write("\t\tvirtual DTboolean	operator ==	(const Callback%d& rhs) const = 0;\n" % x)
		f.write("};\n\n")
		
		# Class 2
		f.write("template <typename CLASS,");
		
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		
		f.write(">\n");
		
		f.write("class Callback%dImpl: public Callback%d<" % (x,x))
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")

		f.write("> {\n")
		f.write("\tpublic:\n")
		f.write("\t\ttypedef Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> CBT;\n")
		f.write("\t\tDEFINE_TYPE(Callback%dImpl,CBT)\n\n" % x)

		f.write("\t\tCallback%dImpl (CLASS *c, void (CLASS::*fn)(" % x)
		for t in range(0, x):
			f.write("T%d t%d" % (t,t))
			if (t < x-1):
				f.write(",")
		f.write("))\n")
		
		f.write("\t\t{\n")
		f.write("\t\t\t_obj = c;\n")
		f.write("\t\t\t_fn = fn;\n")
		f.write("\t\t}\n\n")

		f.write("\t\tvirtual void operator ()(")
		for t in range(0, x):
			f.write("T%d t%d" % (t,t))
			if (t < x-1):
				f.write(",")
		f.write(")\n")
		f.write("\t\t{\n")
		f.write("\t\t\tif (_obj && _fn)\n")
		f.write("\t\t\t\t(_obj->*_fn)(")
		for t in range(0, x):
			f.write("t%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(");\n")
		f.write("\t\t}\n\n")

		f.write("\t\tvirtual DTboolean operator == (const CBT& rhs) const\n")
		f.write("\t\t{\n")
		f.write("\t\t\tconst Callback%dImpl<" % x)
		f.write("CLASS,")
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> *rhs_type = checked_cast<const Callback%dImpl<CLASS," % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> *>(&rhs);\n")
		f.write("\t\t\treturn rhs_type && (_obj == rhs_type->_obj) && (_fn == rhs_type->_fn);\n")
		f.write("\t\t}\n\n")

		f.write("\tpublic:\n")
		f.write("\t\tCLASS *_obj;\n")
		f.write("\t\tvoid (CLASS::*_fn)(")
		for t in range(0, x):
			f.write("T%d t%d" % (t,t))
			if (t < x-1):
				f.write(",")
		f.write(");\n")
		f.write("};\n\n")


		# Class 3
		f.write("template <");
		
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		
		f.write(">\n");
		
		f.write("class CallbackStatic%dImpl: public Callback%d<" % (x,x))
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")

		f.write("> {\n")
		f.write("\tpublic:\n")
		f.write("\t\ttypedef Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> CBT;\n")
		f.write("\t\tDEFINE_TYPE(CallbackStatic%dImpl,CBT)\n\n" % x)

		f.write("\t\tCallbackStatic%dImpl (void (*fn)(" % x)
		for t in range(0, x):
			f.write("T%d t%d" % (t,t))
			if (t < x-1):
				f.write(",")
		f.write("))\n")
		
		f.write("\t\t{\n")
		f.write("\t\t\t_fn = fn;\n")
		f.write("\t\t}\n\n")

		f.write("\t\tvirtual void operator ()(")
		for t in range(0, x):
			f.write("T%d t%d" % (t,t))
			if (t < x-1):
				f.write(",")
		f.write(")\n")
		f.write("\t\t{\n")
		f.write("\t\t\tif (_fn)\n")
		f.write("\t\t\t\t(*_fn)(")
		for t in range(0, x):
			f.write("t%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(");\n")
		f.write("\t\t}\n\n")

		f.write("\t\tvirtual DTboolean operator == (const CBT& rhs) const\n")
		f.write("\t\t{\n")
		f.write("\t\t\tconst CallbackStatic%dImpl<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> *rhs_type = checked_cast<const CallbackStatic%dImpl<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> *>(&rhs);\n")
		f.write("\t\t\treturn rhs_type && (_fn == rhs_type->_fn);\n")
		f.write("\t\t}\n\n")

		f.write("\tpublic:\n")
		f.write("\t\tvoid (*_fn)(")
		for t in range(0, x):
			f.write("T%d t%d" % (t,t))
			if (t < x-1):
				f.write(",")
		f.write(");\n")
		f.write("};\n")

		f.write('''\n//==============================================================================
//==============================================================================\n\n''')

	f.write('''template<typename CLASS>
inline std::shared_ptr<Callback0> make_callback(CLASS *c, void (CLASS::*fn)(void))
{
\treturn std::shared_ptr<Callback0>(new ("Callback0") Callback0Impl<CLASS>(c, fn));
}\n\n''')


	for x in range(1, 16):

		f.write("template<typename CLASS,")
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">\n")

		f.write("inline std::shared_ptr<Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> > make_callback(CLASS *c, void (CLASS::*fn)(")
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("))\n")
		
		f.write("{\n")
		f.write("\treturn std::shared_ptr<Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> >(new (\"Callback%d\") Callback%dImpl<CLASS," % (x,x) )
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">(c, fn));\n")
		f.write("}\n\n")

	f.write('''//==============================================================================
//==============================================================================\n\n''')

	f.write('''inline std::shared_ptr<Callback0> make_callback(void (*fn)(void))
{
\treturn std::shared_ptr<Callback0>(new ("Callback0") CallbackStatic0Impl(fn));
}\n\n''')

	for x in range(1, 16):

		f.write("template<")
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">\n")

		f.write("inline std::shared_ptr<Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> > make_callback(void (*fn)(")
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("))\n")
		
		f.write("{\n")
		f.write("\treturn std::shared_ptr<Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> >(new (\"Callback%d\") CallbackStatic%dImpl<" % (x,x) )
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">(fn));\n")
		f.write("}\n\n")



	f.write('''//==============================================================================
//==============================================================================

} // DT3

#endif
''')


	f.close()


callbacks()
