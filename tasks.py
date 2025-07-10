
from invoke import task, Collection
import shutil

@task
def lint(c):
    """Lint the source using clang-format"""
    # Check if clang-format is installed
    if not shutil.which("clang-format"):
        print("Error: clang-format is not installed or not in PATH")
        return
    # Recursively lint include/shox directory
    c.run("find include/shox -name '*.h' -o -name '*.hpp' -o -name '*.cpp' -o -name '*.cc' | xargs clang-format -i")
    print("Linting completed")

@task
def build_and_package(c):
    """Build, install and package the project"""
    c.run("cmake -G 'Ninja Multi-Config' -S . -B build/manual -DSHOX_ENABLE_TESTING=OFF")
    c.run("cmake --build build/manual --config Release -j 8")
    with c.cd("build/manual"):
        c.run("cpack")
        c.run("mkdir -p ../../out")
        c.run("cp shox-* ../../out/")

@task
def test(c):
    """Run the tests"""
    c.run("cmake -G 'Ninja Multi-Config' -S . -B build/manual -DSHOX_ENABLE_TESTING=ON")
    c.run("cmake --build build/manual --config Debug -j 8")
    with c.cd("build/manual/tests"):
        c.run("ctest --output-on-failure")