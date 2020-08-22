FROM debian:10

RUN \
  echo "debconf debconf/frontend select Noninteractive" | \
    debconf-set-selections

RUN \
  echo 'APT::Install-Recommends "false";' > \
    /etc/apt/apt.conf.d/disable-install-recommends

RUN \
  apt update -qq && \
  apt install -y \
    bzip2 \
    gcc \
    libc6-dev \
    make \
    pkg-config \
    sudo \
    tzdata && \
  apt clean && \
  rm -rf /var/lib/apt/lists/*

RUN \
  useradd --user-group --create-home libgcroots

RUN \
  echo "libgcroots ALL=(ALL:ALL) NOPASSWD:ALL" | \
    EDITOR=tee visudo -f /etc/sudoers.d/libgcroots

USER libgcroots

RUN mkdir -p /home/libgcroots/build
WORKDIR /home/libgcroots/build

CMD /source/ci/distcheck.sh
