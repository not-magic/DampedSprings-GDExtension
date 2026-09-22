#!/usr/bin/env python
import os
import sys

# You can find documentation for SCons and SConstruct files at:
# https://scons.org/documentation.html

# This lets SCons know that we're using godot-cpp, from the godot-cpp folder.
env = SConscript("godot-cpp/SConstruct")

# Configures the 'src' directory as a source for header files.
env.Append(CPPPATH=["src/"])


# Collects all .cpp files in the 'src' folder as compile targets.
sources = Glob("src/*.cpp")

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# The filename for the dynamic library for this GDExtension.
# $SHLIBPREFIX is a platform specific prefix for the dynamic library ('lib' on Unix, '' on Windows).
# $SHLIBSUFFIX is the platform specific suffix for the dynamic library (for example '.dll' on Windows).
# env["suffix"] includes the build's feature tags (e.g. '.windows.template_debug.x86_64')
# (see https://docs.godotengine.org/en/stable/tutorials/export/feature_tags.html).
# The final path should match a path in the '.gdextension' file.
lib_filename = "{}gdexample{}{}".format(env.subst('$SHLIBPREFIX'), env["suffix"], env.subst('$SHLIBSUFFIX'))

# Creates a SCons target for the path with our sources.
library = env.SharedLibrary(
    "project/bin/{}".format(lib_filename),
    source=sources,
)

# Selects the shared library as the default target.
Default(library)

# --- Unit tests (tests/) ---
# The spring integration math (src/damped_spring_math.h) is a header-only,
# engine-independent template, so its tests build and run as a plain native
# program -- no godot-cpp linking, no running Godot process required.
# Uses its own native Environment rather than the (possibly cross-compiling)
# `env` above, since the test binary needs to run on this machine.
test_env = Environment()
test_env.Append(CPPPATH=["src/"])
if test_env["CXX"] == "cl":
    test_env.Append(CXXFLAGS=["/std:c++17"])
else:
    test_env.Append(CXXFLAGS=["-std=c++17"])

test_program = test_env.Program("tests/bin/test_damped_spring", Glob("tests/*.cpp"))
run_tests = test_env.Alias("tests", test_program, test_program[0].abspath)
AlwaysBuild(run_tests)
