#ifndef DT3_FACTORY
#define DT3_FACTORY
//==============================================================================
///	
///	File: Factory.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Types/Base/BaseInclude.hpp"
#include "DT3Core/Types/Utility/Callback.hpp"
#include "DT3Core/System/StaticInitializer.hpp"
#include <map>
#include <set>
#include <memory>
#include <string>

namespace DT3 {

//==============================================================================
// Forward declarations
//==============================================================================

class BaseClass;
class Progress;
class FilePath;

//==============================================================================
//==============================================================================

class Factory {
    private:
								Factory						(void);	
								Factory						(const Factory &rhs);
		Factory &				operator =					(const Factory &rhs);		
								~Factory					(void);
            
    public:
	
		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<BaseClass>		create_world                (const std::string cid);

		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<BaseClass>		create_object               (const std::string cid, DTboolean net_create = false);

		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<BaseClass>		create_object_from_stream   (   const FilePath &pathname,
                                                                                std::shared_ptr<Progress> progress = NULL,
                                                                                std::shared_ptr<Callback<std::shared_ptr<BaseClass>>> obj_loaded_cb = NULL);
				
		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<BaseClass>		create_importer             (const std::string ext);

		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<BaseClass>		create_unit_test            (const std::string cid);

		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<BaseClass>		create_command              (const std::string cid);

		/// Description
		/// \param param description
		/// \return description
		static std::shared_ptr<BaseClass>		create_tool                 (const std::string cid);
        
    
		/// Description
		/// \param param description
		/// \return description
        static void                             set_creation_callback       (std::shared_ptr<Callback<std::shared_ptr<BaseClass>>> cb)    {   _factory_created_callback = cb; }
        
    private:
        static std::shared_ptr<Callback<std::shared_ptr<BaseClass>>>     _factory_created_callback;

};

//==============================================================================
//==============================================================================

class CreatorBase {
    public:
				CreatorBase(void)	{}
		virtual ~CreatorBase(void)	{}
		
        virtual std::shared_ptr<BaseClass>      create_object (void) const = 0;
        virtual std::string                     stored_class_id (void) const = 0;
};


template <typename T>
class Creator: public CreatorBase {
    public:
				Creator(void)	{}
		virtual ~Creator(void)	{}
	
        virtual std::shared_ptr<BaseClass>      create_object (void) const      {   return T::create(); }
        virtual std::string                     stored_class_id (void) const    {   return std::string(T::class_id_static());       }
};

//==============================================================================
/// Factory registration
//==============================================================================

extern std::map<std::string, std::shared_ptr<CreatorBase>>&    factory_map              (void);
extern std::map<std::string, std::shared_ptr<CreatorBase>>&    factory_importer_map     (void);
extern std::map<std::string, std::shared_ptr<CreatorBase>>&    unit_test_map            (void);
extern std::map<std::string, std::shared_ptr<CreatorBase>>&    command_map              (void);
extern std::map<std::string, std::set<std::string>>&           placeable_map            (void);
extern std::map<std::string, std::string>&                     adapter_map              (void);
extern std::map<std::string, std::set<std::string>>&           component_map            (void);
extern std::map<std::string, std::shared_ptr<CreatorBase>>&    tool_map                 (void);
extern std::map<std::string, std::shared_ptr<CreatorBase>>&    world_map                (void);
extern std::map<std::string, std::set<std::string>>&           script_map               (void);

#define IMPLEMENT_FACTORY_CREATION(Class)	\
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::factory_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
            } \
        } registration_obj##Class; \
    }

#define IMPLEMENT_FACTORY_UNIT_TEST(Class)	\
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::unit_test_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryUnitTestCreator") Creator<Class>); \
            } \
        } registration_obj##Class; \
    }

#define IMPLEMENT_FACTORY_COMMAND(Class)	\
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::command_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryCommandCreator") Creator<Class>); \
            } \
        } registration_obj##Class; \
    }
    
#define IMPLEMENT_FACTORY_IMPORTER(Class,Extension)	\
    namespace { \
        struct registration_class##Class##Extension: public StaticInitializerCallback { \
            void initialize() { \
                DT3::factory_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
                DT3::factory_importer_map()[TO_STRING(Extension)] = std::shared_ptr<CreatorBase>(new ("FactoryImporterCreator") Creator<Class>); \
            } \
        } registration_obj##Class##Extension; \
    }

#define IMPLEMENT_FACTORY_CREATION_PLACEABLE(Class,Category,Adapter) \
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::factory_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
                DT3::placeable_map()[Category].insert( Class::class_id_static() ); \
                DT3::adapter_map()[Class::class_id_static()] = Adapter; \
            } \
        } registration_obj##Class; \
    }

#define IMPLEMENT_FACTORY_CREATION_COMPONENT(Class,Category,Adapter) \
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::factory_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
                DT3::component_map()[Category].insert( Class::class_id_static() ); \
                DT3::adapter_map()[Class::class_id_static()] = Adapter; \
            } \
        } registration_obj##Class; \
    }

#define IMPLEMENT_FACTORY_CREATION_WORLD(Class) \
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::factory_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
                DT3::world_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
            } \
        } registration_obj##Class; \
    }
    
#define IMPLEMENT_FACTORY_CREATION_SCRIPT(Class,Category,Adapter) \
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::factory_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
                if (std::string(Category) != "")																						\
                    DT3::script_map()[Category].insert( Class::class_id_static() ); \
                DT3::adapter_map()[Class::class_id_static()] = #Adapter; \
            } \
        } registration_obj##Class; \
    }

#define IMPLEMENT_FACTORY_TOOL(Class)	\
    namespace { \
        struct registration_class##Class: public StaticInitializerCallback { \
            void initialize() { \
                DT3::tool_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryToolCreator") Creator<Class>); \
                DT3::factory_map()[Class::class_id_static()] = std::shared_ptr<CreatorBase>(new ("FactoryObjectCreator") Creator<Class>); \
            } \
        } registration_obj##Class; \
    }					
				
//==============================================================================
//==============================================================================

} // DT3

#endif
