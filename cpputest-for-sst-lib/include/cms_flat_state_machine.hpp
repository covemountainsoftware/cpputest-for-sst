// Originally found at:
// https://github.com/covemountainsoftware/activeObjectUnitTestingDemo
//
// MIT License
//
// Copyright (c) <2019-2026> <Matthew Eshleman - https://covemountainsoftware.com>
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

#ifndef CPPUTEST_FOR_SST_LIB_CMS_FLAT_STATE_MACHINE_HPP
#define CPPUTEST_FOR_SST_LIB_CMS_FLAT_STATE_MACHINE_HPP

#include "cms_standard_signals.hpp"

namespace cms
{
    /**
     * @brief The FlatStateMachine class provides for simple flat statemachine
     *        functionality.
     *
     * @param template param EventT: event type processed by this state machine
     *
     * @note derived state machines must provide a concrete implementation of
     *       the initial pseudo state via the method InitialPseudoState().
     */
    template <typename EventT>
    class FlatStateMachine
    {
    public:
        virtual ~FlatStateMachine() = default;

        /**
         * @brief Initialize - method to call to initialize the state machine.
         *                     Must be called before any event processing.
         */
        void Initialize();

        /**
         * @brief ProcessEvent - process a single event.
         *
         * @param event - ptr to the event to process
         *
         */
        void ProcessEvent(const EventT* event);

    protected:
        struct StateRtn;
        using StateMethodHandler = StateRtn (FlatStateMachine<EventT>::*)(const EventT* const event);

        struct StateRtn
        {
            StateRtn() = default;

            explicit StateRtn(const StateMethodHandler method) : mMethod(method)
            {
            }

            StateMethodHandler mMethod;
        };

        /**
         * @brief InitialPseudoState - pure virtual. Derived class must provide
         *                             a concrete implementation.
         *
         * @return The state to transition to
         *
         * @note: the implementation MUST return a handler via the TransitionTo() method
         */
        virtual StateRtn InitialPseudoState(const EventT* event) = 0;

        /**
         * @brief TransitionTo - Request a Local Transition to the desired target state
         * @param targetState - pointer to member, i.e the handler for the target state
         */
        template <typename StateMethodT>
        StateRtn TransitionTo(StateMethodT targetState)
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-negative-value"
            return StateRtn(static_cast<StateMethodHandler>(targetState));
#pragma GCC diagnostic pop
        };

        /**
         * @brief Handled - The event was handled.
         * @return
         */
        StateRtn Handled()
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-negative-value"
            return StateRtn(static_cast<StateMethodHandler>(mCurrent));
#pragma GCC diagnostic pop
        }

    private:
        const EventT EnterEvent = {SM_ENTER_SIG};
        const EventT ExitEvent = {SM_EXIT_SIG};

        StateMethodHandler mCurrent = nullptr;
    };

    template <typename EventT>
    void FlatStateMachine<EventT>::Initialize()
    {
        //get the initial desired state
        mCurrent = &FlatStateMachine<EventT>::InitialPseudoState;
        auto rtn = ((this)->*(mCurrent))(&EnterEvent);

        //now enter the initial desired state
        mCurrent = rtn.mMethod;
        ((this)->*(mCurrent))(&EnterEvent);
    }

    template <typename EventT>
    void FlatStateMachine<EventT>::ProcessEvent(const EventT* const event)
    {
        auto rtn = ((this)->*(mCurrent))(event);
        if (rtn.mMethod != mCurrent)
        {
            ((this)->*(mCurrent))(&ExitEvent);
            mCurrent = rtn.mMethod;
            ((this)->*(mCurrent))(&EnterEvent);
        }
    }
} //namespace cms


#endif //CPPUTEST_FOR_SST_LIB_CMS_FLAT_STATE_MACHINE_HPP
