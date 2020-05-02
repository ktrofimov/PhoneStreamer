###
#BASIC
###
QT       += core gui widgets quick multimedia sql
TARGET = PhoneStreamer
TEMPLATE = app
TRANSLATIONS = languages/ps_ru.ts
###
#CONFIG
###
win32 {
CONFIG += c++11
}

###
#DEFINES
###
win32 {
DEFINES += PJ_NATIVE_STRING_IS_UNICODE=1 \
           PJ_WIN32=1 \
           PJ_IS_LITTLE_ENDIAN=1 \
           PJ_IS_BIG_ENDIAN=0 \
           PJ_HAS_WINSOCK2_H \
           PJ_HAS_WS2TCPIP_H

DEFINES -= UNICODE

LIBS += -lwinmm \
        -lole32 \
        -lws2_32 \
        -lwsock32 \
        -lgdi32
}

RC_FILE = icon.rc

INCLUDEPATH += pjsip/ \
    pjsip/pjlib/include \
    pjsip/pjlib-util/include \
    pjsip/pjsip/include \
    pjsip/pjmedia/include \
    pjsip/pjnath/include \
    pjsip/third_party/gsm/inc \
    pjsip/third_party/resample/include \
    pjsip/third_party/speex/include \
    pjsip/third_party/srtp/include \
    pjsip/third_party/srtp/crypto/include \
    pjsip/third_party/build/srtp \


HEADERS  += \
    MyAccount.h \
    MyCall.h \
    mainwindow.h \
    VoIP.h \
    ui_mainwindow.h \
    runguard.h

