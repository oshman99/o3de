/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzToolsFramework/Viewport/ViewportMessages.h>
#include <AzCore/Math/ToString.h>
#include <AzCore/std/string/string.h>
#include <AzCore/Debug/Trace.h>

namespace AzManipulatorTestFramework
{
    //! Base class for derived immediate and retained action dispatchers.
    template <typename DerivedDispatcherT>
    class ActionDispatcher
    {
    public:
        virtual ~ActionDispatcher() = default;

        //! Enable grid snapping.
        DerivedDispatcherT* EnableSnapToGrid();
        //! Disable grid snapping.
        DerivedDispatcherT* DisableSnapToGrid();
        //! Set the grid size.
        DerivedDispatcherT* GridSize(float size);
        //! Enable/disable action logging.
        DerivedDispatcherT* LogActions(bool logging);
        //! Output a trace debug message.
        template <typename... Args>
        DerivedDispatcherT* Trace(const char* format, const Args&... args);
        //! Set the camera state.
        DerivedDispatcherT* CameraState(const AzFramework::CameraState& cameraState);
        //! Set the left mouse button down.
        DerivedDispatcherT* MouseLButtonDown();
        //! Set the left mouse button up.
        DerivedDispatcherT* MouseLButtonUp();
        //! Set the keyboard modifier button down.
        DerivedDispatcherT* KeyboardModifierDown(const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier);
        //! Set the keyboard modifier button up.
        DerivedDispatcherT* KeyboardModifierUp(const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier);
        //! Set the mouse position to the specified screen space position.
        DerivedDispatcherT* MousePosition(const AzFramework::ScreenPoint& position);
        //! Expect the selected manipulator to be interacting.
        DerivedDispatcherT* ExpectManipulatorBeingInteracted();
        //! Do not expect the selected manipulator to be interacting.
        DerivedDispatcherT* ExpectManipulatorNotBeingInteracted();
        //! Set the world transform of the specified entity.
        DerivedDispatcherT* SetEntityWorldTransform(AZ::EntityId entityId, const AZ::Transform& transform);
        //! Select the specified entity.
        DerivedDispatcherT* SetSelectedEntity(AZ::EntityId entity);
        //! Select the specified entities.
        DerivedDispatcherT* SetSelectedEntities(const AzToolsFramework::EntityIdList& entities);
        //! Enter component mode for the specified component type's uuid.
        DerivedDispatcherT* EnterComponentMode(const AZ::Uuid& uuid);
        //! Break out to the debugger mid action sequence (note: do not leave uses in production code).
        DerivedDispatcherT* DebugBreak();
        //!  Enter component mode for the specified component type.
        template <typename ComponentT>
        DerivedDispatcherT* EnterComponentMode();
    protected:
        // Actions to be implemented by derived immediate and retained action dispatchers.
        virtual void EnableSnapToGridImpl() = 0;
        virtual void DisableSnapToGridImpl() = 0;
        virtual void GridSizeImpl(float size) = 0;
        virtual void CameraStateImpl(const AzFramework::CameraState& cameraState) = 0;
        virtual void MouseLButtonDownImpl() = 0;
        virtual void MouseLButtonUpImpl() = 0;
        virtual void MousePositionImpl(const AzFramework::ScreenPoint& position) = 0;
        virtual void KeyboardModifierDownImpl(const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier) = 0;
        virtual void KeyboardModifierUpImpl(const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier) = 0;
        virtual void ExpectManipulatorBeingInteractedImpl() = 0;
        virtual void ExpectManipulatorNotBeingInteractedImpl() = 0;
        virtual void SetEntityWorldTransformImpl(AZ::EntityId entityId, const AZ::Transform& transform) = 0;
        virtual void SetSelectedEntityImpl(AZ::EntityId entity) = 0;
        virtual void SetSelectedEntitiesImpl(const AzToolsFramework::EntityIdList& entities) = 0;
        virtual void EnterComponentModeImpl(const AZ::Uuid& uuid) = 0;
        template <typename... Args>
        void Log(const char* format, const Args&... args);
        bool m_logging = false;
    private:
        const char* KeyboardModifierString(const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier);
    };

    template <typename DerivedDispatcherT>
    template <typename... Args>
    void ActionDispatcher<DerivedDispatcherT>::Log(const char* format, const Args&... args)
    {
        if (m_logging)
        {
            AZ_Printf("ActionDispatcher", format, args...);
        }
    }

