#ifndef DT3_SCRIPTINGPAGEFLIPPER
#define DT3_SCRIPTINGPAGEFLIPPER
//==============================================================================
///	
///	File: ScriptingRadioButton.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// Boolean sequencer.
//==============================================================================

class ScriptingRadioButton: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingRadioButton,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingRadioButton    (void);
									ScriptingRadioButton    (const ScriptingRadioButton &rhs);
        ScriptingRadioButton &		operator =				(const ScriptingRadioButton &rhs);	
        virtual                     ~ScriptingRadioButton   (void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                remove_from_world       (void);

	public:

		/// Called to initialize the object
		virtual void				initialize				(void);

        // Event handlers for each input
        void                        in1                     (PlugNode *sender)  {   flip(sender,&_in1);    }
        void                        in2                     (PlugNode *sender)  {   flip(sender,&_in2);    }
        void                        in3                     (PlugNode *sender)  {   flip(sender,&_in3);    }
        void                        in4                     (PlugNode *sender)  {   flip(sender,&_in4);    }
        void                        in5                     (PlugNode *sender)  {   flip(sender,&_in5);    }
        void                        in6                     (PlugNode *sender)  {   flip(sender,&_in6);    }
        void                        in7                     (PlugNode *sender)  {   flip(sender,&_in7);    }
        void                        in8                     (PlugNode *sender)  {   flip(sender,&_in8);    }
        void                        in9                     (PlugNode *sender)  {   flip(sender,&_in9);    }
        void                        in10                    (PlugNode *sender)  {   flip(sender,&_in10);   }
        void                        in11                    (PlugNode *sender)  {   flip(sender,&_in11);   }
        void                        in12                    (PlugNode *sender)  {   flip(sender,&_in12);   }
        void                        in13                    (PlugNode *sender)  {   flip(sender,&_in13);   }
        void                        in14                    (PlugNode *sender)  {   flip(sender,&_in14);   }
        void                        in15                    (PlugNode *sender)  {   flip(sender,&_in15);   }
        void                        in16                    (PlugNode *sender)  {   flip(sender,&_in16);   }
        void                        in17                    (PlugNode *sender)  {   flip(sender,&_in17);   }
        void                        in18                    (PlugNode *sender)  {   flip(sender,&_in18);   }
        void                        in19                    (PlugNode *sender)  {   flip(sender,&_in19);   }
        void                        in20                    (PlugNode *sender)  {   flip(sender,&_in20);   }

	private:
        void                        flip                    (PlugNode *sender, Event *in);

        Event                       _in1;
        Event                       _in2;
        Event                       _in3;
        Event                       _in4;
        Event                       _in5;
        Event                       _in6;
        Event                       _in7;
        Event                       _in8;
        Event                       _in9;
        Event                       _in10;
        Event                       _in11;
        Event                       _in12;
        Event                       _in13;
        Event                       _in14;
        Event                       _in15;
        Event                       _in16;
        Event                       _in17;
        Event                       _in18;
        Event                       _in19;
        Event                       _in20;

        Event                       _set1;
        Event                       _reset1;
        Event                       _set2;
        Event                       _reset2;
        Event                       _set3;
        Event                       _reset3;
        Event                       _set4;
        Event                       _reset4;
        Event                       _set5;
        Event                       _reset5;
        Event                       _set6;
        Event                       _reset6;
        Event                       _set7;
        Event                       _reset7;
        Event                       _set8;
        Event                       _reset8;
        Event                       _set9;
        Event                       _reset9;
        Event                       _set10;
        Event                       _reset10;
        Event                       _set11;
        Event                       _reset11;
        Event                       _set12;
        Event                       _reset12;
        Event                       _set13;
        Event                       _reset13;
        Event                       _set14;
        Event                       _reset14;
        Event                       _set15;
        Event                       _reset15;
        Event                       _set16;
        Event                       _reset16;
        Event                       _set17;
        Event                       _reset17;
        Event                       _set18;
        Event                       _reset18;
        Event                       _set19;
        Event                       _reset19;
        Event                       _set20;
        Event                       _reset20;
};

//==============================================================================
//==============================================================================

} // DT3

#endif
