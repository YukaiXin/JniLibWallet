# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        secLinkJni

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/seclink.c
        src/main/cpp/sec/ecdsa.c
        src/main/cpp/sec/secp256k1.c
        src/main/cpp/sec/pbkdf2.c
        src/main/cpp/sec/sing/address.c
        src/main/cpp/sec/sing/base58.c
        src/main/cpp/sec/sing/bignum.c
        src/main/cpp/sec/sing/bip32.c
        src/main/cpp/sec/sing/blake256.c
        src/main/cpp/sec/sing/blake2b.c
        src/main/cpp/sec/sing/curves.c
        src/main/cpp/sec/sing/hasher.c
        src/main/cpp/sec/sing/hmac.c
        src/main/cpp/sec/sing/memzero.c
        src/main/cpp/sec/sing/nist256p1.c
        src/main/cpp/sec/sing/rand.c
        src/main/cpp/sec/sing/ripemd160.c
        src/main/cpp/sec/sing/sha2.c
        src/main/cpp/sec/sing/sha3.c
        src/main/cpp/sec/sing/aes/aes_modes.c
        src/main/cpp/sec/sing/aes/aescrypt.c
        src/main/cpp/sec/sing/aes/aeskey.c
        src/main/cpp/sec/sing/aes/aestab.c
        src/main/cpp/sec/sing/ed25519-donna/curve25519-donna-32bit.c
        src/main/cpp/sec/sing/ed25519-donna/curve25519-donna-helpers.c
        src/main/cpp/sec/sing/ed25519-donna/curve25519-donna-scalarmult-base.c
        src/main/cpp/sec/sing/ed25519-donna/ed25519-donna-32bit-tables.c
        src/main/cpp/sec/sing/ed25519-donna/ed25519-donna-basepoint-table.c
        src/main/cpp/sec/sing/ed25519-donna/ed25519-donna-impl-base.c
        src/main/cpp/sec/sing/ed25519-donna/ed25519-keccak.c
        src/main/cpp/sec/sing/ed25519-donna/ed25519-sha3.c
        src/main/cpp/sec/sing/ed25519-donna/ed25519.c
        src/main/cpp/sec/sing/ed25519-donna/modm-donna-32bit.c
        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        secLinkJni

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})