    template <typename DerivedDispatcherT>
    template <typename... Args>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::Trace(const char* format, const Args&... args)
    {
        Log(format, args...);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::EnableSnapToGrid()
    {
        Log("Enabling SnapToGrid");
        EnableSnapToGridImpl();
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::DisableSnapToGrid()
    {
        Log("Disabling SnapToGrid");
        DisableSnapToGridImpl();
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::GridSize(float size)
    {
        Log("GridSize: %f", size);
        GridSizeImpl(size);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::LogActions(bool logging)
    {
        m_logging = logging;
        Log("Log actions: %s", m_logging ? "enabled" : "disabled");
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::CameraState(const AzFramework::CameraState& cameraState)
    {
        Log("Camera state: p(%f, %f, %f) d(%f, %f, %f)",
            float(cameraState.m_position.GetX()), float(cameraState.m_position.GetY()), float(cameraState.m_position.GetZ()),
            float(cameraState.m_forward.GetX()), float(cameraState.m_forward.GetY()), float(cameraState.m_forward.GetZ()));
        CameraStateImpl(cameraState);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::MouseLButtonDown()
    {
        Log("%s", "Mouse left button down");
        MouseLButtonDownImpl();
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::MouseLButtonUp()
    {
        Log("%s", "Mouse left button up");
        MouseLButtonUpImpl();
        return static_cast<DerivedDispatcherT*>(this);
    }
    template <typename DerivedDispatcherT>
    const char* ActionDispatcher<DerivedDispatcherT>::KeyboardModifierString(
        const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier)
    {
        using namespace AzToolsFramework::ViewportInteraction;
        switch (keyModifier)
        {
        case KeyboardModifier::Alt:
            return "Alt";
        case KeyboardModifier::Control:
            return "Ctrl";
        case KeyboardModifier::Shift:
            return "Shift";
        case KeyboardModifier::None:
            return "None";
        default: return "Unknown modifier";
        }
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::KeyboardModifierDown(
        const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier)
    {
        Log("Keyboard modifier down: %s", KeyboardModifierString(keyModifier));
        KeyboardModifierDownImpl(keyModifier);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::KeyboardModifierUp(
        const AzToolsFramework::ViewportInteraction::KeyboardModifier& keyModifier)
    {
        Log("Keyboard modifier up: %s", KeyboardModifierString(keyModifier));
        KeyboardModifierUpImpl(keyModifier);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::MousePosition(const AzFramework::ScreenPoint& position)
    {
        Log("Mouse position: (%i, %i)", position.m_x, position.m_y);
        MousePositionImpl(position);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::ExpectManipulatorBeingInteracted()
    {
        Log("Expecting manipulator interacting");
        ExpectManipulatorBeingInteractedImpl();
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::ExpectManipulatorNotBeingInteracted()
    {
        Log("Not expecting manipulator interacting");
        ExpectManipulatorNotBeingInteractedImpl();
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::SetEntityWorldTransform(
        AZ::EntityId entityId, const AZ::Transform& transform)
    {
        Log("Setting entity world transform: %s", AZ::ToString(transform).c_str());
        SetEntityWorldTransformImpl(entityId, transform);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::SetSelectedEntity(
        AZ::EntityId entity)
    {
        Log("Selecting entity: %u", static_cast<AZ::u64>(entity));
        SetSelectedEntityImpl(entity);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::SetSelectedEntities(
        const AzToolsFramework::EntityIdList& entities)
    {
        for (const auto& entity : entities)
        {
            Log("Selecting entity %u", static_cast<AZ::u64>(entity));
        }
        SetSelectedEntitiesImpl(entities);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::EnterComponentMode(const AZ::Uuid& uuid)
    {
        Log("Entering component mode: %s", uuid.ToString<AZStd::string>().c_str());
        EnterComponentModeImpl(uuid);
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::DebugBreak()
    {
        Log("Breaking to debugger");
        AZ::Debug::Trace::Break();
        return static_cast<DerivedDispatcherT*>(this);
    }

    template <typename DerivedDispatcherT>
    template <typename ComponentT>
    DerivedDispatcherT* ActionDispatcher<DerivedDispatcherT>::EnterComponentMode()
    {
        EnterComponentMode(AZ::AzTypeInfo<ComponentT>::Uuid());
        return static_cast<DerivedDispatcherT*>(this);
    }
} // namespace AzManipulatorTestFramework
