
include (platform.pri)

INCLUDEPATH += $${PWD}/../include
VPATH += $${PWD}/../include

# Abstract interface header file
Interface_Dir = interface

Interface_Header = \
    $${Interface_Dir}/iio.h \
    $${Interface_Dir}/isocket.h\
    $${Interface_Dir}/ievent.h \
    $${Interface_Dir}/ircontext.h \
    $${Interface_Dir}/inative.h \
    $${Interface_Dir}/icursor.h \
    $${Interface_Dir}/ipackage.h
# interface end.

# libky header file
LibKY_Dir = .
LibKY_Maths_Dir = $${LibKY_Dir}/maths
LibKY_Maths_Header = \
    $${LibKY_Maths_Dir}/ky_maths.h \
    $${LibKY_Maths_Dir}/ky_vec4.h \
    $${LibKY_Maths_Dir}/ky_vec3.h \
    $${LibKY_Maths_Dir}/ky_vec2.h \
    $${LibKY_Maths_Dir}/ky_mat4X4.h \
    $${LibKY_Maths_Dir}/ky_mat3x3.h \
    $${LibKY_Maths_Dir}/ky_mat2x2.h \
    $${LibKY_Maths_Dir}/ky_quaternion.h \
    $${LibKY_Maths_Dir}/ky_plane.h \
    $${LibKY_Maths_Dir}/ky_matMxN.h \
    $${LibKY_Maths_Dir}/ky_line3.h \
    $${LibKY_Maths_Dir}/ky_line2.h \
    $${LibKY_Maths_Dir}/ky_aabb.h \
    $${LibKY_Maths_Dir}/ky_quadtree.h

LibKY_Tools_Dir = $${LibKY_Dir}/tools
LibKY_Tools_Header = \
    $${LibKY_Tools_Dir}/ky_algorlthm.h \
    $${LibKY_Tools_Dir}/ky_algorlthm.inl \
    $${LibKY_Tools_Dir}/ky_vector.h \
    $${LibKY_Tools_Dir}/ky_uuid.h \
    $${LibKY_Tools_Dir}/ky_url.h \
    $${LibKY_Tools_Dir}/ky_typeinfo.h \
    $${LibKY_Tools_Dir}/ky_timer.h \
    $${LibKY_Tools_Dir}/ky_string.h \
    $${LibKY_Tools_Dir}/ky_stream.h \
    $${LibKY_Tools_Dir}/ky_stack.h \
    $${LibKY_Tools_Dir}/ky_queue.h \
    $${LibKY_Tools_Dir}/ky_parser.h \
    $${LibKY_Tools_Dir}/ky_memory.h \
    $${LibKY_Tools_Dir}/ky_md5.h \
    $${LibKY_Tools_Dir}/ky_mapdata.h \
    $${LibKY_Tools_Dir}/ky_map.h \
    $${LibKY_Tools_Dir}/ky_hash_map.h \
    $${LibKY_Tools_Dir}/ky_list.h \
    $${LibKY_Tools_Dir}/ky_linked.h \
    $${LibKY_Tools_Dir}/ky_color.h \
    $${LibKY_Tools_Dir}/ky_image.h \
    $${LibKY_Tools_Dir}/ky_datetime.h \
    $${LibKY_Tools_Dir}/ky_array.h \
    $${LibKY_Tools_Dir}/ky_vector.inl \
    $${LibKY_Tools_Dir}/ky_stream.inl \
    $${LibKY_Tools_Dir}/ky_map.inl \
    $${LibKY_Tools_Dir}/ky_list.inl \
    $${LibKY_Tools_Dir}/ky_linked.inl \
    $${LibKY_Tools_Dir}/ky_array.inl \
    $${LibKY_Tools_Dir}/ky_hash_map.inl \
    $${LibKY_Tools_Dir}/ky_bitset.h \
    $${LibKY_Tools_Dir}/ky_bitset.inl \
    $${LibKY_Tools_Dir}/ky_signal.inl \
    $${LibKY_Tools_Dir}/ky_regex.h  \
    $${LibKY_Tools_Dir}/ky_variant.h

LibKY_Network_Dir = $${LibKY_Dir}/network
LibKY_Network_Header = \
    $${LibKY_Network_Dir}/ky_netaddr.h \
    $${LibKY_Network_Dir}/ky_socket.h \
    $${LibKY_Network_Dir}/ky_server.h \
    $${LibKY_Network_Dir}/ky_stun.h \
    $${LibKY_Network_Dir}/ky_turn.h \
    $${LibKY_Network_Dir}/ky_http.h \
    $${LibKY_Network_Dir}/ky_https.h \

LibKY_Header = \
    $${LibKY_Maths_Header} \
    $${LibKY_Tools_Header} \
    $${LibKY_Network_Header} \
    $${LibKY_Dir}/ky_macro.h\
    $${LibKY_Dir}/ky_main.h \
    $${LibKY_Dir}/ky_intrin.h \
    $${LibKY_Dir}/ky_type.h \
    $${LibKY_Dir}/ky_type.inl \
    $${LibKY_Dir}/ky_binary.h \
    $${LibKY_Dir}/ky_define.h \
    $${LibKY_Dir}/ky_barrier.h \
    $${LibKY_Dir}/ky_atomic.h \
    $${LibKY_Dir}/ky_atomic.inl \
    $${LibKY_Dir}/ky_thread.h \
    $${LibKY_Dir}/ky_debug.h \
    $${LibKY_Dir}/ky_ptr.h \
    $${LibKY_Dir}/ky_utils.h \
    $${LibKY_Dir}/ky_fsys.h \
    $${LibKY_Dir}/ky_event.h \
    $${LibKY_Dir}/ky_object.h \
    $${LibKY_Dir}/ky_application.h \
    $${LibKY_Dir}/ky_cpu.h \
    $${LibKY_Dir}/ky_poll.h

HEADERS += \
    $${Interface_Header} \
    $${LibKY_Header}

LIBS += -L$${PWD}/build/$${BuildLevel}/$${Platform} -lky -lpthread

#CONFIG += C++14
QMAKE_CXXFLAGS += -std=c++14 -Wno-ambiguous-ellipsis -Wno-unknown-pragmas
QMAKE_CFLAGS += -std=c11 -Wno-ambiguous-ellipsis -Wno-unknown-pragmas
