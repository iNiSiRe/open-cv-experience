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