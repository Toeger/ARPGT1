/*
** Lua binding: luaskillbindings
** Generated automatically by tolua++-1.0.93 on Tue Mar 29 23:58:37 2016.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_luaskillbindings_open (lua_State* tolua_S);

#include "luaskills.h"
#pragma GCC diagnostic ignored "-Wunused-parameter" //disable warnings preventing the generated code to compile

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: test */
#ifndef TOLUA_DISABLE_tolua_luaskillbindings_test00
static int tolua_luaskillbindings_test00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnoobj(tolua_S,1,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   test();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'test'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_luaskillbindings_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"test",tolua_luaskillbindings_test00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_luaskillbindings (lua_State* tolua_S) {
 return tolua_luaskillbindings_open(tolua_S);
};
#endif

