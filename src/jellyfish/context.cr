module Jellyfish
  class Context
    alias QuickJS = Binding::QuickJS

    @pointer : QuickJS::JSContext

    def initialize(runtime : Runtime)
      @pointer = QuickJS.create_context(runtime.to_unsafe)
    end

    def eval(source_code : String) : Value
      Value.new(@pointer, QuickJS.evaluate_source_code(@pointer, source_code, 0))
    end

    def register_global_function(function_name : String, procedure : Proc(QuickJS::JSContext, QuickJS::JSValue, LibC::Int, QuickJS::JSValue*, QuickJS::JSValue), argument_size : Int32)
      QuickJS.register_global_function(@pointer, QuickJS::CrystalProcedure.new(pointer: procedure.pointer, context: procedure.closure_data), function_name, argument_size)
    end

    def to_unsafe
      @pointer
    end

    def finalize
      QuickJS.free_context(@pointer)
    end
  end
end