###
#SOURCE FILES
###
SOURCES += main.cpp\
        mainwindow.cpp \
    MyAccount.cpp \
    MyCall.cpp \
    VoIP.cpp \
    pjsip/third_party/BaseClasses/amfilter.cpp \
    pjsip/third_party/BaseClasses/amvideo.cpp \
    pjsip/third_party/BaseClasses/arithutil.cpp \
    pjsip/third_party/BaseClasses/combase.cpp \
    pjsip/third_party/BaseClasses/ctlutil.cpp \
    pjsip/third_party/BaseClasses/mtype.cpp \
    pjsip/third_party/BaseClasses/renbase.cpp \
    pjsip/third_party/BaseClasses/wxdebug.cpp \
    pjsip/third_party/BaseClasses/wxlist.cpp \
    pjsip/third_party/BaseClasses/wxutil.cpp \
    pjsip/third_party/g7221/common/basicop.c \
    pjsip/third_party/g7221/common/common.c \
    pjsip/third_party/g7221/common/huff_tab.c \
    pjsip/third_party/g7221/common/tables.c \
    pjsip/third_party/g7221/decode/coef2sam.c \
    pjsip/third_party/g7221/decode/dct4_s.c \
    pjsip/third_party/g7221/decode/decoder.c \
    pjsip/third_party/g7221/encode/dct4_a.c \
    pjsip/third_party/g7221/encode/encoder.c \
    pjsip/third_party/g7221/encode/sam2coef.c \
    pjsip/third_party/gsm/src/add.c \
    pjsip/third_party/gsm/src/code.c \
    pjsip/third_party/gsm/src/debug.c \
    pjsip/third_party/gsm/src/decode.c \
    pjsip/third_party/gsm/src/gsm_create.c \
    pjsip/third_party/gsm/src/gsm_decode.c \
    pjsip/third_party/gsm/src/gsm_destroy.c \
    pjsip/third_party/gsm/src/gsm_encode.c \
    pjsip/third_party/gsm/src/gsm_explode.c \
    pjsip/third_party/gsm/src/gsm_implode.c \
    pjsip/third_party/gsm/src/gsm_option.c \
    pjsip/third_party/gsm/src/gsm_print.c \
    pjsip/third_party/gsm/src/long_term.c \
    pjsip/third_party/gsm/src/gsm_lpc.c \
    pjsip/third_party/gsm/src/gsm_preprocess.c \
    pjsip/third_party/gsm/src/rpe.c \
    pjsip/third_party/gsm/src/short_term.c \
    pjsip/third_party/gsm/src/table.c \
    pjsip/third_party/ilbc/anaFilter.c \
    pjsip/third_party/ilbc/constants.c \
    pjsip/third_party/ilbc/createCB.c \
    pjsip/third_party/ilbc/doCPLC.c \
    pjsip/third_party/ilbc/enhancer.c \
    pjsip/third_party/ilbc/filter.c \
    pjsip/third_party/ilbc/FrameClassify.c \
    pjsip/third_party/ilbc/gainquant.c \
    pjsip/third_party/ilbc/getCBvec.c \
    pjsip/third_party/ilbc/helpfun.c \
    pjsip/third_party/ilbc/hpInput.c \
    pjsip/third_party/ilbc/hpOutput.c \
    pjsip/third_party/ilbc/iCBConstruct.c \
    pjsip/third_party/ilbc/iCBSearch.c \
    pjsip/third_party/ilbc/iLBC_decode.c \
    pjsip/third_party/ilbc/iLBC_encode.c \
    pjsip/third_party/ilbc/LPCdecode.c \
    pjsip/third_party/ilbc/LPCencode.c \
    pjsip/third_party/ilbc/lsf.c \
    pjsip/third_party/ilbc/packing.c \
    pjsip/third_party/ilbc/StateConstructW.c \
    pjsip/third_party/ilbc/StateSearchW.c \
    pjsip/third_party/ilbc/syntFilter.c \
    pjsip/third_party/milenage/milenage.c \
    pjsip/third_party/milenage/rijndael.c \
    pjsip/third_party/resample/src/resamplesubs.c \
    pjsip/third_party/speex/libspeex/bits.c \
    pjsip/third_party/speex/libspeex/cb_search.c \
    pjsip/third_party/speex/libspeex/exc_5_64_table.c \
    pjsip/third_party/speex/libspeex/exc_5_256_table.c \
    pjsip/third_party/speex/libspeex/exc_8_128_table.c \
    pjsip/third_party/speex/libspeex/exc_10_16_table.c \
    pjsip/third_party/speex/libspeex/exc_10_32_table.c \
    pjsip/third_party/speex/libspeex/exc_20_32_table.c \
    pjsip/third_party/speex/libspeex/fftwrap.c \
    pjsip/third_party/speex/libspeex/filterbank.c \
    pjsip/third_party/speex/libspeex/filters.c \
    pjsip/third_party/speex/libspeex/gain_table.c \
    pjsip/third_party/speex/libspeex/gain_table_lbr.c \
    pjsip/third_party/speex/libspeex/hexc_10_32_table.c \
    pjsip/third_party/speex/libspeex/hexc_table.c \
    pjsip/third_party/speex/libspeex/high_lsp_tables.c \
    pjsip/third_party/speex/libspeex/kiss_fft.c \
    pjsip/third_party/speex/libspeex/kiss_fftr.c \
    pjsip/third_party/speex/libspeex/lpc.c \
    pjsip/third_party/speex/libspeex/lsp.c \
    pjsip/third_party/speex/libspeex/lsp_tables_nb.c \
    pjsip/third_party/speex/libspeex/ltp.c \
    pjsip/third_party/speex/libspeex/mdf.c \
    pjsip/third_party/speex/libspeex/modes.c \
    pjsip/third_party/speex/libspeex/modes_wb.c \
    pjsip/third_party/speex/libspeex/nb_celp.c \
    pjsip/third_party/speex/libspeex/preprocess.c \
    pjsip/third_party/speex/libspeex/quant_lsp.c \
    pjsip/third_party/speex/libspeex/resample.c \
    pjsip/third_party/speex/libspeex/sb_celp.c \
    pjsip/third_party/speex/libspeex/smallft.c \
    pjsip/third_party/speex/libspeex/speex.c \
    pjsip/third_party/speex/libspeex/speex_callbacks.c \
    pjsip/third_party/speex/libspeex/stereo.c \
    pjsip/third_party/speex/libspeex/vbr.c \
    pjsip/third_party/speex/libspeex/vq.c \
    pjsip/third_party/speex/libspeex/window.c \
    pjsip/third_party/srtp/crypto/cipher/aes.c \
    pjsip/third_party/srtp/crypto/cipher/aes_cbc.c \
    pjsip/third_party/srtp/crypto/cipher/aes_icm.c \
    pjsip/third_party/srtp/crypto/cipher/cipher.c \
    pjsip/third_party/srtp/crypto/cipher/null_cipher.c \
    pjsip/third_party/srtp/crypto/hash/auth.c \
    pjsip/third_party/srtp/crypto/hash/hmac.c \
    pjsip/third_party/srtp/crypto/hash/null_auth.c \
    pjsip/third_party/srtp/crypto/hash/ch_sha1.c \
    pjsip/third_party/srtp/crypto/kernel/alloc.c \
    pjsip/third_party/srtp/crypto/kernel/crypto_kernel.c \
    pjsip/third_party/srtp/crypto/kernel/key.c \
    pjsip/third_party/srtp/crypto/math/datatypes.c \
    pjsip/third_party/srtp/crypto/math/gf2_8.c \
    pjsip/third_party/srtp/crypto/math/stat.c \
    pjsip/third_party/srtp/crypto/replay/rdb.c \
    pjsip/third_party/srtp/crypto/replay/rdbx.c \
    pjsip/third_party/srtp/crypto/replay/ut_sim.c \
    pjsip/third_party/srtp/crypto/rng/ctr_prng.c \
    pjsip/third_party/srtp/crypto/rng/prng.c \
    pjsip/third_party/srtp/crypto/rng/rand_source.c \
    pjsip/third_party/srtp/pjlib/srtp_err.c \
    pjsip/third_party/srtp/srtp/ekt.c \
    pjsip/third_party/srtp/srtp/srtp.c \
    pjsip/pjlib/src/pj/activesock.c \
    pjsip/pjlib/src/pj/addr_resolv_sock.c \
    pjsip/pjlib/src/pj/array.c \
    pjsip/pjlib/src/pj/config.c \
    pjsip/pjlib/src/pj/ctype.c \
    pjsip/pjlib/src/pj/pj_errno.c \
    pjsip/pjlib/src/pj/except.c \
    pjsip/pjlib/src/pj/fifobuf.c \
    pjsip/pjlib/src/pj/file_access_win32.c \
    pjsip/pjlib/src/pj/file_io_win32.c \
    pjsip/pjlib/src/pj/guid.c \
    pjsip/pjlib/src/pj/guid_win32.c \
    pjsip/pjlib/src/pj/hash.c \
    pjsip/pjlib/src/pj/ip_helper_win32.c \
    pjsip/pjlib/src/pj/list.c \
    pjsip/pjlib/src/pj/lock.c \
    pjsip/pjlib/src/pj/log.c \
    pjsip/pjlib/src/pj/log_writer_stdout.c \
    pjsip/pjlib/src/pj/os_core_win32.c \
    pjsip/pjlib/src/pj/os_error_win32.c \
    pjsip/pjlib/src/pj/os_info.c \
    pjsip/pjlib/src/pj/os_time_win32.c \
    pjsip/pjlib/src/pj/os_timestamp_win32.c \
    pjsip/pjlib/src/pj/pool.c \
    pjsip/pjlib/src/pj/pool_buf.c \
    pjsip/pjlib/src/pj/pool_caching.c \
    pjsip/pjlib/src/pj/pool_dbg.c \
    pjsip/pjlib/src/pj/pool_policy_malloc.c \
    pjsip/pjlib/src/pj/rand.c \
    pjsip/pjlib/src/pj/rbtree.c \
    pjsip/pjlib/src/pj/sock_common.c \
    pjsip/pjlib/src/pj/sock_qos_common.c \
    pjsip/pjlib/src/pj/sock_qos_wm.c \
    pjsip/pjlib/src/pj/sock_select.c \
    pjsip/pjlib/src/pj/ssl_sock_common.c \
    pjsip/pjlib/src/pj/ssl_sock_dump.c \
    pjsip/pjlib/src/pj/pj_string.c \
    pjsip/pjlib/src/pj/timer.c \
    pjsip/pjlib/src/pj/types.c \
    pjsip/pjlib/src/pj/unicode_win32.c \
    pjsip/pjlib/src/pj/ioqueue_select.c \
    pjsip/pjlib/src/pj/sock_bsd.c \
    pjsip/pjlib/src/pj/sock_qos_bsd.c \
    pjsip/pjlib/src/pj/sock_qos_dummy.c \
    pjsip/pjlib/src/pj/os_time_common.c \
    pjsip/pjlib/src/pj/os_timestamp_common.c \
    pjsip/pjlib-util/src/pjlib-util/base64.c \
    #pjsip/pjlib/src/pj/ioqueue_common_abs.c
    pjsip/pjlib-util/src/pjlib-util/cli.c \
    pjsip/pjlib-util/src/pjlib-util/cli_console.c \
    pjsip/pjlib-util/src/pjlib-util/cli_telnet.c \
    pjsip/pjlib-util/src/pjlib-util/crc32.c \
    pjsip/pjlib-util/src/pjlib-util/dns.c \
    pjsip/pjlib-util/src/pjlib-util/dns_dump.c \
    pjsip/pjlib-util/src/pjlib-util/dns_server.c \
    pjsip/pjlib-util/src/pjlib-util/errno.c \
    pjsip/pjlib-util/src/pjlib-util/getopt.c \
    pjsip/pjlib-util/src/pjlib-util/hmac_md5.c \
    pjsip/pjlib-util/src/pjlib-util/hmac_sha1.c \
    pjsip/pjlib-util/src/pjlib-util/http_client.c \
    pjsip/pjlib-util/src/pjlib-util/json.c \
    pjsip/pjlib-util/src/pjlib-util/md5.c \
    pjsip/pjlib-util/src/pjlib-util/pcap.c \
    pjsip/pjlib-util/src/pjlib-util/resolver.c \
    pjsip/pjlib-util/src/pjlib-util/scanner.c \
    pjsip/pjlib-util/src/pjlib-util/scanner_cis_bitwise.c \
    pjsip/pjlib-util/src/pjlib-util/scanner_cis_uint.c \
    pjsip/pjlib-util/src/pjlib-util/sha1.c \
    pjsip/pjlib-util/src/pjlib-util/srv_resolver.c \
    pjsip/pjlib-util/src/pjlib-util/string.c \
    pjsip/pjlib-util/src/pjlib-util/stun_simple.c \
    pjsip/pjlib-util/src/pjlib-util/stun_simple_client.c \
    pjsip/pjlib-util/src/pjlib-util/symbols.c \
    pjsip/pjlib-util/src/pjlib-util/xml.c \
    pjsip/pjmedia/src/pjmedia/alaw_ulaw.c \
    pjsip/pjmedia/src/pjmedia/alaw_ulaw_table.c \
    pjsip/pjmedia/src/pjmedia/pjm_audiodev.c \
    pjsip/pjmedia/src/pjmedia/avi_player.c \
    pjsip/pjmedia/src/pjmedia/bidirectional.c \
    pjsip/pjmedia/src/pjmedia/clock_thread.c \
    pjsip/pjmedia/src/pjmedia/codec.c \
    pjsip/pjmedia/src/pjmedia/conf_switch.c \
    pjsip/pjmedia/src/pjmedia/conference.c \
    pjsip/pjmedia/src/pjmedia/converter.c \
    pjsip/pjmedia/src/pjmedia/converter_libswscale.c \
    pjsip/pjmedia/src/pjmedia/converter_libyuv.c \
    pjsip/pjmedia/src/pjmedia/delaybuf.c \
    pjsip/pjmedia/src/pjmedia/echo_common.c \
    pjsip/pjmedia/src/pjmedia/echo_port.c \
    pjsip/pjmedia/src/pjmedia/echo_speex.c \
    pjsip/pjmedia/src/pjmedia/echo_suppress.c \
    pjsip/pjmedia/src/pjmedia/pjm_endpoint.c \
    pjsip/pjmedia/src/pjmedia/pjm_errno.c \
    pjsip/pjmedia/src/pjmedia/event.c \
    pjsip/pjmedia/src/pjmedia/ffmpeg_util.c \
    pjsip/pjmedia/src/pjmedia/format.c \
    pjsip/pjmedia/src/pjmedia/g711.c \
    pjsip/pjmedia/src/pjmedia/jbuf.c \
    pjsip/pjmedia/src/pjmedia/master_port.c \
    pjsip/pjmedia/src/pjmedia/mem_capture.c \
    pjsip/pjmedia/src/pjmedia/mem_player.c \
    pjsip/pjmedia/src/pjmedia/null_port.c \
    pjsip/pjmedia/src/pjmedia/plc_common.c \
    pjsip/pjmedia/src/pjmedia/port.c \
    pjsip/pjmedia/src/pjmedia/resample_libsamplerate.c \
    pjsip/pjmedia/src/pjmedia/resample_port.c \
    pjsip/pjmedia/src/pjmedia/resample_resample.c \
    pjsip/pjmedia/src/pjmedia/resample_speex.c \
    pjsip/pjmedia/src/pjmedia/rtcp.c \
    pjsip/pjmedia/src/pjmedia/rtcp_xr.c \
    pjsip/pjmedia/src/pjmedia/rtp.c \
    pjsip/pjmedia/src/pjmedia/sdp.c \
    pjsip/pjmedia/src/pjmedia/sdp_cmp.c \
    pjsip/pjmedia/src/pjmedia/sdp_neg.c \
    pjsip/pjmedia/src/pjmedia/silencedet.c \
    pjsip/pjmedia/src/pjmedia/sound_legacy.c \
    pjsip/pjmedia/src/pjmedia/sound_port.c \
    pjsip/pjmedia/src/pjmedia/splitcomb.c \
    pjsip/pjmedia/src/pjmedia/stereo_port.c \
    pjsip/pjmedia/src/pjmedia/stream.c \
    pjsip/pjmedia/src/pjmedia/stream_common.c \
    pjsip/pjmedia/src/pjmedia/stream_info.c \
    pjsip/pjmedia/src/pjmedia/tonegen.c \
    pjsip/pjmedia/src/pjmedia/transport_adapter_sample.c \
    pjsip/pjmedia/src/pjmedia/transport_ice.c \
    pjsip/pjmedia/src/pjmedia/transport_loop.c \
    pjsip/pjmedia/src/pjmedia/transport_srtp.c \
    pjsip/pjmedia/src/pjmedia/transport_udp.c \
    pjsip/pjmedia/src/pjmedia/pjm_types.c \
    pjsip/pjmedia/src/pjmedia/vid_codec.c \
    pjsip/pjmedia/src/pjmedia/vid_codec_util.c \
    pjsip/pjmedia/src/pjmedia/vid_port.c \
    pjsip/pjmedia/src/pjmedia/vid_stream.c \
    pjsip/pjmedia/src/pjmedia/vid_stream_info.c \
    pjsip/pjmedia/src/pjmedia/vid_tee.c \
    pjsip/pjmedia/src/pjmedia/pjm_videodev.c \
    pjsip/pjmedia/src/pjmedia/wav_player.c \
    pjsip/pjmedia/src/pjmedia/wav_playlist.c \
    pjsip/pjmedia/src/pjmedia/wav_writer.c \
    pjsip/pjmedia/src/pjmedia/wave.c \
    pjsip/pjmedia/src/pjmedia/wsola.c \
    pjsip/pjmedia/src/pjmedia-audiodev/symb_aps_dev.cpp \
    pjsip/pjmedia/src/pjmedia-audiodev/symb_mda_dev.cpp \
    pjsip/pjmedia/src/pjmedia-audiodev/symb_vas_dev.cpp \
    pjsip/pjmedia/src/pjmedia-audiodev/audiodev.c \
    pjsip/pjmedia/src/pjmedia-audiodev/audiotest.c \
    pjsip/pjmedia/src/pjmedia-audiodev/bdimad_dev.c \
    pjsip/pjmedia/src/pjmedia-audiodev/pjma_errno.c \
    pjsip/pjmedia/src/pjmedia-audiodev/legacy_dev.c \
    pjsip/pjmedia/src/pjmedia-audiodev/null_dev.c \
    pjsip/pjmedia/src/pjmedia-audiodev/pa_dev.c \
    pjsip/pjmedia/src/pjmedia-audiodev/wmme_dev.c \
    pjsip/pjmedia/src/pjmedia-codec/g722/g722_dec.c \
    pjsip/pjmedia/src/pjmedia-codec/g722/g722_enc.c \
    pjsip/pjmedia/src/pjmedia-codec/openh264.cpp \
    pjsip/pjmedia/src/pjmedia-codec/amr_sdp_match.c \
    pjsip/pjmedia/src/pjmedia-codec/audio_codecs.c \
    pjsip/pjmedia/src/pjmedia-codec/ffmpeg_vid_codecs.c \
    pjsip/pjmedia/src/pjmedia-codec/g722.c \
    pjsip/pjmedia/src/pjmedia-codec/g7221.c \
    pjsip/pjmedia/src/pjmedia-codec/g7221_sdp_match.c \
    pjsip/pjmedia/src/pjmedia-codec/gsm.c \
    pjsip/pjmedia/src/pjmedia-codec/h263_packetizer.c \
    pjsip/pjmedia/src/pjmedia-codec/h264_packetizer.c \
    pjsip/pjmedia/src/pjmedia-codec/ilbc.c \
    pjsip/pjmedia/src/pjmedia-codec/ipp_codecs.c \
    pjsip/pjmedia/src/pjmedia-codec/l16.c \
    pjsip/pjmedia/src/pjmedia-codec/opencore_amr.c \
    pjsip/pjmedia/src/pjmedia-codec/passthrough.c \
    pjsip/pjmedia/src/pjmedia-codec/silk.c \
    pjsip/pjmedia/src/pjmedia-codec/speex_codec.c \
    pjsip/pjmedia/src/pjmedia-videodev/dshowclasses.cpp \
    pjsip/pjmedia/src/pjmedia-videodev/avi_dev.c \
    pjsip/pjmedia/src/pjmedia-videodev/colorbar_dev.c \
    pjsip/pjmedia/src/pjmedia-videodev/dshow_dev.c \
    pjsip/pjmedia/src/pjmedia-videodev/pjmv_errno.c \
    pjsip/pjmedia/src/pjmedia-videodev/ffmpeg_dev.c \
    pjsip/pjmedia/src/pjmedia-videodev/sdl_dev.c \
    pjsip/pjmedia/src/pjmedia-videodev/videodev.c \
    pjsip/pjnath/src/pjnath/nath_errno.c \
    pjsip/pjnath/src/pjnath/ice_session.c \
    pjsip/pjnath/src/pjnath/ice_strans.c \
    pjsip/pjnath/src/pjnath/nat_detect.c \
    pjsip/pjnath/src/pjnath/stun_auth.c \
    pjsip/pjnath/src/pjnath/stun_msg.c \
    pjsip/pjnath/src/pjnath/stun_msg_dump.c \
    pjsip/pjnath/src/pjnath/stun_session.c \
    pjsip/pjnath/src/pjnath/stun_sock.c \
    pjsip/pjnath/src/pjnath/stun_transaction.c \
    pjsip/pjnath/src/pjnath/turn_session.c \
    pjsip/pjnath/src/pjnath/turn_sock.c \
    pjsip/pjsip/src/pjsip/sip_auth_aka.c \
    pjsip/pjsip/src/pjsip/sip_auth_client.c \
    pjsip/pjsip/src/pjsip/sip_auth_msg.c \
    pjsip/pjsip/src/pjsip/sip_auth_parser.c \
    pjsip/pjsip/src/pjsip/sip_auth_server.c \
    pjsip/pjsip/src/pjsip/sip_config.c \
    pjsip/pjsip/src/pjsip/sip_dialog.c \
    pjsip/pjsip/src/pjsip/sip_endpoint.c \
    pjsip/pjsip/src/pjsip/sip_errno.c \
    pjsip/pjsip/src/pjsip/sip_msg.c \
    pjsip/pjsip/src/pjsip/sip_multipart.c \
    pjsip/pjsip/src/pjsip/sip_parser.c \
    pjsip/pjsip/src/pjsip/sip_resolve.c \
    pjsip/pjsip/src/pjsip/sip_tel_uri.c \
    pjsip/pjsip/src/pjsip/sip_transaction.c \
    pjsip/pjsip/src/pjsip/sip_transport.c \
    pjsip/pjsip/src/pjsip/sip_transport_loop.c \
    pjsip/pjsip/src/pjsip/sip_transport_tcp.c \
    pjsip/pjsip/src/pjsip/sip_transport_tls.c \
    pjsip/pjsip/src/pjsip/sip_transport_udp.c \
    pjsip/pjsip/src/pjsip/sip_ua_layer.c \
    pjsip/pjsip/src/pjsip/sip_uri.c \
    pjsip/pjsip/src/pjsip/sip_util.c \
    pjsip/pjsip/src/pjsip/sip_util_proxy.c \
    pjsip/pjsip/src/pjsip/sip_util_statefull.c \
    pjsip/pjsip/src/pjsip-simple/pjss_errno.c \
    pjsip/pjsip/src/pjsip-simple/evsub.c \
    pjsip/pjsip/src/pjsip-simple/evsub_msg.c \
    pjsip/pjsip/src/pjsip-simple/iscomposing.c \
    pjsip/pjsip/src/pjsip-simple/mwi.c \
    pjsip/pjsip/src/pjsip-simple/pidf.c \
    pjsip/pjsip/src/pjsip-simple/presence.c \
    pjsip/pjsip/src/pjsip-simple/presence_body.c \
    pjsip/pjsip/src/pjsip-simple/publishc.c \
    pjsip/pjsip/src/pjsip-simple/rpid.c \
    pjsip/pjsip/src/pjsip-simple/xpidf.c \
    pjsip/pjsip/src/pjsip-ua/sip_100rel.c \
    pjsip/pjsip/src/pjsip-ua/sip_inv.c \
    pjsip/pjsip/src/pjsip-ua/sip_reg.c \
    pjsip/pjsip/src/pjsip-ua/sip_replaces.c \
    pjsip/pjsip/src/pjsip-ua/sip_timer.c \
    pjsip/pjsip/src/pjsip-ua/sip_xfer.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_acc.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_aud.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_call.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_core.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_dump.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_im.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_media.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_pres.c \
    pjsip/pjsip/src/pjsua-lib/pjsua_vid.c \
    pjsip/pjsip/src/pjsua2/account.cpp \
    pjsip/pjsip/src/pjsua2/call.cpp \
    pjsip/pjsip/src/pjsua2/endpoint.cpp \
    pjsip/pjsip/src/pjsua2/pjsua2_json.cpp \
    pjsip/pjsip/src/pjsua2/media.cpp \
    pjsip/pjsip/src/pjsua2/persistent.cpp \
    pjsip/pjsip/src/pjsua2/pjsua2_presence.cpp \
    pjsip/pjsip/src/pjsua2/siptypes.cpp \
    pjsip/pjsip/src/pjsua2/pjsua2_types.cpp \
    runguard.cpp


FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc \
    sounds.qrc \
    style.qrc

DISTFILES += \
    icon.rc \
    languages/ps_ru.ts
