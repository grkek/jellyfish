module Jellyfish
  @[Link(ldflags: "#{__DIR__}/../../bin/jellyfish.a -lquickjs -lc++abi")]
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
      evaluateSourceCode(context : JSContext, sourceCode : LibC::Char*) : JSValue

    fun register_global_function =
      registerGlobalFunction(
      context : JSContext,
      crystalProcedure : CrystalProcedure,
      functionName : LibC::Char*,
      argumentSize : LibC::Int
    ) : JSValue
  end
end
