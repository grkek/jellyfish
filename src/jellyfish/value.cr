require "./binding/**"

module Jellyfish
  class Value
    alias QuickJS = Binding::QuickJS

    @context : QuickJS::JSContext
    @value : QuickJS::JSValue

    def initialize(@context : QuickJS::JSContext, @value : QuickJS::JSValue)
    end

    def initialize(@context : QuickJS::JSContext, str : String)
      @value = QuickJS.string_to_value(@context, str)
    end

    def initialize(@context : QuickJS::JSContext, val : Bool)
      @value = QuickJS.boolean_to_value(@context, val)
    end

    def initialize(@context : QuickJS::JSContext, d : Float64)
      @value = QuickJS.float64_to_value(@context, d)
    end

    def initialize(@context : QuickJS::JSContext, val : Int32)
      @value = QuickJS.int32_to_value(@context, val)
    end

    def initialize(@context : QuickJS::JSContext, val : Int64)
      @value = QuickJS.int64_to_value(@context, val)
    end

    def to_s : String
      String.new(QuickJS.value_to_string(@context, @value))
    end

    def to_bool : Bool
      QuickJS.value_to_bool(@context, @value)
    end

    def to_f64 : Float64
      QuickJS.value_to_float64(@context, @value)
    end

    def to_i32 : Int32
      QuickJS.value_to_int32(@context, @value)
    end

    def to_i64 : Int64
      QuickJS.value_to_int64(@context, @value)
    end

    def to_unsafe
      @value
    end

    def finalize
      QuickJS.free_value(@context, @value)
    end
  end
end