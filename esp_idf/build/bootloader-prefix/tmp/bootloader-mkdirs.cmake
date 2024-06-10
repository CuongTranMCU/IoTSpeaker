# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v5.1.2/components/bootloader/subproject"
  "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader"
  "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader-prefix"
  "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader-prefix/tmp"
  "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader-prefix/src/bootloader-stamp"
  "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader-prefix/src"
  "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/NAM3/KI 2/DESIGN_WIRELESS_EMBEDDED_SYSTEM/PRACTICE/IOT_SPEAKER/IoT_Speaker/esp_idf/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
