# Solid

Desktop hardware abstraction

## Introduction

%Solid is a device integration framework.  It provides a way of querying and
interacting with hardware independently of the underlying operating system.

It provides the following features for application developers:

- Hardware Discovery
- Power Management
- Network Management


## Usage

If you are using CMake, you need to have

    find_package(KF5Solid NO_MODULE)

(or similar) in your CMakeLists.txt file, and you need to link to KF5::Solid.

See the documentation for the Solid namespace, and the [tutorials on
TechBase][tutorials].



## Links

- Home page: <http://solid.kde.org>
- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-frameworks-devel>
- IRC channel: \#kde-devel on Freenode
- Git repository: <https://projects.kde.org/projects/frameworks/solid/repository>

There is also a [Solid song](@ref song).

[tutorials]: http://techbase.kde.org/Development/Tutorials/Solid_Tutorials
