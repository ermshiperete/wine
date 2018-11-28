/*
 * COM+ Services
 *
 * Copyright 2018 Eberhard Beilharz
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#define COBJMACROS

#include "config.h"

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "ole2.h"
#include "rpcproxy.h"
#include "comsvcs.h"
#include "wine/heap.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(comsvcs);

typedef struct holder
{
    IHolder IHolder_iface;
    LONG ref;

} holder;

static inline holder *impl_from_IHolder(IHolder *iface)
{
	return CONTAINING_RECORD(iface, holder, IHolder_iface);
}

static HRESULT WINAPI holder_QueryInterface(IHolder *iface, REFIID riid, void **object)
{
	holder *This = impl_from_IHolder(iface);

	TRACE("(%p)->(%s %p)\n", This, debugstr_guid(riid), object);

	*object = NULL;

	if (IsEqualGUID(riid, &IID_IUnknown) ||
		IsEqualGUID(riid, &IID_IHolder))
	{
		*object = &This->IHolder_iface;
		IUnknown_AddRef((IUnknown *)*object);

		return S_OK;
	}

	WARN("(%p)->(%s,%p),not found\n", This, debugstr_guid(riid), object);
	return E_NOINTERFACE;
}

static ULONG WINAPI holder_AddRef(IHolder *iface)
{
	holder *This = impl_from_IHolder(iface);
	ULONG ref = InterlockedIncrement(&This->ref);
	TRACE("(%p)->(%d)\n", This, ref);
	return ref;
}

static ULONG WINAPI holder_Release(IHolder *iface)
{
	holder *This = impl_from_IHolder(iface);
	ULONG ref = InterlockedDecrement(&This->ref);
	TRACE("(%p)->(%d)\n", This, ref);

	if (!ref)
	{
		heap_free(This);
	}

	return ref;
}

static inline dispensermanager *impl_from_IDispenserManager(IDispenserManager *iface)
{
	return CONTAINING_RECORD(iface, dispensermanager, IDispenserManager_iface);
}

static HRESULT WINAPI dismanager_QueryInterface(IDispenserManager *iface, REFIID riid, void **object)
{
	dispensermanager *This = impl_from_IDispenserManager(iface);

	TRACE("(%p)->(%s %p)\n", This, debugstr_guid(riid), object);

	*object = NULL;

	if (IsEqualGUID(riid, &IID_IUnknown) ||
		IsEqualGUID(riid, &IID_IDispenserManager))
	{
		*object = &This->IDispenserManager_iface;
		IUnknown_AddRef((IUnknown *)*object);

		return S_OK;
	}

	WARN("(%p)->(%s,%p),not found\n", This, debugstr_guid(riid), object);
	return E_NOINTERFACE;
}

static ULONG WINAPI dismanager_AddRef(IDispenserManager *iface)
{
	dispensermanager *This = impl_from_IDispenserManager(iface);
	ULONG ref = InterlockedIncrement(&This->ref);
	TRACE("(%p)->(%d)\n", This, ref);
	return ref;
}

static ULONG WINAPI dismanager_Release(IDispenserManager *iface)
{
	dispensermanager *This = impl_from_IDispenserManager(iface);
	ULONG ref = InterlockedDecrement(&This->ref);
	TRACE("(%p)->(%d)\n", This, ref);

	if (!ref)
	{
		heap_free(This);
	}

	return ref;
}

static HRESULT WINAPI holder_AllocResource(IHolder *iface, RESTYPID typeid, RESID *resid)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p)->(%d, %p) stub\n", This, typeid, resid);

	return E_NOTIMPL;
}

static HRESULT WINAPI holder_FreeResource(const RESID resid)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p)->(%d) stub\n", This, resid);

	return E_NOTIMPL;
}

static HRESULT WINAPI holder_TrackResource(const RESID resid)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p)->(%d) stub\n", This, resid);

	return E_NOTIMPL;
}

static HRESULT WINAPI holder_TrackResourceS(const SRESID resid)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p)->(%s) stub\n", This, resid);

	return E_NOTIMPL;
}

static HRESULT WINAPI holder_UntrackResource(const RESID resid, const BOOL value)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p)->(%d, %d) stub\n", This, resid, value);

	return E_NOTIMPL;
}

static HRESULT WINAPI holder_UntrackResourceS(const SRESID resid, const BOOL value)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p)->(%d, %d) stub\n", This, resid, value);

	return E_NOTIMPL;
}

static HRESULT WINAPI holder_Close(void)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p) stub\n", This);

	return E_NOTIMPL;
}

static HRESULT WINAPI holder_RequestDestroyResource(const RESID resid)
{
	holder *This = impl_from_IHolder(iface);

	FIXME("(%p)->(%d) stub\n", This, resid);

	return E_NOTIMPL;
}

struct IHolderVtbl holder_vtbl =
{
	holder_QueryInterface,
	holder_AddRef,
	holder_Release,
	holder_AllocResource,
	holder_FreeResource,
	holder_TrackResource,
	holder_TrackResourceS,
	holder_UntrackResource,
	holder_UntrackResourceS,
	holder_Close,
	holder_RequestDestroyResource
};

static HRESULT WINAPI holder_CreateInstance(void **object)
{
	holder *holder;
	HRESULT ret;

	holder = heap_alloc(sizeof(*holder));
	if (!holder)
	{
		*object = NULL;
		return E_OUTOFMEMORY;
	}

	holder->IHolder_iface.lpVtbl = &holder_vtbl;
	holder->ref = 1;

	ret = holder_QueryInterface(&holder->IHolder_iface, riid, object);
	holder_Release(&holder->IHolder_iface);

	return ret;
}
