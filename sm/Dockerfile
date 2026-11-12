# Build riproducibile del System Manager NXP (imx-sm) per i.MX94.
#
# Produce gli stessi byte dei binari committati in sm/: e' la "ricetta" per verificarli.
#   docker build -f sm/Dockerfile --output type=local,dest=build/sm-docker sm
# (o: make sm-verify)
#
# Cosa rende la build riproducibile:
#   - toolchain Arm pinnato (versione dichiarata da NXP nel tag, TC_VERSION) e verificato con sha256
#   - clone completo del tag: SM_COMMIT/SM_BUILD in build_info.h sono quelli di imx-sm
#   - percorsi fissi (/src/imx-sm, /opt/tools): finiscono nelle stringhe di assert e nell'ELF
#   - data e ora di build = data del commit del tag (wrapper di `date` + SOURCE_DATE_EPOCH)

FROM ubuntu:24.04 AS build
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential git ca-certificates curl xz-utils srecord perl \
    && rm -rf /var/lib/apt/lists/*

# Toolchain: 14.2.rel1 e' il TC_VERSION di sm/makefiles/common.mak nel tag lf-6.12.20-2.0.0.
# Lo sha256 e' quello pubblicato da Arm (<url>.sha256asc).
ARG TC_VERSION=14.2.rel1
ARG TC_SHA256=62a63b981fe391a9cbad7ef51b17e49aeaa3e7b0d029b36ca1e9c3b2a9b78823
RUN curl -fsSL -o /tmp/tc.tar.xz \
        "https://developer.arm.com/-/media/Files/downloads/gnu/${TC_VERSION}/binrel/arm-gnu-toolchain-${TC_VERSION}-x86_64-arm-none-eabi.tar.xz" \
    && echo "${TC_SHA256}  /tmp/tc.tar.xz" | sha256sum -c - \
    && mkdir -p /opt/tools && tar -xJf /tmp/tc.tar.xz -C /opt/tools && rm /tmp/tc.tar.xz

# `date` usato dal Makefile NXP per SM_DATE/SM_TIME: rispetta SOURCE_DATE_EPOCH
COPY date-wrapper.sh /usr/local/bin/date

ARG SM_REPO=https://github.com/nxp-imx/imx-sm.git
ARG SM_TAG=lf-6.12.20-2.0.0
RUN git clone -q -c advice.detachedHead=false --single-branch --branch "${SM_TAG}" "${SM_REPO}" /src/imx-sm

ARG SM_CONFIGS="mx94evk mx94alt"
RUN set -e; cd /src/imx-sm; \
    export TOOLS=/opt/tools SOURCE_DATE_EPOCH="$(git log -1 --format=%ct)" TZ=UTC; \
    mkdir -p /out; \
    for c in ${SM_CONFIGS}; do \
        make config="$c" all M=1 BRANCH="${SM_TAG}" -j"$(nproc)" >/out/build-$c.log 2>&1 \
            || { tail -30 /out/build-$c.log; exit 1; }; \
        cp "build/$c/m33_image.bin" "/out/sm_$c.bin"; \
        cp "build/$c/m33_image.elf" "/out/sm_$c.elf"; \
    done; \
    { echo "SM_TAG=${SM_TAG}"; echo "SM_COMMIT=$(git rev-parse HEAD)"; \
      echo "SOURCE_DATE_EPOCH=${SOURCE_DATE_EPOCH}"; echo "TC_VERSION=${TC_VERSION}"; \
      echo "TC_SHA256=${TC_SHA256}"; } > /out/BUILDINFO; \
    cd /out && sha256sum sm_*.bin > SHA256SUMS

# Solo gli artefatti: estratti con --output type=local
FROM scratch
COPY --from=build /out/ /
