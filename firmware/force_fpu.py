# force_fpu.py

Import("env")

fpu_flags = [
   "-mcpu=cortex-m7",
   "-mthumb",
   "-mfpu=fpv5-sp-d16",
   "-mfloat-abi=hard"
]

env.Replace(
    ASFLAGS=fpu_flags
)

env.Append(
    CCFLAGS=fpu_flags,
    LINKFLAGS=fpu_flags
)
