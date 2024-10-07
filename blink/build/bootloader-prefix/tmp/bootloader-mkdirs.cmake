# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/grrr/esp/esp-idf/components/bootloader/subproject"
  "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader"
  "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader-prefix"
  "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader-prefix/tmp"
  "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader-prefix/src/bootloader-stamp"
  "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader-prefix/src"
  "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/grrr/Documentos/git/hard_firm_IoT/blink/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
