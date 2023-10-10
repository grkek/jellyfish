#include "jellyfish.h"
#include "quickjs.h"

JSRuntime *createRuntime()
{
  return JS_NewRuntime();
}

JSContext *createContext(JSRuntime *runtime)
{
  return JS_NewContext(runtime);
}

JSValue evaluateSourceCode(JSContext *context, const char *sourceCode, int evaluationFlags)
{
  return JS_Eval(
      context,
      sourceCode,
      strlen(sourceCode),
      "untitled.js",
      evaluationFlags);
}


extern "C" JSValue registerGlobalFunction(JSContext *context, CrystalProcedure<JSValue, JSContext*, JSValue, int, JSValue*> crystalProcedure, const char * functionName, int argumentSize)
{
  JSValue globalObject = JS_GetGlobalObject(context);

  auto wrapperFunction = fnptr<JSCFunction>([crystalProcedure](JSContext *context, JSValue thisValue, int argumentCount, JSValue* argumentVector) {
    return crystalProcedure(context, thisValue, argumentCount, argumentVector);
  });

  auto function = JS_NewCFunction(context, wrapperFunction, functionName, 0);

  JS_SetPropertyStr(context, globalObject, functionName, function);
  JS_FreeValue(context, globalObject);

  return JS_NewInt32(context, 1337);
}

JSValue getException(JSContext *context)
{
  return JS_GetException(context);
}

CrystalString valueToCrystalString(JSContext *context, JSValue value)
{
  auto string = JS_ToCString(context, value);

  CrystalString crystalString;

  crystalString.ptr = string;
  crystalString.size = strlen(string);

  return crystalString;
}