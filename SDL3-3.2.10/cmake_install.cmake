# Install script for directory: /nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/sdl3.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so.0.2.10"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/libSDL3.so.0.2.10"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/libSDL3.so.0"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so.0.2.10"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so.0"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/libSDL3.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSDL3.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/libSDL3_test.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3headersTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3headersTargets.cmake"
         "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3headersTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3headersTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3headersTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3" TYPE FILE FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3headersTargets.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3sharedTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3sharedTargets.cmake"
         "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3sharedTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3sharedTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3sharedTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3" TYPE FILE FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3sharedTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3" TYPE FILE FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3sharedTargets-relwithdebinfo.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3testTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3testTargets.cmake"
         "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3testTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3testTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3/SDL3testTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3" TYPE FILE FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3testTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3" TYPE FILE FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/CMakeFiles/Export/lib/cmake/SDL3/SDL3testTargets-relwithdebinfo.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SDL3" TYPE FILE FILES
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/SDL3Config.cmake"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/SDL3ConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL3" TYPE FILE FILES
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_assert.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_asyncio.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_atomic.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_audio.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_begin_code.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_bits.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_blendmode.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_camera.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_clipboard.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_close_code.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_copying.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_cpuinfo.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_dialog.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_egl.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_endian.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_error.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_events.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_filesystem.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_gamepad.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_gpu.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_guid.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_haptic.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_hidapi.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_hints.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_init.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_intrin.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_iostream.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_joystick.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_keyboard.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_keycode.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_loadso.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_locale.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_log.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_main.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_main_impl.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_messagebox.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_metal.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_misc.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_mouse.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_mutex.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_oldnames.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengl.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengl_glext.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengles.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengles2.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengles2_gl2.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengles2_gl2ext.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengles2_gl2platform.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_opengles2_khrplatform.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_pen.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_pixels.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_platform.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_platform_defines.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_power.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_process.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_properties.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_rect.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_render.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_scancode.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_sensor.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_stdinc.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_storage.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_surface.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_system.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_thread.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_time.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_timer.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_touch.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_tray.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_version.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_video.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_vulkan.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include-revision/SDL3/SDL_revision.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SDL3" TYPE FILE FILES
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_assert.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_common.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_compare.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_crc32.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_font.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_fuzzer.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_harness.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_log.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_md5.h"
    "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/include/SDL3/SDL_test_memory.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/licenses/SDL3" TYPE FILE FILES "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/LICENSE.txt")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/nfs/homes/rrollin/Documents/lem_ipc/SDL3-3.2.10/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
