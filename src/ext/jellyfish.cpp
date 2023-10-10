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

extern "C" JSValue registerGlobalFunction(JSContext *context, CrystalProcedure<JSValue, JSContext *, JSValue, int, JSValue *> crystalProcedure, const char *functionName, int argumentSize)
{
  JSValue globalObject = JS_GetGlobalObject(context);

  auto wrapperFunction = fnptr<JSCFunction>([crystalProcedure](JSContext *context, JSValue thisValue, int argumentCount, JSValue *argumentVector)
                                            { return crystalProcedure(context, thisValue, argumentCount, argumentVector); });

  auto function = JS_NewCFunction(context, wrapperFunction, functionName, 0);

  JS_SetPropertyStr(context, globalObject, functionName, function);
  JS_FreeValue(context, globalObject);

  return function;
}

JSValue getException(JSContext *context)
{
  return JS_GetException(context);
}

/*
  Convert JSValue to native values
*/

const char *valueToString(JSContext *context, JSValue value)
{
  return JS_ToCString(context, value);
}

bool valueToBoolean(JSContext *context, JSValue value)
{
  return (bool)JS_ToBool(context, value);
}

double valueToFloat64(JSContext *context, JSValue value)
{
  double result;

  JS_ToFloat64(context, &result, value);

  return result;
}

int valueToInt32(JSContext *context, JSValue value)
{
  int result;

  JS_ToInt32(context, &result, value);

  return result;
}

int64_t valueToInt64(JSContext *context, JSValue value)
{
  int64_t result;

  JS_ToInt64(context, &result, value);

  return result;
}

/*
  Convert native values to JSValue
*/

JSValue stringToJsValue(JSContext *context, const char *str)
{
  return JS_NewString(context, str);
}

JSValue booleanToJsValue(JSContext *context, int val)
{
  return JS_NewBool(context, val);
}

JSValue float64ToJsValue(JSContext *context, double d)
{
  return JS_NewFloat64(context, d);
}

JSValue int32ToJsValue(JSContext *context, int val)
{
  return JS_NewInt32(context, val);
}

JSValue int64ToJsValue(JSContext *context, int64_t val)
{
  return JS_NewInt64(context, val);
}

/*
  Free allocated memory
*/

void freeRuntime(JSRuntime *runtime)
{
  return JS_FreeRuntime(runtime);
}

void freeContext(JSContext *context)
{
  return JS_FreeContext(context);
}

void freeValue(JSContext *context, JSValue value)
{
  return JS_FreeValue(context, value);
}