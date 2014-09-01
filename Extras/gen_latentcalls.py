def latentcalls():

	f = open('LatentCall.hpp','w')
	f.write('''#ifndef DT3_LATENTCALL
#define DT3_LATENTCALL
//==============================================================================
///	
///	File: LatentCall.hpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Utility/Callback.hpp"
#include <memory>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
//==============================================================================

class LatentCall {
    public:
        DEFINE_TYPE_BASE(LatentCall)

                        LatentCall          (void)                      {}
                        LatentCall          (const LatentCall &rhs)     {}
        LatentCall&     operator =          (const LatentCall& rhs)     {	return (*this);     }
                        ~LatentCall         (void)                      {}

    public:
        virtual void    fire            (void) = 0;
};

//==============================================================================
//==============================================================================

class LatentCall0: public LatentCall {
    public:		         
        DEFINE_TYPE(LatentCall0,LatentCall)
        
        LatentCall0 (std::shared_ptr<Callback0> cb) 
        	:	_cb(cb)
        {}
        
        LatentCall0  (const LatentCall0 &rhs)
            :   LatentCall(rhs),
            	_cb(rhs._cb)
        {}
        
        LatentCall0&     operator =          (const LatentCall0& rhs)
        {
            LatentCall::operator = (rhs);
            _cb = rhs._cb;
            
            return (*this);
        }
        
        ~LatentCall0         (void)
        {
        
        }


        void fire       () {
            (*_cb)();
        }

    private:
        std::shared_ptr<Callback0>     _cb;
};

//==============================================================================
//==============================================================================
\n''')

	for x in range(1, 16):
			
		# Class 1
		f.write("template <");
		
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		
		f.write(">\n");
		
		f.write("class LatentCall%d: public LatentCall {\n" % x)

		f.write("\tpublic:\n");
		f.write("\t\tDEFINE_TYPE(LatentCall%d,LatentCall)\n\n" % x)
		
		f.write("\t\tLatentCall%d (std::shared_ptr<Callback%d<" % (x,x))
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> > cb,\n")
		
		for t in range(0, x):
			f.write("\t\t\tconst typename TypeTraits<T%d>::NonRefType &t%d" % (t,t))
			if (t < x-1):
				f.write(",\n")
			else:
				f.write("\n")
		
		f.write("\t\t)\n")
		f.write("\t\t\t:\t_cb(cb),\n")
		
		for t in range(0, x):
			f.write("\t\t\t\t_t%d(t%d)" % (t,t))
			if (t < x-1):
				f.write(",\n")
			else:
				f.write("\n")

		f.write("\t\t{}\n\n")



		f.write("\t\tLatentCall%d (const LatentCall%d<" % (x,x))
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">& rhs)\n")
		f.write("\t\t\t:\tLatentCall(rhs),\n")
		
		f.write("\t\t\t\t_cb(rhs._cb),\n")
		
		for t in range(0, x):
			f.write("\t\t\t\t_t%d(rhs._t%d)" % (t,t))
			if (t < x-1):
				f.write(",\n")
			else:
				f.write("\n")

		f.write("\t\t{}\n\n")



		f.write("\t\tLatentCall%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">& operator = (const LatentCall%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">& rhs)\n")		
		f.write("\t\t{\n")		
		f.write("\t\t\tLatentCall::operator = (rhs);\n")		
		f.write("\t\t\t_cb = rhs._cb;\n")		

		for t in range(0, x):
			f.write("\t\t\t_t%d = rhs.t%d;\n" % (t,t))

		f.write("\t\t\treturn (*this);\n")
		f.write("\t\t}\n\n")
		
		f.write("\t\t~LatentCall%d (void)\n" % x)
		f.write("\t\t{\n")
		f.write("\t\t}\n\n")

		f.write("\t\tvoid fire (void) {\n")
		f.write("\t\t\t(*_cb)(")
		for t in range(0, x):
			f.write("_t%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(");\n")
		f.write("\t\t}\n\n")

		f.write("\tprivate:\n")
		
		f.write("\t\tstd::shared_ptr<Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> > _cb;\n")
		
		for t in range(0, x):
			f.write("\t\ttypename TypeTraits<T%d>::BaseType _t%d;\n" % (t,t))
			
		f.write("};\n\n")
		
	f.write('''//==============================================================================
//==============================================================================\n\n''')

	f.write('''template<typename CLASS>
inline std::shared_ptr<LatentCall> makeLatentCall(CLASS *c, void (CLASS::*fn)(void))
{
    return  std::shared_ptr<LatentCall>(
                new LatentCall0(
                    std::shared_ptr<Callback0>(new ("Callback0") Callback0Impl<CLASS>(c, fn))
                )
            );
}

''')

	for x in range(1, 16):
		f.write("template<typename CLASS, ")
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">\n")
		
		f.write("inline std::shared_ptr<LatentCall> makeLatentCall(CLASS *c, void (CLASS::*fn)(")
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("),\n")
		for t in range(0, x):
			f.write("\t\ttypename TypeTraits<T%d>::NonRefType t%d" % (t,t))
			if (t < x-1):
				f.write(",")
			f.write("\n")
		f.write(")\n")

		f.write("{\n")
		f.write("\treturn std::shared_ptr<LatentCall>(\n")
		f.write("\t\t\t\tnew LatentCall%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">(\n")

		f.write("\t\t\t\t\tstd::shared_ptr<Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> >(new (\"Callback%d\") Callback%dImpl<CLASS," % (x,x) )
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">(c, fn)),\n")
		f.write("\t\t\t\t\t")
		for t in range(0, x):
			f.write("t%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("\n\t\t\t\t)\n");
		f.write("\t\t\t);\n");
		f.write("}\n\n");

	f.write('''//==============================================================================
//==============================================================================\n\n''')

	f.write('''inline std::shared_ptr<LatentCall> makeLatentCall(void (*fn)(void))
{
    return  std::shared_ptr<LatentCall>(
                new LatentCall0(
                    std::shared_ptr<Callback0>(new ("Callback0") CallbackStatic0Impl(fn))
                )
            );
}

''')

	for x in range(1, 16):
		f.write("template<")
		for t in range(0, x):
			f.write("typename T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">\n")
		
		f.write("inline std::shared_ptr<LatentCall> makeLatentCall(void (*fn)(")
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("),\n")
		for t in range(0, x):
			f.write("\t\ttypename TypeTraits<T%d>::NonRefType t%d" % (t,t))
			if (t < x-1):
				f.write(",")
			f.write("\n")

		f.write(")\n")

		f.write("{\n")
		f.write("\treturn std::shared_ptr<LatentCall>(\n")
		f.write("\t\t\t\tnew LatentCall%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">(\n")

		f.write("\t\t\t\t\tstd::shared_ptr<Callback%d<" % x)
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("> >(new (\"Callback%d\") CallbackStatic%dImpl<" % (x,x) )
		for t in range(0, x):
			f.write("T%d" % t)
			if (t < x-1):
				f.write(",")
		f.write(">(fn)),\n")
		f.write("\t\t\t\t\t")
		for t in range(0, x):
			f.write("t%d" % t)
			if (t < x-1):
				f.write(",")
		f.write("\n\t\t\t\t)\n");
		f.write("\t\t\t);\n");
		f.write("}\n\n");

	f.write('''//==============================================================================
//==============================================================================

} // DT3

#endif
''')

	f.close()
	
	


latentcalls()
