from os import path
from conans.model import Generator
from traceback import format_exc
from logging import error
from conans import ConanFile, tools

required_conan_version = ">=1.40.3"


class ConanPackagesCmake(Generator):

    @property
    def content(self):
        try:
            ignore_version_package_names = []
            cmake_find_by_module_package_names = [
                'glm',
                'vulkan',
                'glfw'
            ]

            content = ''

            content += 'set(' + self.conanfile.name + '_CONAN_PACKAGE_NAMES\n'
            for dep_name, dep_cpp_info in self.deps_build_info.dependencies:
                content += '    "' + dep_name + '" \n'
            content += ')\n'

            content += 'set(' + self.conanfile.name + '_CMAKE_PACKAGE_NAMES\n'
            for dep_name, dep_cpp_info in self.deps_build_info.dependencies:
                content += '    "' + dep_cpp_info.get_name('cmake_find_package') + '" \n'
            content += ')\n'

            content += 'set(' + self.conanfile.name + '_CMAKE_PACKAGE_VERSIONS\n'
            for dep_name, dep_cpp_info in self.deps_build_info.dependencies:
                if dep_name in ignore_version_package_names:
                    content += '    "' + '<ignore>' + '" \n'
                else:
                    content += '    "' + dep_cpp_info.version + '" \n'
            content += ')\n'

            content += 'set(' + self.conanfile.name + '_CMAKE_PACKAGE_FIND_TYPES\n'
            for dep_name, dep_cpp_info in self.deps_build_info.dependencies:
                if dep_name in cmake_find_by_module_package_names:
                    content += '    "' + 'MODULE' + '" \n'
                else:
                    content += '    "' + 'CONFIG' + '" \n'
            content += ')\n'

            content += 'set(' + self.conanfile.name + '_CMAKE_PACKAGE_PATHS\n'
            for dep_name, dep_cpp_info in self.deps_build_info.dependencies:
                content += '    "' + self.convert_to_cmake_build_paths(dep_cpp_info.build_paths) + '" \n'
            content += ')\n'

            return content
        except Exception as e:
            error(format_exc())
            raise e

    @property
    def filename(self):
        return "conan-packages.cmake"

    def convert_to_cmake_build_paths(self, dep_cpp_info_build_paths):
        build_paths = []
        for build_path in dep_cpp_info_build_paths:
            cmake_build_path = build_path.replace('\\', '/')
            if cmake_build_path.endswith('/'):
                cmake_build_path = cmake_build_path[:-1]
            build_paths.append(cmake_build_path)
        return '<sep>'.join(build_paths)


class ConanConfiguration(ConanFile):
    requires = [
        "glm/0.9.9.8",
        "vulkan/1.2.182.0",
        "glfw/3.3.4"
    ]
    settings = "arch", "os", "compiler", "build_type"
    options = {"type": ["interface", "static", "shared"]}
    generators = "ConanPackagesCmake", "cmake_find_package"

    def set_name(self):
        self.name = path.basename(path.dirname(path.abspath(__file__)))

    def set_version(self):
        self.version = tools.load(path.join(path.dirname(path.dirname(path.abspath(__file__))), "version.txt")).strip()

    def imports(self):
        self.copy(pattern="*.dll", dst="bin", src="bin")
        self.copy(pattern="*.json", dst="bin", src="bin")
        self.copy(pattern="*.dylib", dst="lib", src="lib")

    def package_info(self):
        if self.options.type == "interface":
            self.cpp_info.libs = []
        else:
            self.cpp_info.libs = tools.collect_libs(self)


if __name__ == "__main__":
    pass
