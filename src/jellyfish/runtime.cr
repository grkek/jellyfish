module Jellyfish
  class Runtime
    alias QuickJS = Binding::QuickJS

    @pointer : QuickJS::JSRuntime

    def initialize
      @pointer = QuickJS.create_runtime
    end

    def to_unsafe
      @pointer
    end

    def finalize
      QuickJS.free_runtime(@pointer)
    end
  end
end