/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */


#ifndef CRYINCLUDE_EDITOR_PREFERENCESSTDPAGES_H
#define CRYINCLUDE_EDITOR_PREFERENCESSTDPAGES_H
#pragma once

#include "Include/IPreferencesPage.h"
#include <AzCore/std/functional.h>
#include <AzCore/std/containers/vector.h>

//////////////////////////////////////////////////////////////////////////
class CStdPreferencesClassDesc
    : public IPreferencesPageClassDesc
    , public IPreferencesPageCreator
{
    ULONG m_refCount;
    AZStd::vector< AZStd::function< IPreferencesPage*() > > m_pageCreators;
public:
    CStdPreferencesClassDesc();
    virtual ~CStdPreferencesClassDesc() = default;

    //////////////////////////////////////////////////////////////////////////
    // IUnkown implementation.
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(const IID& riid, void** ppvObj);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();
    //////////////////////////////////////////////////////////////////////////

    virtual REFGUID ClassID();

    //////////////////////////////////////////////////////////////////////////
    virtual int GetPagesCount();
    virtual IPreferencesPage* CreateEditorPreferencesPage(int index) override;
};

#endif // CRYINCLUDE_EDITOR_PREFERENCESSTDPAGES_H

