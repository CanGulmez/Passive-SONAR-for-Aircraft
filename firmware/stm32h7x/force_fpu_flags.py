Import("env")

fpu_flags = [
   "-mcpu=cortex-m4",
   "-mthumb",
   "-mfpu=fpv4-sp-d16",
   "-mfloat-abi=hard"
]

env.Replace(
    ASFLAGS=fpu_flags
)

env.Append(
    CCFLAGS=fpu_flags,
    LINKFLAGS=fpu_flags
)