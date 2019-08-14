

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for BuildingGeneralizationEsri.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __BuildingGeneralizationEsri_i_h__
#define __BuildingGeneralizationEsri_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Ibuilding_simplify_FWD_DEFINED__
#define __Ibuilding_simplify_FWD_DEFINED__
typedef interface Ibuilding_simplify Ibuilding_simplify;

#endif 	/* __Ibuilding_simplify_FWD_DEFINED__ */


#ifndef __building_simplify_FWD_DEFINED__
#define __building_simplify_FWD_DEFINED__

#ifdef __cplusplus
typedef class building_simplify building_simplify;
#else
typedef struct building_simplify building_simplify;
#endif /* __cplusplus */

#endif 	/* __building_simplify_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __Ibuilding_simplify_INTERFACE_DEFINED__
#define __Ibuilding_simplify_INTERFACE_DEFINED__

/* interface Ibuilding_simplify */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_Ibuilding_simplify;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("734CAF16-E975-420F-BD46-EF01E75950A5")
    Ibuilding_simplify : public IUnknown
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct Ibuilding_simplifyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Ibuilding_simplify * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Ibuilding_simplify * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Ibuilding_simplify * This);
        
        END_INTERFACE
    } Ibuilding_simplifyVtbl;

    interface Ibuilding_simplify
    {
        CONST_VTBL struct Ibuilding_simplifyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Ibuilding_simplify_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Ibuilding_simplify_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Ibuilding_simplify_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __Ibuilding_simplify_INTERFACE_DEFINED__ */



#ifndef __BuildingGeneralizationEsriLib_LIBRARY_DEFINED__
#define __BuildingGeneralizationEsriLib_LIBRARY_DEFINED__

/* library BuildingGeneralizationEsriLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_BuildingGeneralizationEsriLib;

EXTERN_C const CLSID CLSID_building_simplify;

#ifdef __cplusplus

class DECLSPEC_UUID("0B792481-F559-4287-B34D-872A43958126")
building_simplify;
#endif
#endif /* __BuildingGeneralizationEsriLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


