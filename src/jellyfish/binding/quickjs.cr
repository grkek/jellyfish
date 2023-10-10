module Jellyfish
  module Binding
    @[Link(ldflags: "#{__DIR__}/../../../bin/jellyfish.a -lquickjs -lc++abi")]
    lib QuickJS
      alias JSRuntime = Void*
      alias JSContext = Void*

      enum Tag : Int64
        # all tags with a reference count are negative
        FIRST             = -11 # first negative tag
        BIG_DECIMAL       = -11
        BIG_INT           = -10
        BIG_FLOAT         =  -9
        SYMBOL            =  -8
        STRING            =  -7
        MODULE            =  -3 # used internally
        FUNCTION_BYTECODE =  -2 # used internally
        OBJECT            =  -1

        INT           = 0
        BOOL          = 1
        NULL          = 2
        UNDEFINED     = 3
        UNINITIALIZED = 4
        CATCH_OFFSET  = 5
        EXCEPTION     = 6
        FLOAT64       = 7
        # any larger tag is FLOAT64 if JS_NAN_BOXING
      end

      struct CrystalProcedure
        pointer : Void*
        context : Void*
      end

      struct CrystalString
        pointer : LibC::Char*
        size : LibC::Int
      end

      struct JSValue
        value_union : ValueUnion
        tag : Tag
      end

      union ValueUnion
        int32 : Int32
        float32 : Float32
        pointer : Void*
      end

      fun create_runtime = createRuntime : JSRuntime
      fun create_context = createContext(runtime : JSRuntime) : JSContext

      fun evaluate_source_code =
        evaluateSourceCode(context : JSContext, source_code : LibC::Char*, evaluation_flags : LibC::Int) : JSValue

      fun register_global_function =
        registerGlobalFunction(
        context : JSContext,
        crystal_procedure : CrystalProcedure,
        function_name : LibC::Char*,
        argument_size : LibC::Int
      ) : JSValue

      # Convert JSValue to native values
      fun value_to_string = valueToString(context : JSContext, value : JSValue) : LibC::Char*
      fun value_to_boolean = valueToBoolean(context : JSContext, value : JSValue) : Bool
      fun value_to_float64 = valueToFloat64(context : JSContext, value : JSValue) : Float64
      fun value_to_int32 = valueToInt32(context : JSContext, value : JSValue) : Int32
      fun value_to_int64 = valueToInt64(context : JSContext, value : JSValue) : Int64

      # Convert native values to JSValue
      fun string_to_value = stringToJsValue(context : JSContext, str : LibC::Char*) : JSValue
      fun boolean_to_value = booleanToJsValue(context : JSContext, val : Bool) : JSValue
      fun float64_to_value = float64ToJsValue(context : JSContext, d : Float64) : JSValue
      fun int32_to_value = int32ToJsValue(context : JSContext, val : Int32) : JSValue
      fun int64_to_value = int64ToJsValue(context : JSContext, val : Int64) : JSValue
      fun undefined_value = undefinedJsValue : JSValue

      # Free allocated memory
      fun free_runtime = freeRuntime(runtime : JSRuntime) : Void
      fun free_context = freeContext(context : JSContext) : Void
      fun free_value = freeValue(context : JSContext, value : JSValue) : Void
    end
  end
end
