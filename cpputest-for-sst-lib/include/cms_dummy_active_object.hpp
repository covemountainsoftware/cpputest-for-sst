/// @brief A dummy active object (SST Task), useful for testing interactions
///        in a unit test.
/// @ingroup
/// @cond
///***************************************************************************
///MIT License
///
///Copyright (c) 2026 Cove Mountain Software (aka Matthew Eshleman)
///
///Permission is hereby granted, free of charge, to any person obtaining a copy
///of this software and associated documentation files (the "Software"), to deal
///in the Software without restriction, including without limitation the rights
///to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
///copies of the Software, and to permit persons to whom the Software is
///furnished to do so, subject to the following conditions:
///
///The above copyright notice and this permission notice shall be included in all
///copies or substantial portions of the Software.
///
///THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
///SOFTWARE.
///***************************************************************************
/// @endcond

#ifndef CMS_SST_DUMMY_ACTIVE_OBJECT_HPP
#define CMS_SST_DUMMY_ACTIVE_OBJECT_HPP

#include "sst.hpp"
#include <functional>
#include <memory>

namespace cms::test
{
    /// The Dummy Active Object, derived from SST::Task,
    /// may be used when an active object (AO) under test is
    /// interacting with another AO during a test.
    class DummyActiveObject : public SST::Task
    {
    public:
        enum class EventBehavior
        {
            CALLBACK, //will call the provided callback
        };

        using PostedEventHandler = std::function<void(SST::Evt const*)>;

        DummyActiveObject() :
            Task()
            , m_eventHandler(nullptr)
            , m_behavior(EventBehavior::CALLBACK)
        {
        }

        explicit DummyActiveObject(const EventBehavior behavior) :
            Task()
            , m_eventHandler(nullptr)
            , m_behavior(behavior)
        {
        }

        DummyActiveObject(const DummyActiveObject&) = delete;
        DummyActiveObject& operator=(const DummyActiveObject&) = delete;
        DummyActiveObject(DummyActiveObject&&) = delete;
        DummyActiveObject& operator=(DummyActiveObject&&) = delete;

        void SetPostedEventHandler(const PostedEventHandler& handler)
        {
            if (m_behavior == EventBehavior::CALLBACK)
            {
                m_eventHandler = handler;
            }
        }

        void init(SST::Evt const* ie) override
        {
            if ((m_eventHandler != nullptr) && (ie != nullptr))
            {
                m_eventHandler(ie);
            }
        }

        void dispatch(SST::Evt const* e) override
        {
            if ((m_eventHandler != nullptr) && (e != nullptr))
            {
                m_eventHandler(e);
            }
        }

    private:
        PostedEventHandler m_eventHandler;
        EventBehavior m_behavior;
    };

    using DefaultDummyActiveObject = DummyActiveObject;
    using DefaultDummyActiveObjectUniquePtr = std::unique_ptr<DefaultDummyActiveObject>;
}

#endif   // CMS_SST_DUMMY_ACTIVE_OBJECT_HPP
