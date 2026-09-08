// MIT License
//
// Copyright (c) <2026> <Matthew Eshleman - https://covemountainsoftware.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef CPPUTEST_FOR_SST_LIB_CMS_SST_FLAT_STATE_MACHINE_TASK_HPP
#define CPPUTEST_FOR_SST_LIB_CMS_SST_FLAT_STATE_MACHINE_TASK_HPP

#include "cms_flat_state_machine.hpp"
#include "sst.hpp"

namespace cms
{
    using SstEvtFlatStateMachine = FlatStateMachine<SST::Evt>;

    /**
     * An SST task that is a flat statemachine too, pardon the multiple-inheritance.
     * Operates on SST:Evt events.
     * Usage of this Active Object + Flat State Machine, implies that signals must
     * adhere to the FlatStateMachine's requirements (i.e. certain event signal values
     * are reserved, see cms_standard_signals.hpp)
     */
    class SstFlatStateMachineTask : public SST::Task, public SstEvtFlatStateMachine
    {
    public:
        void init(const SST::Evt* const ie) override
        {
            //init the corresponding flat state machine
            this->Initialize();

            //if we have an initial event, dispatch it now.
            if (ie != nullptr)
            {
                this->dispatch(ie);
            }
        }

        void dispatch(const SST::Evt* e) override
        {
            this->ProcessEvent(e);
        }
    };
} // cms

#endif //CPPUTEST_FOR_SST_LIB_CMS_SST_FLAT_STATE_MACHINE_TASK_HPP
