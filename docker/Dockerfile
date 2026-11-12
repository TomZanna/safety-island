# Ambiente di build: gcc per mkimage, toolchain ARM per l'SM, git per i sorgenti NXP.
# Non serve per flash/uart/debug (USB), solo per sm e image.
#
# Il toolchain ARM NON viene scaricato: si passa come named build context "toolchain"
# (make docker-env lo prende da TOOLCHAIN_DIR). Gli URL di developer.arm.com provati
# per 15.3.rel2 rispondono 404, quindi non sono inseribili come default.
FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential git ca-certificates curl make python3 srecord \
    && rm -rf /var/lib/apt/lists/*

COPY --from=toolchain / /opt/tools/arm-gnu-toolchain-15.3.rel2-x86_64-arm-none-eabi
ENV TOOLS=/opt/tools
RUN test -x /opt/tools/arm-gnu-toolchain-15.3.rel2-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc \
    && /opt/tools/arm-gnu-toolchain-15.3.rel2-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc --version | head -1

WORKDIR /work
