# Ambiente per il target "image" senza installare nulla sull'host:
# cmake per il superbuild, gcc/make per compilare mkimage_imx8, git per scaricarlo,
# python3 per l'ispezione. L'SM ha il suo Docker dedicato e riproducibile: sm/Dockerfile.
FROM ubuntu:24.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential cmake git ca-certificates curl xz-utils make python3 \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /work
