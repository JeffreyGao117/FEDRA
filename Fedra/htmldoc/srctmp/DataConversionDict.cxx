//
// File generated by C:\root_v5.34.26\bin\rootcint.EXE at Fri Sep 04 22:31:14 2015

// Do NOT change. Changes will be lost next time file is generated
//

#define R__DICTIONARY_FILENAME DataConversionDict
#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "DataConversionDict.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"

// Direct notice to TROOT of the dictionary's loading.
namespace {
   static struct DictInit {
      DictInit() {
         ROOT::RegisterModule();
      }
   } __TheDictionaryInitializer;
}

// START OF SHADOWS

namespace ROOTShadow {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOTShadow
// END OF SHADOWS

/********************************************************
* DataConversionDict.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

extern "C" void G__cpp_reset_tagtableDataConversionDict();

extern "C" void G__set_cpp_environmentDataConversionDict() {
  G__add_ipath(".");
  G__add_ipath("..\\..\\include");
  G__cpp_reset_tagtableDataConversionDict();
}
#include <new>
extern "C" int G__cpp_dllrevDataConversionDict() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncDataConversionDict {
 public:
  G__Sizep2memfuncDataConversionDict(): p(&G__Sizep2memfuncDataConversionDict::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncDataConversionDict::*p)();
};

size_t G__get_sizep2memfuncDataConversionDict()
{
  G__Sizep2memfuncDataConversionDict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceDataConversionDict() {

   /* Setting up class inheritance */
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableDataConversionDict() {

   /* Setting up typedef entry */
   G__search_typename2("iterator<iterator_traits<string::iterator>::iterator_category,iterator_traits<string::iterator>::value_type,iterator_traits<string::iterator>::difference_type,iterator_traits<string::iterator>::pointer,iterator_traits<string::iterator>::reference>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,char>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,char,long>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,char,long,char*>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<iterator_traits<string::const_iterator>::iterator_category,iterator_traits<string::const_iterator>::value_type,iterator_traits<string::const_iterator>::difference_type,iterator_traits<string::const_iterator>::pointer,iterator_traits<string::const_iterator>::reference>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>::iterator_category,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>::value_type,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>::difference_type,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>::pointer,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>::reference>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,ROOT::TSchemaHelper,long>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,ROOT::TSchemaHelper,long,ROOT::TSchemaHelper*>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLconst_iteratorgR),0,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::const_iterator>::iterator_category,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::const_iterator>::value_type,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::const_iterator>::difference_type,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::const_iterator>::pointer,iterator_traits<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::const_iterator>::reference>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<TVirtualArray*>",117,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<TVirtualArray*>",117,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>::iterator_category,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>::value_type,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>::difference_type,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>::pointer,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>::reference>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,TVirtualArray*>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,TVirtualArray*,long>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<random_access_iterator_tag,TVirtualArray*,long,TVirtualArray**>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLconst_iteratorgR),0,G__get_linked_tagnum(&G__DataConversionDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::const_iterator>::iterator_category,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::const_iterator>::value_type,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::const_iterator>::difference_type,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::const_iterator>::pointer,iterator_traits<vector<TVirtualArray*,allocator<TVirtualArray*> >::const_iterator>::reference>",117,G__get_linked_tagnum(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */
extern "C" void G__cpp_setup_memvarDataConversionDict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/

/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncDataConversionDict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalDataConversionDict() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcDataConversionDict() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR = { "iterator<random_access_iterator_tag,char,long,char*,char&>" , 115 , -1 };
G__linked_taginfo G__DataConversionDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };
G__linked_taginfo G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR = { "iterator<random_access_iterator_tag,ROOT::TSchemaHelper,long,ROOT::TSchemaHelper*,ROOT::TSchemaHelper&>" , 115 , -1 };
G__linked_taginfo G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLconst_iteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::const_iterator>" , 99 , -1 };
G__linked_taginfo G__DataConversionDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR = { "vector<TVirtualArray*,allocator<TVirtualArray*> >" , 99 , -1 };
G__linked_taginfo G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>" , 99 , -1 };
G__linked_taginfo G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR = { "iterator<random_access_iterator_tag,TVirtualArray*,long,TVirtualArray**,TVirtualArray*&>" , 115 , -1 };
G__linked_taginfo G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLconst_iteratorgR = { "reverse_iterator<vector<TVirtualArray*,allocator<TVirtualArray*> >::const_iterator>" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableDataConversionDict() {
  G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR.tagnum = -1 ;
  G__DataConversionDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR.tagnum = -1 ;
  G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLconst_iteratorgR.tagnum = -1 ;
  G__DataConversionDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR.tagnum = -1 ;
  G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR.tagnum = -1 ;
  G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLconst_iteratorgR.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableDataConversionDict() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOcharcOlongcOcharmUcOcharaNgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOROOTcLcLTSchemaHelpercOlongcOROOTcLcLTSchemaHelpermUcOROOTcLcLTSchemaHelperaNgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLconst_iteratorgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_iteratorlErandom_access_iterator_tagcOTVirtualArraymUcOlongcOTVirtualArraymUmUcOTVirtualArraymUaNgR);
   G__get_linked_tagnum_fwd(&G__DataConversionDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLconst_iteratorgR);
}
extern "C" void G__cpp_setupDataConversionDict(void) {
  G__check_setup_version(30051515,"G__cpp_setupDataConversionDict()");
  G__set_cpp_environmentDataConversionDict();
  G__cpp_setup_tagtableDataConversionDict();

  G__cpp_setup_inheritanceDataConversionDict();

  G__cpp_setup_typetableDataConversionDict();

  G__cpp_setup_memvarDataConversionDict();

  G__cpp_setup_memfuncDataConversionDict();
  G__cpp_setup_globalDataConversionDict();
  G__cpp_setup_funcDataConversionDict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncDataConversionDict();
  return;
}
class G__cpp_setup_initDataConversionDict {
  public:
    G__cpp_setup_initDataConversionDict() { G__add_setup_func("DataConversionDict",(G__incsetup)(&G__cpp_setupDataConversionDict)); G__call_setup_funcs(); }
   ~G__cpp_setup_initDataConversionDict() { G__remove_setup_func("DataConversionDict"); }
};
G__cpp_setup_initDataConversionDict G__cpp_setup_initializerDataConversionDict;

