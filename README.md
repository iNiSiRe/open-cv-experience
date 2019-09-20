git-core,libass-dev,libfreetype6-dev,libsdl2-dev,libtool,libva-dev,libvdpau-dev,libvorbis-dev,libxcb1-dev,libxcb-shm0-dev,libxcb-xfixes0-dev,pkg-config,texinfo,wget,zlib1g-dev,yasm,libx264-dev,libx265-dev,libnuma-dev,libvpx-dev,libfdk-aac-dev,libmp3lame-dev,libopus-dev

libavfilter,libavformat,libavcodec,libswscale,libswresample,libavutil,ffprope,ffmpeg

./configure \
          --extra-libs="-lpthread -lm" \
          --enable-gpl \
          --enable-libass \
          --enable-libfdk-aac \
          --enable-libfreetype \
          --enable-libmp3lame \
          --enable-libopus \
          --enable-libvpx \
          --enable-libx264 \
          --enable-libx265 \
          --enable-libvorbis \
          --enable-nonfree \


ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream -g 52 \
-an -vcodec libx264 -vb 448k \
-f mp4 -movflags frag_keyframe+empty_moov+dash \
output_1.mp4

-c:v copy -f mp4 -movflags +dash

ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream  -g 52 -an -vcodec libx264 -vb 448k -f mp4 -movflags frag_keyframe+empty_moov out.mp4

ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -an -c:v copy -b:v 2000k -f dash -window_size 10 -extra_window_size 0 -seg_duration 1 -remove_at_exit 1 manifest.mpd

ffmpeg -re -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -map 0 -an -c:v libx264 \
-b:v:0 800k -b:v:1 300k -s:v:1 320x170 -profile:v:1 baseline \
-profile:v:0 main -bf 1 -keyint_min 120 -g 120 -sc_threshold 0 \
-b_strategy 0 -use_timeline 1 -use_template 1 \
-window_size 5 -adaptation_sets "id=0,streams=v" \
-remove_at_exit 1 -f dash manifest.mpd


rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream

ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
-c:v libvpx-vp9 -keyint_min 150 \
-g 150 -tile-columns 4 -frame-parallel 1 -f webm \
-an -vf scale=1280:720 -b:v 1500k -dash 1 out.webm

ffmpeg \
  -f webm_dash_manifest -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
  -c copy \
  -map 0 \
  -f webm_dash_manifest \
  -adaptation_sets "id=0,streams=0" \
  manifest.mpd

  ffmpeg -i out.webm -f webm_dash_manifest -c copy -map 0 -adaptation_sets "id=0,streams=0" manifest.mpd

    ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream -c:v libvpx-vp9 -keyint_min 150 \
    -g 150 -tile-columns 4 -frame-parallel 1  -f webm -dash 1 \
    -an -vf scale=160:90 -b:v 250k -dash 1 video_160x90_250k.webm \
    -an -vf scale=320:180 -b:v 500k -dash 1 video_320x180_500k.webm \
    -an -vf scale=640:360 -b:v 750k -dash 1 video_640x360_750k.webm \
    -an -vf scale=640:360 -b:v 1000k -dash 1 video_640x360_1000k.webm \
    -an -vf scale=1280:720 -b:v 1500k -dash 1 video_1280x720_1500k.webm

    ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
    -c:v libtheora -keyint_min 150 \
    -g 150 -tile-columns 4 -frame-parallel 1 -f ogg \
    -an -vf scale=1280:720 -b:v 1500k -dash 1 out.ogg

    ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
     -g 52 -vcodec libvpx -an out.webm




     ffmpeg -y -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
      -map 0:1 -pix_fmt yuv420p \
         -codec libvpx-vp9 \
         -keyint_min 60 -g 60 -speed 6 -tile-columns 4 -frame-parallel 1 \
     -threads 8 -static-thresh 0 -max-intra-rate 300 -deadline realtime \
     -lag-in-frames 0 -error-resilient 1 \
         -f webm_chunk \
         -header test.hdr \
         -chunk_start_index 1 test_%d.chk


         ffmpeg \
                 -f webm_dash_manifest -live 1 \
                 -i test.hdr \
                 -codec libvpx-vp9  \
                 -map 0 \
                 -f webm_dash_manifest -live 1 \
                 -adaptation_sets "id=0,streams=0" \
                 -chunk_start_index 1 \
                 -chunk_duration_ms 2000 \
                 -time_shift_buffer_depth 7200 \
                 -minimum_update_period 7200 \
                 test.mpd



ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
-g 52 -an -vcodec libx264 -vb 448k -f mp4 -movflags frag_keyframe+empty_moov+dash+faststart video.mp4



ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
-g 52 -an -vcodec libx264 \
-movflags frag_keyframe+empty_moov+dash \
-f webm_dash_manifest -live 1 \
-map 0:0 \
-adaptation_sets "id=0,streams=0" \
-chunk_start_index 1 \
-chunk_duration_ms 2000 \
test.mpd


ffmpeg \
  -f webm_dash_manifest -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
  -c copy \
  -map 0 \
  -f webm_dash_manifest \
  -adaptation_sets "id=0,streams=0" \
  manifest.mpd


ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -map 0 -an -c:v copy \
 -f dash -dash_segment_type mp4 -adaptation_sets "id=0,streams=0" manifest.mpd

 ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -map 0:0 -an -c:v libx264 \
 -keyint_min 120 -g 120 \
 -use_timeline 0 -use_template 0 \
 -window_size 5 -adaptation_sets "id=0,streams=v" \
 -f dash -streaming 1 -single_file 1 -seg_duration 3 manifest.mpd


 ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -an -vcodec copy -f mp4 -movflags frag_keyframe+empty_moov video2.mp4


 ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
        -c:v libvpx-vp9 -an \
        -s 1024x780 \
        -f webm video.webm


ffmpeg -re -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -c:v copy -flags +cgop -g 30 -hls_time 1 stream.m3u8

ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -map 0 -an -vcodec copy \
 -f segment -segment_format mp4 -segment_format_options movflags=frag_keyframe+empty_moov -segment_time 60 -segment_list list.flat %03d.mp4

ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
 -map 0 -an -vcodec copy \
 -f segment -segment_format mp4 -segment_format_options movflags=frag_keyframe+empty_moov+omit_tfhd_offset+default_base_moof \
 -segment_list list.flat -segment_atclocktime 1 -segment_time 60 %03d.mp4

 ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream \
  -map 0 -an -vcodec copy \
  -f segment -segment_format mp4 -segment_list list.flat -segment_atclocktime 1 -segment_time 60 %03d.mp4

  # Working
  ffmpeg -rtsp_transport tcp -i rtsp://192.168.31.246:554/user=admin_password=tlJwpbo6_channel=1_stream=0.sdp?real_stream   -map 0 -an -vcodec copy   -f segment -segment_format mp4 -segment_list list.flat -segment_atclocktime 1 -segment_time 60 -segment_format_options movflags=frag_keyframe+empty_moov+dash %03d.mp4